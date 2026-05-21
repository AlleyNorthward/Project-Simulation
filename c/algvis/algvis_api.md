# algvis-c API Layer v2

这个版本不推翻你已有的 `algvis.h / algvis.c`，而是在上面新增一层更好记的 API：

```text
底层：AvGraph / AvMatrix / AvArray / AvTrace
上层：State + new_state() + snapshot() + 少量对象函数
```

你平时写题时尽量只接触：

```c
State *s = new_state(&cfg);
snapshot(s, "当前步骤");
write_index(s);
free_state(s);
```

---

## 推荐目录结构

```text
algvis-c/
  CMakeLists.txt
  algvis.h
  algvis.c
  algvis_api.h
  algvis_api.c
  examples/
    api_array_sort.c
    api_matrix_bfs.c
    api_list.c
```

---

# 1. algvis_api.h

```c
#ifndef ALGVIS_API_H
#define ALGVIS_API_H

#include "algvis.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ID_MAX AV_ID_MAX

/*
 * 对外隐藏 AV_COLOR_ 前缀。
 * 底层仍然复用 AvColor。
 */
typedef enum Color {
  Default = AV_COLOR_DEFAULT,
  Orange  = AV_COLOR_ORANGE,
  Red     = AV_COLOR_RED,
  Green   = AV_COLOR_GREEN,
  Purple  = AV_COLOR_PURPLE,
  Teal    = AV_COLOR_TEAL,
  Gray    = AV_COLOR_GRAY,
  Blue    = AV_COLOR_BLUE
} Color;

/*
 * 统一类型。
 * List / Tree 底层都是 Graph。
 */
typedef enum VisType {
  Array,
  Matrix,
  Graph,
  List,
  Tree
} VisType;

typedef struct TraceConfig {
  const char *dir;
  const char *title;
  int render_svg;
  int remove_dot_after_svg;
} TraceConfig;

typedef struct ArrayConfig {
  size_t length;
  const char *name;
} ArrayConfig;

typedef struct MatrixConfig {
  int rows;
  int cols;
  Color default_color;
  const char *name;
} MatrixConfig;

typedef struct GraphConfig {
  const char *name;
  int directed;

  /*
   * 可选。
   * 如果 infos == NULL 或 info_count == 0，默认使用一个字段：value。
   */
  const char **infos;
  size_t info_count;
  const int *layers;
  size_t layer_count;
} GraphConfig;

/*
 * new_state 的统一配置。
 * 根据 type 只读取 array / matrix / graph 中对应的那一项。
 */
typedef struct StateConfig {
  VisType type;
  const TraceConfig *trace;

  union {
    ArrayConfig array;
    MatrixConfig matrix;
    GraphConfig graph;
  } as;
} StateConfig;

/*
 * 用户主要操作的状态对象。
 * 内部保留底层指针。
 */
typedef struct State {
  VisType type;
  AvArray *array;
  AvMatrix *matrix;
  AvGraph *graph;
  AvTrace *trace;
} State;

const char *last_error(void);

State *new_state(const StateConfig *cfg);
void free_state(State *s);

/*
 * 保存当前切片。
 * Array  -> av_trace_add_array
 * Matrix -> av_trace_add_matrix
 * Graph/List/Tree -> av_trace_add_graph
 */
int snapshot(State *s, const char *caption);
int write_index(State *s);

/*
 * Array / Matrix 统一接口。
 * 对 Array：i 是下标，j 忽略，传 0 即可。
 * 对 Matrix：i 是 row，j 是 col。
 */
int set_text(State *s, int i, int j, const char *text);
int set_int(State *s, int i, int j, int value);
int set_color(State *s, int i, int j, Color color);
int mark(State *s, int i, int j, const char *text);
int clear_mark(State *s, int i, int j);

/* Array 独有。 */
int array_swap(State *s, size_t i, size_t j);

/* Graph / List / Tree 共用。 */
int graph_node(State *s, const char **values, Color color,
               char out_id[ID_MAX]);
int graph_node_ints(State *s, const int *values, size_t count, Color color,
                    char out_id[ID_MAX]);
int graph_edge(State *s, const char *from, const char *to,
               const char *label);
int graph_edge_int(State *s, const char *from, const char *to, int label);
int graph_node_color(State *s, const char *node_id, Color color);
int graph_node_values(State *s, const char *node_id, const char **values);
int graph_node_values_color(State *s, const char *node_id,
                            const char **values, Color color);

/*
 * List 是 Graph 的简化包装。
 * 默认是有向图，rankdir=LR。
 */
int list_node(State *s, const char *value, Color color, char out_id[ID_MAX]);
int list_next(State *s, const char *from, const char *to);

/*
 * Tree 也是 Graph 的简化包装。
 * 默认是有向图，rankdir=TB。
 */
int tree_node(State *s, const char *value, Color color, char out_id[ID_MAX]);
int tree_child(State *s, const char *parent, const char *child_value,
               Color color, char child_id[ID_MAX]);
int tree_edge(State *s, const char *parent, const char *child,
              const char *label);

/*
 * 为了少敲参数，提供几个简单宏。
 * 这些宏不是必须用。
 */
#define array_set_text(s, i, text) set_text((s), (int)(i), 0, (text))
#define array_set_int(s, i, value) set_int((s), (int)(i), 0, (value))
#define array_set_color(s, i, color) set_color((s), (int)(i), 0, (color))
#define array_mark(s, i, text) mark((s), (int)(i), 0, (text))
#define array_clear_mark(s, i) clear_mark((s), (int)(i), 0)

#define matrix_set_text(s, r, c, text) set_text((s), (r), (c), (text))
#define matrix_set_int(s, r, c, value) set_int((s), (r), (c), (value))
#define matrix_set_color(s, r, c, color) set_color((s), (r), (c), (color))
#define matrix_mark(s, r, c, text) mark((s), (r), (c), (text))
#define matrix_clear_mark(s, r, c) clear_mark((s), (r), (c))

#ifdef __cplusplus
}
#endif

#endif /* ALGVIS_API_H */
```

