#include "algvis_api.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char g_api_error[512] = {0};

static int api_error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(g_api_error, sizeof(g_api_error), fmt, ap);
  va_end(ap);
  return 0;
}

static void clear_api_error(void) { g_api_error[0] = '\0'; }

const char *last_error(void) {
  if (g_api_error[0])
    return g_api_error;
  return av_last_error();
}

static AvColor to_av_color(Color c) { return (AvColor)c; }

static AvTraceConfig to_av_trace_config(const TraceConfig *tc) {
  AvTraceConfig out;
  out.dir = tc ? tc->dir : NULL;
  out.title = tc ? tc->title : NULL;
  out.render_svg = tc ? tc->render_svg : 0;
  out.remove_dot_after_svg = tc ? tc->remove_dot_after_svg : 0;
  return out;
}

static AvGraphConfig to_av_graph_config(const GraphConfig *gc, VisType type,
                                        const char **default_infos,
                                        int *default_layers) {
  AvGraphConfig out;

  const char *default_name = "G";
  if (type == List)
    default_name = "List";
  if (type == Tree)
    default_name = "Tree";

  out.name = gc && gc->name ? gc->name : default_name;

  if (type == List || type == Tree) {
    out.directed = 1;
  } else {
    out.directed = gc ? gc->directed : 1;
  }

  if (gc && gc->infos && gc->info_count > 0) {
    out.infos = gc->infos;
    out.info_count = gc->info_count;
    out.layers = gc->layers;
    out.layer_count = gc->layer_count;
  } else {
    default_infos[0] = "value";
    default_layers[0] = 1;
    out.infos = default_infos;
    out.info_count = 1;
    out.layers = default_layers;
    out.layer_count = 1;
  }

  return out;
}

State *new_state(const StateConfig *cfg) {
  clear_api_error();

  if (!cfg) {
    api_error("StateConfig 不能为空");
    return NULL;
  }

  State *s = (State *)calloc(1, sizeof(State));
  if (!s) {
    api_error("内存分配失败");
    return NULL;
  }

  s->type = cfg->type;

  if (cfg->trace) {
    AvTraceConfig tc = to_av_trace_config(cfg->trace);
    s->trace = av_trace_new(&tc);
    if (!s->trace) {
      free_state(s);
      return NULL;
    }
  }

  switch (cfg->type) {
  case Array: {
    size_t n = cfg->as.array.length;
    const char *name = cfg->as.array.name ? cfg->as.array.name : "Array";
    s->array = av_array_new(n, name);
    if (!s->array) {
      free_state(s);
      return NULL;
    }
    break;
  }

  case Matrix: {
    int rows = cfg->as.matrix.rows;
    int cols = cfg->as.matrix.cols;
    Color color = cfg->as.matrix.default_color;
    const char *name = cfg->as.matrix.name ? cfg->as.matrix.name : "Matrix";
    s->matrix = av_matrix_new(rows, cols, to_av_color(color), name);
    if (!s->matrix) {
      free_state(s);
      return NULL;
    }
    break;
  }

  case Graph:
  case List:
  case Tree: {
    const char *default_infos[1];
    int default_layers[1];
    AvGraphConfig gc = to_av_graph_config(&cfg->as.graph, cfg->type,
                                          default_infos, default_layers);
    s->graph = av_graph_new(&gc);
    if (!s->graph) {
      free_state(s);
      return NULL;
    }

    if (cfg->type == List) {
      av_graph_set_graph_attr(s->graph, "rankdir", "LR");
    } else if (cfg->type == Tree) {
      av_graph_set_graph_attr(s->graph, "rankdir", "TB");
    }
    break;
  }

  default:
    api_error("未知 State 类型");
    free_state(s);
    return NULL;
  }

  return s;
}

void free_state(State *s) {
  if (!s)
    return;
  av_trace_free(s->trace);
  av_array_free(s->array);
  av_matrix_free(s->matrix);
  av_graph_free(s->graph);
  free(s);
}

int snapshot(State *s, const char *caption) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (!s->trace)
    return api_error("State 没有绑定 Trace，无法 snapshot");

  switch (s->type) {
  case Array:
    return av_trace_add_array(s->trace, s->array, caption);
  case Matrix:
    return av_trace_add_matrix(s->trace, s->matrix, caption);
  case Graph:
  case List:
  case Tree:
    return av_trace_add_graph(s->trace, s->graph, caption);
  default:
    return api_error("未知 State 类型");
  }
}

