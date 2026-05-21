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
  Orange = AV_COLOR_ORANGE,
  Red = AV_COLOR_RED,
  Green = AV_COLOR_GREEN,
  Purple = AV_COLOR_PURPLE,
  Teal = AV_COLOR_TEAL,
  Gray = AV_COLOR_GRAY,
  Blue = AV_COLOR_BLUE
} Color;

/*
 * 统一类型。
 * List / Tree 底层都是 Graph。
 */
typedef enum VisType { Array, Matrix, Graph, List, Tree } VisType;

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
int graph_node(State *s, const char **values, Color color, char out_id[ID_MAX]);
int graph_node_ints(State *s, const int *values, size_t count, Color color,
                    char out_id[ID_MAX]);
int graph_edge(State *s, const char *from, const char *to, const char *label);
int graph_edge_int(State *s, const char *from, const char *to, int label);
int graph_node_color(State *s, const char *node_id, Color color);
int graph_node_values(State *s, const char *node_id, const char **values);
int graph_node_values_color(State *s, const char *node_id, const char **values,
                            Color color);

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