---

# 2. algvis_api.c

```c
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

static void clear_api_error(void) {
  g_api_error[0] = '\0';
}

const char *last_error(void) {
  if (g_api_error[0]) return g_api_error;
  return av_last_error();
}

static AvColor to_av_color(Color c) {
  return (AvColor)c;
}

static AvTraceConfig to_av_trace_config(const TraceConfig *tc) {
  AvTraceConfig out;
  out.dir = tc ? tc->dir : NULL;
  out.title = tc ? tc->title : NULL;
  out.render_svg = tc ? tc->render_svg : 0;
  out.remove_dot_after_svg = tc ? tc->remove_dot_after_svg : 0;
  return out;
}

static AvGraphConfig to_av_graph_config(const GraphConfig *gc,
                                        VisType type,
                                        const char **default_infos,
                                        int *default_layers) {
  AvGraphConfig out;

  const char *default_name = "G";
  if (type == List) default_name = "List";
  if (type == Tree) default_name = "Tree";

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
  if (!s) return;
  av_trace_free(s->trace);
  av_array_free(s->array);
  av_matrix_free(s->matrix);
  av_graph_free(s->graph);
  free(s);
}

int snapshot(State *s, const char *caption) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (!s->trace) return api_error("State 没有绑定 Trace，无法 snapshot");

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

  if (!s) return api_error("State 不能为空");
  if (!s->trace) return api_error("State 没有绑定 Trace，无法生成 index.html");
  return av_trace_write_index(s->trace);
}

int set_text(State *s, int i, int j, const char *text) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0) return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_text(s->array, (size_t)i, text);
  case Matrix:
    return av_matrix_set(s->matrix, i, j, text);
  default:
    return api_error("set_text 只支持 Array / Matrix");
  }
}

int set_int(State *s, int i, int j, int value) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0) return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_int(s->array, (size_t)i, value);
  case Matrix:
    return av_matrix_set_int(s->matrix, i, j, value);
  default:
    return api_error("set_int 只支持 Array / Matrix");
  }
}

int set_color(State *s, int i, int j, Color color) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0) return api_error("Array 下标不能为负数: %d", i);
    return av_array_set_color(s->array, (size_t)i, to_av_color(color));
  case Matrix:
    return av_matrix_set_color(s->matrix, i, j, to_av_color(color));
  default:
    return api_error("set_color 只支持 Array / Matrix");
  }
}

int mark(State *s, int i, int j, const char *text) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0) return api_error("Array 下标不能为负数: %d", i);
    return av_array_mark(s->array, (size_t)i, text);
  case Matrix:
    return av_matrix_mark(s->matrix, i, j, text);
  default:
    return api_error("mark 只支持 Array / Matrix");
  }
}

int clear_mark(State *s, int i, int j) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");

  switch (s->type) {
  case Array:
    if (i < 0) return api_error("Array 下标不能为负数: %d", i);
    return av_array_clear_mark(s->array, (size_t)i);
  case Matrix:
    return av_matrix_clear_mark(s->matrix, i, j);
  default:
    return api_error("clear_mark 只支持 Array / Matrix");
  }
}

int array_swap(State *s, size_t i, size_t j) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != Array) return api_error("array_swap 只支持 Array");
  return av_array_swap(s->array, i, j);
}

static int ensure_graph_state(State *s, const char *func) {
  if (!s) return api_error("State 不能为空");
  if (s->type != Graph && s->type != List && s->type != Tree) {
    return api_error("%s 只支持 Graph / List / Tree", func);
  }
  return 1;
}

int graph_node(State *s, const char **values, Color color,
               char out_id[ID_MAX]) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node")) return 0;
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int graph_node_ints(State *s, const int *values, size_t count, Color color,
                    char out_id[ID_MAX]) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_ints")) return 0;
  return av_graph_add_node_ints(s->graph, values, count, to_av_color(color),
                                out_id);
}

int graph_edge(State *s, const char *from, const char *to,
               const char *label) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_edge")) return 0;
  return av_graph_add_edge(s->graph, from, to, label);
}

int graph_edge_int(State *s, const char *from, const char *to, int label) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_edge_int")) return 0;
  return av_graph_add_edge_int(s->graph, from, to, label);
}

int graph_node_color(State *s, const char *node_id, Color color) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_color")) return 0;
  return av_graph_set_node_color(s->graph, node_id, to_av_color(color));
}

int graph_node_values(State *s, const char *node_id, const char **values) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_values")) return 0;
  return av_graph_set_node_values(s->graph, node_id, values);
}

int graph_node_values_color(State *s, const char *node_id,
                            const char **values, Color color) {
  clear_api_error();

  if (!ensure_graph_state(s, "graph_node_values_color")) return 0;
  return av_graph_set_node_values_and_color(s->graph, node_id, values,
                                            to_av_color(color));
}

int list_node(State *s, const char *value, Color color, char out_id[ID_MAX]) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != List && s->type != Graph) {
    return api_error("list_node 只支持 List / Graph");
  }

  const char *values[] = {value ? value : ""};
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int list_next(State *s, const char *from, const char *to) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != List && s->type != Graph) {
    return api_error("list_next 只支持 List / Graph");
  }

  return av_graph_add_edge(s->graph, from, to, "next");
}

int tree_node(State *s, const char *value, Color color, char out_id[ID_MAX]) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_node 只支持 Tree / Graph");
  }

  const char *values[] = {value ? value : ""};
  return av_graph_add_node(s->graph, values, to_av_color(color), out_id);
}

int tree_child(State *s, const char *parent, const char *child_value,
               Color color, char child_id[ID_MAX]) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_child 只支持 Tree / Graph");
  }

  if (!tree_node(s, child_value, color, child_id)) return 0;
  return av_graph_add_edge(s->graph, parent, child_id, "");
}

int tree_edge(State *s, const char *parent, const char *child,
              const char *label) {
  clear_api_error();

  if (!s) return api_error("State 不能为空");
  if (s->type != Tree && s->type != Graph) {
    return api_error("tree_edge 只支持 Tree / Graph");
  }

  return av_graph_add_edge(s->graph, parent, child, label);
}
```

