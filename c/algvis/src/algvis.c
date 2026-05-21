#include "algvis.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define AV_MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define AV_MKDIR(path) mkdir(path, 0755)
#endif

/* ========================= Common ========================= */

static char g_error[512] = {0};

const char *av_last_error(void) { return g_error; }

void av_free(void *p) { free(p); }

static int av_set_error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(g_error, sizeof(g_error), fmt, ap);
  va_end(ap);
  return 0;
}

static char *av_strdup_(const char *s) {
  if (!s)
    s = "";
  size_t n = strlen(s);
  char *p = (char *)malloc(n + 1);
  if (!p) {
    av_set_error("内存分配失败");
    return NULL;
  }
  memcpy(p, s, n + 1);
  return p;
}

static int av_replace_str(char **dst, const char *src) {
  char *p = av_strdup_(src ? src : "");
  if (!p)
    return 0;
  free(*dst);
  *dst = p;
  return 1;
}

typedef struct AvSb {
  char *buf;
  size_t len;
  size_t cap;
} AvSb;

static int sb_init(AvSb *sb) {
  sb->cap = 256;
  sb->len = 0;
  sb->buf = (char *)malloc(sb->cap);
  if (!sb->buf)
    return av_set_error("内存分配失败");
  sb->buf[0] = '\0';
  return 1;
}

static void sb_free(AvSb *sb) {
  free(sb->buf);
  sb->buf = NULL;
  sb->len = 0;
  sb->cap = 0;
}

static int sb_reserve(AvSb *sb, size_t need) {
  if (need <= sb->cap)
    return 1;
  size_t nc = sb->cap;
  while (nc < need)
    nc *= 2;
  char *p = (char *)realloc(sb->buf, nc);
  if (!p)
    return av_set_error("内存分配失败");
  sb->buf = p;
  sb->cap = nc;
  return 1;
}

static int sb_addn(AvSb *sb, const char *s, size_t n) {
  if (!sb_reserve(sb, sb->len + n + 1))
    return 0;
  memcpy(sb->buf + sb->len, s, n);
  sb->len += n;
  sb->buf[sb->len] = '\0';
  return 1;
}

static int sb_add(AvSb *sb, const char *s) {
  if (!s)
    s = "";
  return sb_addn(sb, s, strlen(s));
}

static int sb_addc(AvSb *sb, char c) { return sb_addn(sb, &c, 1); }

static int sb_addf(AvSb *sb, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  va_list ap2;
  va_copy(ap2, ap);
  int n = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  if (n < 0) {
    va_end(ap);
    return av_set_error("格式化字符串失败");
  }
  if (!sb_reserve(sb, sb->len + (size_t)n + 1)) {
    va_end(ap);
    return 0;
  }
  vsnprintf(sb->buf + sb->len, sb->cap - sb->len, fmt, ap);
  sb->len += (size_t)n;
  va_end(ap);
  return 1;
}

static char *sb_take(AvSb *sb) {
  char *p = sb->buf;
  sb->buf = NULL;
  sb->len = 0;
  sb->cap = 0;
  return p;
}

static char *dot_quote(const char *s) {
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  sb_addc(&sb, '"');
  if (!s)
    s = "";
  for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
    if (*p == '"' || *p == '\\') {
      sb_addc(&sb, '\\');
      sb_addc(&sb, (char)*p);
    } else if (*p == '\n') {
      sb_add(&sb, "\\n");
    } else if (*p == '\r') {
      /* 忽略 */
    } else {
      sb_addc(&sb, (char)*p);
    }
  }
  sb_addc(&sb, '"');
  return sb_take(&sb);
}

/* Graphviz record label 中，| { } < > 等有结构含义，用户文本需要转义。 */
static char *record_escape(const char *s) {
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  if (!s)
    s = "";
  for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
    if (*p == '\\' || *p == '"' || *p == '{' || *p == '}' || *p == '|' ||
        *p == '<' || *p == '>') {
      sb_addc(&sb, '\\');
      sb_addc(&sb, (char)*p);
    } else if (*p == '\n') {
      sb_add(&sb, "\\n");
    } else if (*p == '\r') {
      /* 忽略 */
    } else {
      sb_addc(&sb, (char)*p);
    }
  }
  return sb_take(&sb);
}

static char *html_escape(const char *s) {
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  if (!s || !*s) {
    sb_add(&sb, "&nbsp;");
    return sb_take(&sb);
  }
  for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
    switch (*p) {
    case '&':
      sb_add(&sb, "&amp;");
      break;
    case '<':
      sb_add(&sb, "&lt;");
      break;
    case '>':
      sb_add(&sb, "&gt;");
      break;
    case '"':
      sb_add(&sb, "&quot;");
      break;
    case '\n':
      sb_add(&sb, "<BR/>");
      break;
    case '\r':
      break;
    default:
      sb_addc(&sb, (char)*p);
      break;
    }
  }
  return sb_take(&sb);
}

typedef struct AvAttr {
  char *key;
  char *value;
} AvAttr;

typedef struct AvAttrList {
  AvAttr *items;
  size_t len;
  size_t cap;
} AvAttrList;

static void attrs_free(AvAttrList *a) {
  if (!a)
    return;
  for (size_t i = 0; i < a->len; ++i) {
    free(a->items[i].key);
    free(a->items[i].value);
  }
  free(a->items);
  a->items = NULL;
  a->len = 0;
  a->cap = 0;
}