int write_index(State *s) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (!s->trace)
    return api_error("State 没有绑定 Trace，无法生成 index.html");
  return av_trace_write_index(s->trace);
}

int set_text(State *s, int i, int j, const char *text) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0)
      return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_text(s->array, (size_t)i, text);
  case Matrix:
    return av_matrix_set(s->matrix, i, j, text);
  default:
    return api_error("set_text 只支持 Array / Matrix");
  }
}

int set_int(State *s, int i, int j, int value) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0)
      return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_int(s->array, (size_t)i, value);
  case Matrix:
    return av_matrix_set_int(s->matrix, i, j, value);
  default:
    return api_error("set_int 只支持 Array / Matrix");
  }
}

int set_color(State *s, int i, int j, Color color) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0)
      return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_color(s->array, (size_t)i, to_av_color(color));
  case Matrix:
    return av_matrix_set_color(s->matrix, i, j, to_av_color(color));
  default:
    return api_error("set_color 只支持 Array / Matrix");
  }
}

int mark(State *s, int i, int j, const char *text) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0)
      return api_error("Array 下标不能为负数: %d", i);
    return av_array_mark(s->array, (size_t)i, text);
  case Matrix:
    return av_matrix_mark(s->matrix, i, j, text);
  default:
    return api_error("mark 只支持 Array / Matrix");
  }
}

int clear_mark(State *s, int i, int j) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0)
      return api_error("Array 下标不能为负数: %d", i);
    return av_array_clear_mark(s->array, (size_t)i);
  case Matrix:
    return av_matrix_clear_mark(s->matrix, i, j);
  default:
    return api_error("clear_mark 只支持 Array / Matrix");
  }
}

int array_swap(State *s, size_t i, size_t j) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != Array)
    return api_error("array_swap 只支持 Array");
  return av_array_swap(s->array, i, j);
}

static int ensure_graph_state(State *s, const char *func) {
  if (!s)
    return api_error("State 不能为空");
  if (s->type != Graph && s->type != List && s->type != Tree) {
    return api_error("%s 只支持 Graph / List / Tree", func);
  }
  return 1;
}

int graph_node(State *s, const char **values, Color color,
               char out_id[ID_MAX]) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node"))
    return 0;
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int graph_node_ints(State *s, const int *values, size_t count, Color color,
                    char out_id[ID_MAX]) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_ints"))
    return 0;
  return av_graph_add_node_ints(s->graph, values, count, to_av_color(color),
                                out_id);
}

int graph_edge(State *s, const char *from, const char *to, const char *label) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_edge"))
    return 0;
  return av_graph_add_edge(s->graph, from, to, label);
}

int graph_edge_int(State *s, const char *from, const char *to, int label) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_edge_int"))
    return 0;
  return av_graph_add_edge_int(s->graph, from, to, label);
}

int graph_node_color(State *s, const char *node_id, Color color) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_color"))
    return 0;
  return av_graph_set_node_color(s->graph, node_id, to_av_color(color));
}

int graph_node_values(State *s, const char *node_id, const char **values) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_values"))
    return 0;
  return av_graph_set_node_values(s->graph, node_id, values);
}

int graph_node_values_color(State *s, const char *node_id, const char **values,
                            Color color) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_values_color"))
    return 0;
  return av_graph_set_node_values_and_color(s->graph, node_id, values,
                                            to_av_color(color));
}

int list_node(State *s, const char *value, Color color, char out_id[ID_MAX]) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != List && s->type != Graph) {
    return api_error("list_node 只支持 List / Graph");
  }

  const char *values[] = {value ? value : ""};
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int list_next(State *s, const char *from, const char *to) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != List && s->type != Graph) {
    return api_error("list_next 只支持 List / Graph");
  }

  return av_graph_add_edge(s->graph, from, to, "next");
}

int tree_node(State *s, const char *value, Color color, char out_id[ID_MAX]) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_node 只支持 Tree / Graph");
  }

  const char *values[] = {value ? value : ""};
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int tree_child(State *s, const char *parent, const char *child_value,
               Color color, char child_id[ID_MAX]) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_child 只支持 Tree / Graph");
  }

  if (!tree_node(s, child_value, color, child_id))
    return 0;
  return av_graph_add_edge(s->graph, parent, child_id, "");
}

int tree_edge(State *s, const char *parent, const char *child,
              const char *label) {
  clear_api_error();

  if (!s)
    return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_edge 只支持 Tree / Graph");
  }

  return av_graph_add_edge(s->graph, parent, child, label);
}