---

# 3. 新 CMakeLists.txt

你现在根目录有 `algvis.h / algvis.c / algvis_api.h / algvis_api.c`，示例还在 `examples/`，所以 CMake 写成这样：

```cmake
cmake_minimum_required(VERSION 3.12)

project(algvis_c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(algvis
    algvis.c
    algvis_api.c
)

target_include_directories(algvis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(api_array_sort
    examples/api_array_sort.c
)

target_link_libraries(api_array_sort
    PRIVATE
        algvis
)

add_executable(api_matrix_bfs
    examples/api_matrix_bfs.c
)

target_link_libraries(api_matrix_bfs
    PRIVATE
        algvis
)

add_executable(api_list
    examples/api_list.c
)

target_link_libraries(api_list
    PRIVATE
        algvis
)
```

---

# 4. examples/api_array_sort.c

```c
#include "algvis_api.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "error: %s\n", last_error());
}

static void clear_array(State *s, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    array_set_color(s, i, Default);
    array_clear_mark(s, i);
  }
}

int main(void) {
  int a[] = {5, 2, 9, 1, 6, 3};
  size_t n = sizeof(a) / sizeof(a[0]);

  TraceConfig trace = {
      .dir = "out_api_array",
      .title = "API 层：选择排序",
      .render_svg = 1,
      .remove_dot_after_svg = 0,
  };

  StateConfig cfg = {
      .type = Array,
      .trace = &trace,
      .as.array = {
          .length = n,
          .name = "SortArray",
      },
  };

  State *s = new_state(&cfg);
  if (!s) { die(); return 1; }

  for (size_t i = 0; i < n; ++i) array_set_int(s, i, a[i]);
  snapshot(s, "初始数组");

  for (size_t i = 0; i < n; ++i) {
    size_t min_i = i;

    clear_array(s, n);
    array_set_color(s, i, Orange);
    array_mark(s, i, "i/min");
    snapshot(s, "开始寻找最小值");

    for (size_t j = i + 1; j < n; ++j) {
      clear_array(s, n);
      array_set_color(s, i, Orange);
      array_set_color(s, min_i, Green);
      array_set_color(s, j, Red);
      array_mark(s, i, "i");
      array_mark(s, min_i, "min");
      array_mark(s, j, "j");
      snapshot(s, "比较 a[j] 和当前最小值");

      if (a[j] < a[min_i]) {
        min_i = j;
        clear_array(s, n);
        array_set_color(s, min_i, Green);
        array_mark(s, min_i, "new min");
        snapshot(s, "发现新的最小值");
      }
    }

    if (min_i != i) {
      int tmp = a[i];
      a[i] = a[min_i];
      a[min_i] = tmp;

      array_swap(s, i, min_i);
      clear_array(s, n);
      array_set_color(s, i, Blue);
      array_set_color(s, min_i, Blue);
      array_mark(s, i, "swap");
      array_mark(s, min_i, "swap");
      snapshot(s, "交换元素");
    }

    clear_array(s, n);
    for (size_t k = 0; k <= i; ++k) array_set_color(s, k, Gray);
    snapshot(s, "前缀已经有序");
  }

  if (!write_index(s)) die();
  free_state(s);

  printf("生成完成：out_api_array/index.html\n");
  return 0;
}
```