static int attrs_set(AvAttrList *a, const char *key, const char *value) {
  if (!a || !key || !*key)
    return av_set_error("属性 key 不能为空");
  for (size_t i = 0; i < a->len; ++i) {
    if (strcmp(a->items[i].key, key) == 0) {
      return av_replace_str(&a->items[i].value, value);
    }
  }
  if (a->len == a->cap) {
    size_t nc = a->cap ? a->cap * 2 : 8;
    AvAttr *p = (AvAttr *)realloc(a->items, nc * sizeof(AvAttr));
    if (!p)
      return av_set_error("内存分配失败");
    a->items = p;
    a->cap = nc;
  }
  a->items[a->len].key = av_strdup_(key);
  a->items[a->len].value = av_strdup_(value ? value : "");
  if (!a->items[a->len].key || !a->items[a->len].value)
    return 0;
  a->len++;
  return 1;
}

static int attrs_to_dot(const AvAttrList *a, AvSb *sb) {
  for (size_t i = 0; i < a->len; ++i) {
    if (i)
      sb_add(sb, ", ");
    char *q = dot_quote(a->items[i].value);
    if (!q)
      return 0;
    sb_addf(sb, "%s=%s", a->items[i].key, q);
    free(q);
  }
  return 1;
}

static int attrs_to_html(const AvAttrList *a, AvSb *sb) {
  for (size_t i = 0; i < a->len; ++i) {
    if (i)
      sb_addc(sb, ' ');
    char *q = dot_quote(a->items[i].value);
    if (!q)
      return 0;
    sb_addf(sb, "%s=%s", a->items[i].key, q);
    free(q);
  }
  return 1;
}

static const char *graph_border_color(AvColor c) {
  switch (c) {
  case AV_COLOR_ORANGE:
    return "#ff8c00";
  case AV_COLOR_RED:
    return "#c0392b";
  case AV_COLOR_GREEN:
    return "#27ae60";
  case AV_COLOR_PURPLE:
    return "#8e44ad";
  case AV_COLOR_TEAL:
    return "#16a085";
  case AV_COLOR_GRAY:
    return "#7f8c8d";
  case AV_COLOR_BLUE:
    return "#1f4e79";
  case AV_COLOR_DEFAULT:
  default:
    return "#8383cc";
  }
}

static const char *graph_fill_color(AvColor c) {
  switch (c) {
  case AV_COLOR_ORANGE:
    return "#ffd8a8";
  case AV_COLOR_RED:
    return "#f5b7b1";
  case AV_COLOR_GREEN:
    return "#abebc6";
  case AV_COLOR_PURPLE:
    return "#e8daef";
  case AV_COLOR_TEAL:
    return "#a3e4d7";
  case AV_COLOR_GRAY:
    return "#ecf0f1";
  case AV_COLOR_BLUE:
    return "#6f9fc7";
  case AV_COLOR_DEFAULT:
  default:
    return "#b2d3e4";
  }
}

static const char *cell_bg_color(AvColor c) {
  switch (c) {
  case AV_COLOR_ORANGE:
    return "#ff8c00";
  case AV_COLOR_RED:
    return "#c0392b";
  case AV_COLOR_GREEN:
    return "#27ae60";
  case AV_COLOR_PURPLE:
    return "#8e44ad";
  case AV_COLOR_TEAL:
    return "#16a085";
  case AV_COLOR_GRAY:
    return "#7f8c8d";
  case AV_COLOR_BLUE:
    return "#1f4e79";
  case AV_COLOR_DEFAULT:
  default:
    return "#b2d3e4";
  }
}

static int write_text_file(const char *path, const char *text) {
  FILE *f = fopen(path, "w");
  if (!f)
    return av_set_error("无法写入文件: %s", path ? path : "(null)");
  fputs(text ? text : "", f);
  fclose(f);
  return 1;
}

static char *path_join(const char *a, const char *b) {
  if (!a)
    a = ".";
  if (!b)
    b = "";
  size_t na = strlen(a), nb = strlen(b);
  int need_sep = na > 0 && a[na - 1] != '/' && a[na - 1] != '\\';
  char *p = (char *)malloc(na + (size_t)need_sep + nb + 1);
  if (!p) {
    av_set_error("内存分配失败");
    return NULL;
  }
  memcpy(p, a, na);
  if (need_sep)
    p[na++] = '/';
  memcpy(p + na, b, nb + 1);
  return p;
}

static char *shell_quote(const char *s) {
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
#ifdef _WIN32
  sb_addc(&sb, '"');
  for (const char *p = s; p && *p; ++p) {
    if (*p == '"')
      sb_add(&sb, "\\\"");
    else
      sb_addc(&sb, *p);
  }
  sb_addc(&sb, '"');
#else
  sb_addc(&sb, '\'');
  for (const char *p = s; p && *p; ++p) {
    if (*p == '\'')
      sb_add(&sb, "'\\''");
    else
      sb_addc(&sb, *p);
  }
  sb_addc(&sb, '\'');
#endif
  return sb_take(&sb);
}

int av_render_dot(const char *dot_path, const char *svg_path, int remove_dot) {
  if (!dot_path || !svg_path)
    return av_set_error("dot_path/svg_path 不能为空");
  char *qd = shell_quote(dot_path);
  char *qs = shell_quote(svg_path);
  if (!qd || !qs) {
    free(qd);
    free(qs);
    return 0;
  }
  AvSb cmd;
  if (!sb_init(&cmd)) {
    free(qd);
    free(qs);
    return 0;
  }
  sb_addf(&cmd, "dot -Tsvg %s -o %s", qd, qs);
  int ret = system(cmd.buf);
  sb_free(&cmd);
  free(qd);
  free(qs);
  if (ret != 0)
    return av_set_error(
        "执行 dot 命令失败，请确认 Graphviz 已安装且 dot 在 PATH 中");
  if (remove_dot)
    remove(dot_path);
  return 1;
}