---

# 5. examples/api_matrix_bfs.c

```c
#include "algvis_api.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "error: %s\n", last_error());
}

int main(void) {
  const int R = 3, C = 4;
  int grid[3][4] = {
      {0, 0, 1, 0},
      {0, 0, 0, 0},
      {1, 0, 1, 0},
  };

  TraceConfig trace = {
      .dir = "out_api_matrix",
      .title = "API 层：网格 BFS",
      .render_svg = 1,
      .remove_dot_after_svg = 0,
  };

  StateConfig cfg = {
      .type = Matrix,
      .trace = &trace,
      .as.matrix = {
          .rows = R,
          .cols = C,
          .default_color = Default,
          .name = "Grid",
      },
  };

  State *s = new_state(&cfg);
  if (!s) { die(); return 1; }

  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (grid[r][c]) {
        matrix_set_text(s, r, c, "#");
        matrix_set_color(s, r, c, Gray);
      } else {
        matrix_set_text(s, r, c, ".");
      }
    }
  }

  matrix_set_text(s, 0, 0, "S");
  matrix_set_color(s, 0, 0, Orange);
  matrix_set_text(s, 2, 3, "T");
  matrix_set_color(s, 2, 3, Purple);
  snapshot(s, "初始化网格");

  matrix_mark(s, 0, 0, "cur");
  matrix_set_color(s, 0, 0, Red);
  snapshot(s, "当前位置出队，准备扩展");

  matrix_clear_mark(s, 0, 0);
  matrix_set_color(s, 1, 0, Green);
  matrix_mark(s, 1, 0, "push");
  snapshot(s, "访问下方格子，加入队列");

  if (!write_index(s)) die();
  free_state(s);

  printf("生成完成：out_api_matrix/index.html\n");
  return 0;
}
```

---

# 6. examples/api_list.c

```c
#include "algvis_api.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "error: %s\n", last_error());
}

int main(void) {
  TraceConfig trace = {
      .dir = "out_api_list",
      .title = "API 层：链表变化",
      .render_svg = 1,
      .remove_dot_after_svg = 0,
  };

  StateConfig cfg = {
      .type = List,
      .trace = &trace,
      .as.graph = {
          .name = "LinkedList",
      },
  };

  State *s = new_state(&cfg);
  if (!s) { die(); return 1; }

  char a[ID_MAX], b[ID_MAX], c[ID_MAX];

  list_node(s, "10", Orange, a);
  snapshot(s, "创建头结点 10");

  list_node(s, "20", Default, b);
  list_next(s, a, b);
  snapshot(s, "追加结点 20");

  list_node(s, "30", Default, c);
  list_next(s, b, c);
  snapshot(s, "追加结点 30");

  graph_node_color(s, b, Red);
  snapshot(s, "当前指针指向 20");

  graph_node_color(s, b, Gray);
  graph_node_color(s, c, Red);
  snapshot(s, "当前指针移动到 30");

  if (!write_index(s)) die();
  free_state(s);

  printf("生成完成：out_api_list/index.html\n");
  return 0;
}
```

---

# 7. 这个 API 的使用原则

## 最常用写法

```c
TraceConfig trace = {
    .dir = "out",
    .title = "算法过程",
    .render_svg = 1,
    .remove_dot_after_svg = 0,
};

StateConfig cfg = {
    .type = Array,
    .trace = &trace,
    .as.array = {.length = n, .name = "A"},
};

State *s = new_state(&cfg);
```

然后中途只做两件事：

```c
array_set_color(s, i, Red);
array_mark(s, i, "i");
snapshot(s, "比较当前位置");
```

最后：

```c
write_index(s);
free_state(s);
```

## 为什么没有完全所有函数都统一成一个名字？

C 语言没有函数重载。

如果强行把所有东西都统一成：

```c
set(s, a, b, c, d);
```

最后会变得很难记，因为你不知道 `a/b/c/d` 在 Array、Matrix、Graph 里分别是什么意思。

所以这里采用折中设计：

```text
通用生命周期：new_state / snapshot / write_index / free_state
Array/Matrix 常用操作：set_text / set_int / set_color / mark / clear_mark
Graph 操作：graph_node / graph_edge / graph_node_color
List 包装：list_node / list_next
Tree 包装：tree_node / tree_child / tree_edge
```

也就是说：

- 生命周期接口统一。
- Array / Matrix 的“格子类操作”统一。
- Graph / List / Tree 的“节点边操作”用对象名前缀区分。
- List / Tree 底层仍然是 Graph。

这样接口不会太多，也不会为了统一而变得难用。