/* ========================= Graph ========================= */

typedef struct AvNode {
  char *id;
  char *label;
  AvColor color;
} AvNode;

typedef struct AvEdge {
  char *from;
  char *to;
  char *label;
} AvEdge;

struct AvGraph {
  char *name;
  int directed;
  int counter;

  char **infos;
  size_t info_count;
  int *layers;
  size_t layer_count;

  AvNode *nodes;
  size_t node_len;
  size_t node_cap;

  AvEdge *edges;
  size_t edge_len;
  size_t edge_cap;

  AvAttrList graph_attrs;
  AvAttrList node_attrs;
  AvAttrList edge_attrs;
};

static int graph_find_node(const AvGraph *g, const char *id) {
  for (size_t i = 0; i < g->node_len; ++i) {
    if (strcmp(g->nodes[i].id, id) == 0)
      return (int)i;
  }
  return -1;
}

static const char *node_step_id(const char *node_id) {
  if (!node_id)
    return "";
  if (strncmp(node_id, "node", 4) == 0)
    return node_id + 4;
  return node_id;
}

static char *graph_make_label(const AvGraph *g, const char *node_id,
                              const char **values) {
  if (g->info_count == 0) {
    AvSb sb;
    if (!sb_init(&sb))
      return NULL;
    sb_addf(&sb, "step%s", node_step_id(node_id));
    return sb_take(&sb);
  }

  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  sb_addf(&sb, "{step%s", node_step_id(node_id));

  size_t offset = 0;
  for (size_t i = 0; i < g->layer_count; ++i) {
    sb_add(&sb, "|{");
    int cnt = g->layers[i];
    for (int j = 0; j < cnt; ++j) {
      if (j)
        sb_addc(&sb, '|');
      size_t idx = offset + (size_t)j;
      char *ki = record_escape(g->infos[idx]);
      char *vi = record_escape(values ? values[idx] : "");
      if (!ki || !vi) {
        free(ki);
        free(vi);
        sb_free(&sb);
        return NULL;
      }
      sb_addf(&sb, "%s: %s", ki, vi);
      free(ki);
      free(vi);
    }
    sb_addc(&sb, '}');
    offset += (size_t)cnt;
  }
  sb_addc(&sb, '}');
  return sb_take(&sb);
}

AvGraph *av_graph_new(const AvGraphConfig *cfg) {
  if (!cfg)
    return NULL;
  AvGraph *g = (AvGraph *)calloc(1, sizeof(AvGraph));
  if (!g) {
    av_set_error("内存分配失败");
    return NULL;
  }

  g->name = av_strdup_(cfg->name ? cfg->name : "G");
  g->directed = cfg->directed ? 1 : 0;
  if (!g->name)
    goto fail;

  g->info_count = cfg->info_count;
  g->layer_count = cfg->layer_count;

  size_t sum = 0;
  for (size_t i = 0; i < cfg->layer_count; ++i) {
    if (cfg->layers[i] <= 0) {
      av_set_error("layers 中每一层数量必须大于 0");
      goto fail;
    }
    sum += (size_t)cfg->layers[i];
  }
  if (sum != cfg->info_count) {
    av_set_error("layer_count 和 infos 数量不匹配，无法继续");
    goto fail;
  }
  if (cfg->info_count > 0 && !cfg->infos) {
    av_set_error("info_count > 0 时 infos 不能为空");
    goto fail;
  }

  if (cfg->info_count) {
    g->infos = (char **)calloc(cfg->info_count, sizeof(char *));
    if (!g->infos) {
      av_set_error("内存分配失败");
      goto fail;
    }
    for (size_t i = 0; i < cfg->info_count; ++i) {
      g->infos[i] = av_strdup_(cfg->infos[i]);
      if (!g->infos[i])
        goto fail;
    }
  }
  if (cfg->layer_count) {
    g->layers = (int *)calloc(cfg->layer_count, sizeof(int));
    if (!g->layers) {
      av_set_error("内存分配失败");
      goto fail;
    }
    memcpy(g->layers, cfg->layers, cfg->layer_count * sizeof(int));
  }

  av_graph_set_graph_attr(g, "rankdir", "TB");
  av_graph_set_edge_attr(g, "color", "#8383cc");
  av_graph_set_edge_attr(g, "penwidth", "2");
  av_graph_set_edge_attr(g, "fontcolor", "#9c4a4a");
  av_graph_set_edge_attr(g, "fontsize", "10");
  av_graph_set_node_attr(g, "shape", "record");
  av_graph_set_node_attr(g, "fontname", "Consolas");
  av_graph_set_node_attr(g, "fontcolor", "black");
  av_graph_set_node_attr(g, "color", "#8383cc");
  av_graph_set_node_attr(g, "fillcolor", "#b2d3e4");
  av_graph_set_node_attr(g, "style", "filled");
  av_graph_set_node_attr(g, "penwidth", "2");
  if (!g->directed)
    av_graph_set_edge_attr(g, "arrowhead", "none");

  return g;

fail:
  av_graph_free(g);
  return NULL;
}

void av_graph_free(AvGraph *g) {
  if (!g)
    return;
  free(g->name);
  for (size_t i = 0; i < g->info_count; ++i)
    free(g->infos[i]);
  free(g->infos);
  free(g->layers);
  for (size_t i = 0; i < g->node_len; ++i) {
    free(g->nodes[i].id);
    free(g->nodes[i].label);
  }
  free(g->nodes);
  for (size_t i = 0; i < g->edge_len; ++i) {
    free(g->edges[i].from);
    free(g->edges[i].to);
    free(g->edges[i].label);
  }
  free(g->edges);
  attrs_free(&g->graph_attrs);
  attrs_free(&g->node_attrs);
  attrs_free(&g->edge_attrs);
  free(g);
}

int av_graph_set_graph_attr(AvGraph *g, const char *key, const char *value) {
  if (!g)
    return av_set_error("Graph 不能为空");
  return attrs_set(&g->graph_attrs, key, value);
}

int av_graph_set_node_attr(AvGraph *g, const char *key, const char *value) {
  if (!g)
    return av_set_error("Graph 不能为空");
  return attrs_set(&g->node_attrs, key, value);
}

int av_graph_set_edge_attr(AvGraph *g, const char *key, const char *value) {
  if (!g)
    return av_set_error("Graph 不能为空");
  return attrs_set(&g->edge_attrs, key, value);
}

int av_graph_add_node(AvGraph *g, const char **values, AvColor color,
                      char out_id[AV_ID_MAX]) {
  if (!g)
    return av_set_error("Graph 不能为空");
  if (g->info_count > 0 && !values)
    return av_set_error("values 不能为空");

  if (g->node_len == g->node_cap) {
    size_t nc = g->node_cap ? g->node_cap * 2 : 16;
    AvNode *p = (AvNode *)realloc(g->nodes, nc * sizeof(AvNode));
    if (!p)
      return av_set_error("内存分配失败");
    g->nodes = p;
    g->node_cap = nc;
  }

  char id[AV_ID_MAX];
  snprintf(id, sizeof(id), "node%d", ++g->counter);
  char *label = graph_make_label(g, id, values);
  if (!label)
    return 0;

  g->nodes[g->node_len].id = av_strdup_(id);
  g->nodes[g->node_len].label = label;
  g->nodes[g->node_len].color = color;
  if (!g->nodes[g->node_len].id)
    return 0;
  g->node_len++;

  if (out_id) {
    strncpy(out_id, id, AV_ID_MAX - 1);
    out_id[AV_ID_MAX - 1] = '\0';
  }
  return 1;
}

int av_graph_add_node_ints(AvGraph *g, const int *values, size_t count,
                           AvColor color, char out_id[AV_ID_MAX]) {
  if (!g)
    return av_set_error("Graph 不能为空");
  if (count != g->info_count)
    return av_set_error("整数 values 数量必须等于 info_count");
  char **strs = NULL;
  if (count) {
    strs = (char **)calloc(count, sizeof(char *));
    if (!strs)
      return av_set_error("内存分配失败");
  }
  for (size_t i = 0; i < count; ++i) {
    char tmp[64];
    snprintf(tmp, sizeof(tmp), "%d", values[i]);
    strs[i] = av_strdup_(tmp);
    if (!strs[i]) {
      for (size_t j = 0; j < i; ++j)
        free(strs[j]);
      free(strs);
      return 0;
    }
  }
  int ok = av_graph_add_node(g, (const char **)strs, color, out_id);
  for (size_t i = 0; i < count; ++i)
    free(strs[i]);
  free(strs);
  return ok;
}

int av_graph_set_node_values(AvGraph *g, const char *node_id,
                             const char **values) {
  if (!g || !node_id)
    return av_set_error("Graph/node_id 不能为空");
  int idx = graph_find_node(g, node_id);
  if (idx < 0)
    return av_set_error("没有找到节点: %s", node_id);
  char *label = graph_make_label(g, node_id, values);
  if (!label)
    return 0;
  free(g->nodes[idx].label);
  g->nodes[idx].label = label;
  return 1;
}

int av_graph_set_node_color(AvGraph *g, const char *node_id, AvColor color) {
  if (!g || !node_id)
    return av_set_error("Graph/node_id 不能为空");
  int idx = graph_find_node(g, node_id);
  if (idx < 0)
    return av_set_error("没有找到节点: %s", node_id);
  g->nodes[idx].color = color;
  return 1;
}

int av_graph_set_node_values_and_color(AvGraph *g, const char *node_id,
                                       const char **values, AvColor color) {
  if (!av_graph_set_node_values(g, node_id, values))
    return 0;
  return av_graph_set_node_color(g, node_id, color);
}

int av_graph_add_edge(AvGraph *g, const char *from, const char *to,
                      const char *label) {
  if (!g || !from || !to)
    return av_set_error("Graph/from/to 不能为空");
  if (g->edge_len == g->edge_cap) {
    size_t nc = g->edge_cap ? g->edge_cap * 2 : 16;
    AvEdge *p = (AvEdge *)realloc(g->edges, nc * sizeof(AvEdge));
    if (!p)
      return av_set_error("内存分配失败");
    g->edges = p;
    g->edge_cap = nc;
  }
  g->edges[g->edge_len].from = av_strdup_(from);
  g->edges[g->edge_len].to = av_strdup_(to);
  g->edges[g->edge_len].label = av_strdup_(label ? label : "");
  if (!g->edges[g->edge_len].from || !g->edges[g->edge_len].to ||
      !g->edges[g->edge_len].label) {
    return 0;
  }
  g->edge_len++;
  return 1;
}

int av_graph_add_edge_int(AvGraph *g, const char *from, const char *to,
                          int label) {
  char tmp[64];
  snprintf(tmp, sizeof(tmp), "%d", label);
  return av_graph_add_edge(g, from, to, tmp);
}

int av_graph_pop_edge(AvGraph *g) {
  if (!g)
    return av_set_error("Graph 不能为空");
  if (g->edge_len == 0)
    return av_set_error("没有边可以 pop");
  AvEdge *e = &g->edges[g->edge_len - 1];
  free(e->from);
  free(e->to);
  free(e->label);
  g->edge_len--;
  return 1;
}

char *av_graph_to_dot(const AvGraph *g) {
  if (!g) {
    av_set_error("Graph 不能为空");
    return NULL;
  }
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;

  sb_addf(&sb, "%s %s {\n", g->directed ? "digraph" : "graph", g->name);

  if (g->graph_attrs.len) {
    sb_add(&sb, "  graph[");
    attrs_to_dot(&g->graph_attrs, &sb);
    sb_add(&sb, "]\n");
  }
  if (g->edge_attrs.len) {
    sb_add(&sb, "  edge[");
    attrs_to_dot(&g->edge_attrs, &sb);
    sb_add(&sb, "]\n");
  }
  if (g->node_attrs.len) {
    sb_add(&sb, "  node[");
    attrs_to_dot(&g->node_attrs, &sb);
    sb_add(&sb, "]\n");
  }

  sb_addc(&sb, '\n');

  for (size_t i = 0; i < g->node_len; ++i) {
    char *label = dot_quote(g->nodes[i].label);
    if (!label) {
      sb_free(&sb);
      return NULL;
    }
    sb_addf(&sb, "  %s [label=%s", g->nodes[i].id, label);
    free(label);
    if (g->nodes[i].color != AV_COLOR_DEFAULT) {
      sb_addf(&sb, ", color=\"%s\", fillcolor=\"%s\"",
              graph_border_color(g->nodes[i].color),
              graph_fill_color(g->nodes[i].color));
    }
    sb_add(&sb, "]\n");
  }

  sb_addc(&sb, '\n');
  const char *arrow = g->directed ? " -> " : " -- ";
  for (size_t i = 0; i < g->edge_len; ++i) {
    char *label = dot_quote(g->edges[i].label);
    if (!label) {
      sb_free(&sb);
      return NULL;
    }
    sb_addf(&sb, "  %s%s%s [label=%s];\n", g->edges[i].from, arrow,
            g->edges[i].to, label);
    free(label);
  }

  sb_add(&sb, "}\n");
  return sb_take(&sb);
}

int av_graph_write_dot(const AvGraph *g, const char *path) {
  char *dot = av_graph_to_dot(g);
  if (!dot)
    return 0;
  int ok = write_text_file(path, dot);
  free(dot);
  return ok;
}

int av_graph_export_svg(const AvGraph *g, const char *dot_path,
                        const char *svg_path, int remove_dot) {
  if (!av_graph_write_dot(g, dot_path))
    return 0;
  return av_render_dot(dot_path, svg_path, remove_dot);
}

/* ========================= Matrix ========================= */

typedef struct AvCell {
  char *text;
  char *marker;
  AvColor color;
} AvCell;

struct AvMatrix {
  char *name;
  int rows;
  int cols;
  AvAttrList table_attrs;
  AvCell *cells;
};

static int matrix_index(const AvMatrix *m, int r, int c, int *out) {
  if (!m)
    return av_set_error("Matrix 不能为空");
  if (r < 0 || r >= m->rows || c < 0 || c >= m->cols) {
    return av_set_error("Matrix 下标越界: (%d, %d)", r, c);
  }
  *out = r * m->cols + c;
  return 1;
}

AvMatrix *av_matrix_new(int rows, int cols, AvColor default_color,
                        const char *name) {
  if (rows <= 0 || cols <= 0) {
    av_set_error("Matrix 行列数必须大于 0");
    return NULL;
  }
  AvMatrix *m = (AvMatrix *)calloc(1, sizeof(AvMatrix));
  if (!m) {
    av_set_error("内存分配失败");
    return NULL;
  }
  m->name = av_strdup_(name ? name : "G");
  m->rows = rows;
  m->cols = cols;
  m->cells = (AvCell *)calloc((size_t)rows * (size_t)cols, sizeof(AvCell));
  if (!m->name || !m->cells) {
    av_set_error("内存分配失败");
    av_matrix_free(m);
    return NULL;
  }
  for (int i = 0; i < rows * cols; ++i) {
    m->cells[i].text = av_strdup_("");
    m->cells[i].marker = av_strdup_("");
    m->cells[i].color = default_color;
  }
  av_matrix_set_table_attr(m, "BORDER", "0");
  av_matrix_set_table_attr(m, "CELLBORDER", "1");
  av_matrix_set_table_attr(m, "CELLSPACING", "0");
  av_matrix_set_table_attr(m, "COLOR", "#8383cc");
  return m;
}

void av_matrix_free(AvMatrix *m) {
  if (!m)
    return;
  free(m->name);
  if (m->cells) {
    for (int i = 0; i < m->rows * m->cols; ++i) {
      free(m->cells[i].text);
      free(m->cells[i].marker);
    }
  }
  free(m->cells);
  attrs_free(&m->table_attrs);
  free(m);
}

int av_matrix_set_table_attr(AvMatrix *m, const char *key, const char *value) {
  if (!m)
    return av_set_error("Matrix 不能为空");
  return attrs_set(&m->table_attrs, key, value);
}

int av_matrix_set(AvMatrix *m, int row, int col, const char *text) {
  int idx;
  if (!matrix_index(m, row, col, &idx))
    return 0;
  return av_replace_str(&m->cells[idx].text, text ? text : "");
}

int av_matrix_set_int(AvMatrix *m, int row, int col, int value) {
  char tmp[64];
  snprintf(tmp, sizeof(tmp), "%d", value);
  return av_matrix_set(m, row, col, tmp);
}

int av_matrix_set_color(AvMatrix *m, int row, int col, AvColor color) {
  int idx;
  if (!matrix_index(m, row, col, &idx))
    return 0;
  m->cells[idx].color = color;
  return 1;
}

int av_matrix_mark(AvMatrix *m, int row, int col, const char *marker) {
  int idx;
  if (!matrix_index(m, row, col, &idx))
    return 0;
  return av_replace_str(&m->cells[idx].marker, marker ? marker : "");
}

int av_matrix_clear_mark(AvMatrix *m, int row, int col) {
  return av_matrix_mark(m, row, col, "");
}

char *av_matrix_to_dot(const AvMatrix *m) {
  if (!m) {
    av_set_error("Matrix 不能为空");
    return NULL;
  }
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  sb_addf(&sb, "digraph %s {\n", m->name);
  sb_add(&sb, "  node [shape=plain]\n");
  sb_add(&sb, "  matrix [label=<\n");
  sb_add(&sb, "    <TABLE ");
  attrs_to_html(&m->table_attrs, &sb);
  sb_add(&sb, ">\n");

  for (int r = 0; r < m->rows; ++r) {
    sb_add(&sb, "      <TR>\n");
    for (int c = 0; c < m->cols; ++c) {
      int idx = r * m->cols + c;
      char *text = html_escape(m->cells[idx].text);
      char *mark = html_escape(m->cells[idx].marker);
      if (!text || !mark) {
        free(text);
        free(mark);
        sb_free(&sb);
        return NULL;
      }
      sb_addf(&sb,
              "        <TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" "
              "FIXEDSIZE=\"TRUE\" WIDTH=\"60\" HEIGHT=\"40\" "
              "BGCOLOR=\"%s\">%s",
              cell_bg_color(m->cells[idx].color), text);
      if (m->cells[idx].marker && m->cells[idx].marker[0]) {
        sb_addf(&sb, "<BR/><FONT COLOR=\"red\" POINT-SIZE=\"8\">%s</FONT>",
                mark);
      }
      sb_add(&sb, "</TD>\n");
      free(text);
      free(mark);
    }
    sb_add(&sb, "      </TR>\n");
  }

  sb_add(&sb, "    </TABLE>\n");
  sb_add(&sb, "  >];\n");
  sb_add(&sb, "}\n");
  return sb_take(&sb);
}

int av_matrix_write_dot(const AvMatrix *m, const char *path) {
  char *dot = av_matrix_to_dot(m);
  if (!dot)
    return 0;
  int ok = write_text_file(path, dot);
  free(dot);
  return ok;
}

int av_matrix_export_svg(const AvMatrix *m, const char *dot_path,
                         const char *svg_path, int remove_dot) {
  if (!av_matrix_write_dot(m, dot_path))
    return 0;
  return av_render_dot(dot_path, svg_path, remove_dot);
}

/* ========================= Array ========================= */

typedef struct AvArrayItem {
  char *text;
  char *marker;
  AvColor color;
} AvArrayItem;

struct AvArray {
  char *name;
  size_t length;
  AvAttrList table_attrs;
  AvArrayItem *items;
};

static int array_check(const AvArray *a, size_t index) {
  if (!a)
    return av_set_error("Array 不能为空");
  if (index >= a->length)
    return av_set_error("Array 下标越界: %zu", index);
  return 1;
}

AvArray *av_array_new(size_t length, const char *name) {
  if (length == 0) {
    av_set_error("Array 长度必须大于 0");
    return NULL;
  }
  AvArray *a = (AvArray *)calloc(1, sizeof(AvArray));
  if (!a) {
    av_set_error("内存分配失败");
    return NULL;
  }
  a->name = av_strdup_(name ? name : "G");
  a->length = length;
  a->items = (AvArrayItem *)calloc(length, sizeof(AvArrayItem));
  if (!a->name || !a->items) {
    av_set_error("内存分配失败");
    av_array_free(a);
    return NULL;
  }
  for (size_t i = 0; i < length; ++i) {
    a->items[i].text = av_strdup_("");
    a->items[i].marker = av_strdup_("");
    a->items[i].color = AV_COLOR_DEFAULT;
  }
  av_array_set_table_attr(a, "BORDER", "0");
  av_array_set_table_attr(a, "CELLBORDER", "1");
  av_array_set_table_attr(a, "CELLSPACING", "0");
  av_array_set_table_attr(a, "COLOR", "#8383cc");
  return a;
}

void av_array_free(AvArray *a) {
  if (!a)
    return;
  free(a->name);
  if (a->items) {
    for (size_t i = 0; i < a->length; ++i) {
      free(a->items[i].text);
      free(a->items[i].marker);
    }
  }
  free(a->items);
  attrs_free(&a->table_attrs);
  free(a);
}

int av_array_set_table_attr(AvArray *a, const char *key, const char *value) {
  if (!a)
    return av_set_error("Array 不能为空");
  return attrs_set(&a->table_attrs, key, value);
}

int av_array_set_text(AvArray *a, size_t index, const char *text) {
  if (!array_check(a, index))
    return 0;
  return av_replace_str(&a->items[index].text, text ? text : "");
}

int av_array_set_int(AvArray *a, size_t index, int value) {
  char tmp[64];
  snprintf(tmp, sizeof(tmp), "%d", value);
  return av_array_set_text(a, index, tmp);
}

int av_array_set_color(AvArray *a, size_t index, AvColor color) {
  if (!array_check(a, index))
    return 0;
  a->items[index].color = color;
  return 1;
}

int av_array_mark(AvArray *a, size_t index, const char *marker) {
  if (!array_check(a, index))
    return 0;
  return av_replace_str(&a->items[index].marker, marker ? marker : "");
}

int av_array_clear_mark(AvArray *a, size_t index) {
  return av_array_mark(a, index, "");
}

int av_array_swap(AvArray *a, size_t i, size_t j) {
  if (!array_check(a, i) || !array_check(a, j))
    return 0;
  AvArrayItem tmp = a->items[i];
  a->items[i] = a->items[j];
  a->items[j] = tmp;
  return 1;
}

char *av_array_to_dot(const AvArray *a) {
  if (!a) {
    av_set_error("Array 不能为空");
    return NULL;
  }
  AvSb sb;
  if (!sb_init(&sb))
    return NULL;
  int has_marker = 0;
  for (size_t i = 0; i < a->length; ++i) {
    if (a->items[i].marker && a->items[i].marker[0]) {
      has_marker = 1;
      break;
    }
  }

  sb_addf(&sb, "digraph %s {\n", a->name);
  sb_add(&sb, "  node [shape=plain]\n");
  sb_add(&sb, "  array [label=<\n");
  sb_add(&sb, "    <TABLE ");
  attrs_to_html(&a->table_attrs, &sb);
  sb_add(&sb, ">\n");

  sb_add(&sb, "      <TR>\n");
  for (size_t i = 0; i < a->length; ++i) {
    sb_addf(&sb,
            "        <TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" "
            "FIXEDSIZE=\"TRUE\" WIDTH=\"60\" HEIGHT=\"24\" "
            "BGCOLOR=\"#ecf0f1\"><FONT POINT-SIZE=\"10\">%zu</FONT></TD>\n",
            i);
  }
  sb_add(&sb, "      </TR>\n");

  sb_add(&sb, "      <TR>\n");
  for (size_t i = 0; i < a->length; ++i) {
    char *text = html_escape(a->items[i].text);
    if (!text) {
      sb_free(&sb);
      return NULL;
    }
    sb_addf(&sb,
            "        <TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" "
            "FIXEDSIZE=\"TRUE\" WIDTH=\"60\" HEIGHT=\"40\" "
            "BGCOLOR=\"%s\">%s</TD>\n",
            cell_bg_color(a->items[i].color), text);
    free(text);
  }
  sb_add(&sb, "      </TR>\n");

  if (has_marker) {
    sb_add(&sb, "      <TR>\n");
    for (size_t i = 0; i < a->length; ++i) {
      char *mark = html_escape(a->items[i].marker);
      if (!mark) {
        sb_free(&sb);
        return NULL;
      }
      sb_addf(&sb,
              "        <TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" "
              "FIXEDSIZE=\"TRUE\" WIDTH=\"60\" HEIGHT=\"24\">"
              "<FONT COLOR=\"red\" POINT-SIZE=\"9\">%s</FONT></TD>\n",
              mark);
      free(mark);
    }
    sb_add(&sb, "      </TR>\n");
  }

  sb_add(&sb, "    </TABLE>\n");
  sb_add(&sb, "  >];\n");
  sb_add(&sb, "}\n");
  return sb_take(&sb);
}

int av_array_write_dot(const AvArray *a, const char *path) {
  char *dot = av_array_to_dot(a);
  if (!dot)
    return 0;
  int ok = write_text_file(path, dot);
  free(dot);
  return ok;
}

int av_array_export_svg(const AvArray *a, const char *dot_path,
                        const char *svg_path, int remove_dot) {
  if (!av_array_write_dot(a, dot_path))
    return 0;
  return av_render_dot(dot_path, svg_path, remove_dot);
}

/* ========================= Trace ========================= */

typedef struct AvTraceStep {
  char *caption;
  char *dot_file;
  char *svg_file;
  int svg_ok;
} AvTraceStep;

struct AvTrace {
  char *dir;
  char *title;
  int render_svg;
  int remove_dot_after_svg;
  size_t count;
  size_t cap;
  AvTraceStep *steps;
};

static int ensure_dir(const char *dir) {
  if (!dir || !*dir)
    return av_set_error("输出目录不能为空");
  if (AV_MKDIR(dir) != 0 && errno != EEXIST) {
    return av_set_error("无法创建目录: %s", dir);
  }
  return 1;
}

AvTrace *av_trace_new(const AvTraceConfig *cfg) {
  if (!cfg || !cfg->dir) {
    av_set_error("TraceConfig/dir 不能为空");
    return NULL;
  }
  if (!ensure_dir(cfg->dir))
    return NULL;
  AvTrace *t = (AvTrace *)calloc(1, sizeof(AvTrace));
  if (!t) {
    av_set_error("内存分配失败");
    return NULL;
  }
  t->dir = av_strdup_(cfg->dir);
  t->title = av_strdup_(cfg->title ? cfg->title : "Algorithm Trace");
  t->render_svg = cfg->render_svg;
  t->remove_dot_after_svg = cfg->remove_dot_after_svg;
  if (!t->dir || !t->title) {
    av_trace_free(t);
    return NULL;
  }
  return t;
}

void av_trace_free(AvTrace *t) {
  if (!t)
    return;
  free(t->dir);
  free(t->title);
  for (size_t i = 0; i < t->count; ++i) {
    free(t->steps[i].caption);
    free(t->steps[i].dot_file);
    free(t->steps[i].svg_file);
  }
  free(t->steps);
  free(t);
}

static int trace_add_dot(AvTrace *t, const char *dot, const char *caption) {
  if (!t || !dot)
    return av_set_error("Trace/dot 不能为空");
  if (t->count == t->cap) {
    size_t nc = t->cap ? t->cap * 2 : 16;
    AvTraceStep *p = (AvTraceStep *)realloc(t->steps, nc * sizeof(AvTraceStep));
    if (!p)
      return av_set_error("内存分配失败");
    t->steps = p;
    t->cap = nc;
  }

  char dot_name[64];
  char svg_name[64];
  snprintf(dot_name, sizeof(dot_name), "step_%03zu.dot", t->count + 1);
  snprintf(svg_name, sizeof(svg_name), "step_%03zu.svg", t->count + 1);

  char *dot_path = path_join(t->dir, dot_name);
  char *svg_path = path_join(t->dir, svg_name);
  if (!dot_path || !svg_path) {
    free(dot_path);
    free(svg_path);
    return 0;
  }

  if (!write_text_file(dot_path, dot)) {
    free(dot_path);
    free(svg_path);
    return 0;
  }

  int svg_ok = 0;
  if (t->render_svg)
    svg_ok = av_render_dot(dot_path, svg_path, t->remove_dot_after_svg);

  AvTraceStep *s = &t->steps[t->count++];
  s->caption = av_strdup_(caption ? caption : "");
  s->dot_file = av_strdup_(dot_name);
  s->svg_file = av_strdup_(svg_name);
  s->svg_ok = svg_ok;

  free(dot_path);
  free(svg_path);
  return s->caption && s->dot_file && s->svg_file;
}

int av_trace_add_graph(AvTrace *t, const AvGraph *g, const char *caption) {
  char *dot = av_graph_to_dot(g);
  if (!dot)
    return 0;
  int ok = trace_add_dot(t, dot, caption);
  free(dot);
  return ok;
}

int av_trace_add_matrix(AvTrace *t, const AvMatrix *m, const char *caption) {
  char *dot = av_matrix_to_dot(m);
  if (!dot)
    return 0;
  int ok = trace_add_dot(t, dot, caption);
  free(dot);
  return ok;
}

int av_trace_add_array(AvTrace *t, const AvArray *a, const char *caption) {
  char *dot = av_array_to_dot(a);
  if (!dot)
    return 0;
  int ok = trace_add_dot(t, dot, caption);
  free(dot);
  return ok;
}

int av_trace_write_index(const AvTrace *t) {
  if (!t)
    return av_set_error("Trace 不能为空");
  AvSb html;
  if (!sb_init(&html))
    return 0;

  char *title = html_escape(t->title);
  if (!title) {
    sb_free(&html);
    return 0;
  }

  sb_add(&html, "<!doctype html>\n<html lang=\"zh-CN\">\n<head>\n");
  sb_add(&html, "<meta charset=\"utf-8\">\n");
  sb_addf(&html, "<title>%s</title>\n", title);
  sb_add(&html,
         "<style>\n"
         "body{font-family:Arial,'Microsoft "
         "YaHei',sans-serif;margin:24px;background:#fafafa;color:#222;}\n"
         "h1{margin-bottom:8px;}\n"
         ".step{background:white;border:1px solid "
         "#ddd;border-radius:12px;padding:16px;margin:18px 0;box-shadow:0 2px "
         "8px rgba(0,0,0,.04);}\n"
         ".caption{font-weight:700;margin-bottom:10px;}\n"
         "img{max-width:100%;height:auto;border:1px solid "
         "#eee;background:white;}\n"
         "a{color:#1f4e79;}\n"
         "</style>\n</head>\n<body>\n");
  sb_addf(&html, "<h1>%s</h1>\n", title);
  sb_addf(&html, "<p>共 %zu 个算法切片。</p>\n", t->count);
  free(title);

  for (size_t i = 0; i < t->count; ++i) {
    char *cap = html_escape(t->steps[i].caption);
    if (!cap) {
      sb_free(&html);
      return 0;
    }
    sb_add(&html, "<section class=\"step\">\n");
    sb_addf(&html, "<div class=\"caption\">Step %zu：%s</div>\n", i + 1, cap);
    if (t->steps[i].svg_ok) {
      sb_addf(&html, "<img src=\"%s\" alt=\"step %zu\">\n",
              t->steps[i].svg_file, i + 1);
    } else {
      sb_addf(&html,
              "<p>未生成 SVG。你可以手动运行：<code>dot -Tsvg %s -o "
              "%s</code></p>\n",
              t->steps[i].dot_file, t->steps[i].svg_file);
      sb_addf(&html, "<p><a href=\"%s\">查看 dot 文件</a></p>\n",
              t->steps[i].dot_file);
    }
    sb_add(&html, "</section>\n");
    free(cap);
  }

  sb_add(&html, "</body>\n</html>\n");
  char *index_path = path_join(t->dir, "index.html");
  if (!index_path) {
    sb_free(&html);
    return 0;
  }
  int ok = write_text_file(index_path, html.buf);
  free(index_path);
  sb_free(&html);
  return ok;
}
