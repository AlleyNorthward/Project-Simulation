#ifndef ALGVIS_DEBUG_H
#define ALGVIS_DEBUG_H

#include "algvis_api.h"

/*
 * 使用方式：
 *
 * 1. 开启可视化：
 *    编译时加 -DALGVIS_DEBUG
 *
 * 2. 关闭可视化：
 *    不定义 ALGVIS_DEBUG
 *
 * 关闭后：
 *    DBG_VIS_NEW        -> NULL
 *    DBG_VIS_SNAPSHOT   -> 1
 *    DBG_ARRAY_SET_INT  -> 1
 *    DBG_GRAPH_NODE     -> 1
 *
 * 也就是说，可视化相关代码不会真正执行。
 */

#ifdef ALGVIS_DEBUG

#define DBG_VIS_ENABLED 1

/* ===================== 生命周期 ===================== */

#define DBG_VIS_NEW(cfg) new_state((cfg))
#define DBG_VIS_FREE(s) free_state((s))
#define DBG_VIS_SNAPSHOT(s, caption) snapshot((s), (caption))
#define DBG_VIS_WRITE(s) write_index((s))
#define DBG_VIS_LAST_ERROR() last_error()

/* ===================== 通用 Array / Matrix 操作 ===================== */

#define DBG_SET_TEXT(s, i, j, text) set_text((s), (i), (j), (text))
#define DBG_SET_INT(s, i, j, value) set_int((s), (i), (j), (value))
#define DBG_SET_COLOR(s, i, j, color) set_color((s), (i), (j), (color))
#define DBG_MARK(s, i, j, text) mark((s), (i), (j), (text))
#define DBG_CLEAR_MARK(s, i, j) clear_mark((s), (i), (j))

/* ===================== Array ===================== */

#define DBG_ARRAY_SET_TEXT(s, i, text) array_set_text((s), (i), (text))
#define DBG_ARRAY_SET_INT(s, i, value) array_set_int((s), (i), (value))
#define DBG_ARRAY_SET_COLOR(s, i, color) array_set_color((s), (i), (color))
#define DBG_ARRAY_MARK(s, i, text) array_mark((s), (i), (text))
#define DBG_ARRAY_CLEAR_MARK(s, i) array_clear_mark((s), (i))
#define DBG_ARRAY_SWAP(s, i, j) array_swap((s), (i), (j))

/* ===================== Matrix ===================== */

#define DBG_MATRIX_SET_TEXT(s, r, c, text)                                     \
  matrix_set_text((s), (r), (c), (text))
#define DBG_MATRIX_SET_INT(s, r, c, value)                                     \
  matrix_set_int((s), (r), (c), (value))
#define DBG_MATRIX_SET_COLOR(s, r, c, color)                                   \
  matrix_set_color((s), (r), (c), (color))
#define DBG_MATRIX_MARK(s, r, c, text) matrix_mark((s), (r), (c), (text))
#define DBG_MATRIX_CLEAR_MARK(s, r, c) matrix_clear_mark((s), (r), (c))

/* ===================== Graph ===================== */

#define DBG_GRAPH_NODE(s, values, color, out_id)                               \
  graph_node((s), (values), (color), (out_id))

#define DBG_GRAPH_NODE_INTS(s, values, count, color, out_id)                   \
  graph_node_ints((s), (values), (count), (color), (out_id))

#define DBG_GRAPH_EDGE(s, from, to, label)                                     \
  graph_edge((s), (from), (to), (label))

#define DBG_GRAPH_EDGE_INT(s, from, to, label)                                 \
  graph_edge_int((s), (from), (to), (label))

#define DBG_GRAPH_NODE_COLOR(s, node_id, color)                                \
  graph_node_color((s), (node_id), (color))

#define DBG_GRAPH_NODE_VALUES(s, node_id, values)                              \
  graph_node_values((s), (node_id), (values))

#define DBG_GRAPH_NODE_VALUES_COLOR(s, node_id, values, color)                 \
  graph_node_values_color((s), (node_id), (values), (color))

/* ===================== List ===================== */

#define DBG_LIST_NODE(s, value, color, out_id)                                 \
  list_node((s), (value), (color), (out_id))

#define DBG_LIST_NEXT(s, from, to) list_next((s), (from), (to))

/* ===================== Tree ===================== */

#define DBG_TREE_NODE(s, value, color, out_id)                                 \
  tree_node((s), (value), (color), (out_id))

#define DBG_TREE_CHILD(s, parent, child_value, color, child_id)                \
  tree_child((s), (parent), (child_value), (color), (child_id))

#define DBG_TREE_EDGE(s, parent, child, label)                                 \
  tree_edge((s), (parent), (child), (label))

#else

#define DBG_VIS_ENABLED 0

/* ===================== 生命周期 ===================== */

#define DBG_VIS_NEW(cfg) ((State *)0)
#define DBG_VIS_FREE(s) ((void)0)
#define DBG_VIS_SNAPSHOT(s, caption) (1)
#define DBG_VIS_WRITE(s) (1)
#define DBG_VIS_LAST_ERROR() "AlgVis debug is disabled"

/* ===================== 通用 Array / Matrix 操作 ===================== */

#define DBG_SET_TEXT(s, i, j, text) (1)
#define DBG_SET_INT(s, i, j, value) (1)
#define DBG_SET_COLOR(s, i, j, color) (1)
#define DBG_MARK(s, i, j, text) (1)
#define DBG_CLEAR_MARK(s, i, j) (1)

/* ===================== Array ===================== */

#define DBG_ARRAY_SET_TEXT(s, i, text) (1)
#define DBG_ARRAY_SET_INT(s, i, value) (1)
#define DBG_ARRAY_SET_COLOR(s, i, color) (1)
#define DBG_ARRAY_MARK(s, i, text) (1)
#define DBG_ARRAY_CLEAR_MARK(s, i) (1)
#define DBG_ARRAY_SWAP(s, i, j) (1)

/* ===================== Matrix ===================== */

#define DBG_MATRIX_SET_TEXT(s, r, c, text) (1)
#define DBG_MATRIX_SET_INT(s, r, c, value) (1)
#define DBG_MATRIX_SET_COLOR(s, r, c, color) (1)
#define DBG_MATRIX_MARK(s, r, c, text) (1)
#define DBG_MATRIX_CLEAR_MARK(s, r, c) (1)

/* ===================== Graph ===================== */

#define DBG_GRAPH_NODE(s, values, color, out_id) (1)
#define DBG_GRAPH_NODE_INTS(s, values, count, color, out_id) (1)
#define DBG_GRAPH_EDGE(s, from, to, label) (1)
#define DBG_GRAPH_EDGE_INT(s, from, to, label) (1)
#define DBG_GRAPH_NODE_COLOR(s, node_id, color) (1)
#define DBG_GRAPH_NODE_VALUES(s, node_id, values) (1)
#define DBG_GRAPH_NODE_VALUES_COLOR(s, node_id, values, color) (1)

/* ===================== List ===================== */

#define DBG_LIST_NODE(s, value, color, out_id) (1)
#define DBG_LIST_NEXT(s, from, to) (1)

/* ===================== Tree ===================== */

#define DBG_TREE_NODE(s, value, color, out_id) (1)
#define DBG_TREE_CHILD(s, parent, child_value, color, child_id) (1)
#define DBG_TREE_EDGE(s, parent, child, label) (1)

#endif /* ALGVIS_DEBUG */

/* ===================== 配置初始化辅助宏 ===================== */

/*
 * 这些宏只是为了少写一点配置。
 * 不管 ALGVIS_DEBUG 是否开启，都可以用。
 */

#define DBG_TRACE_CONFIG(dir_, title_)                                         \
  {.dir = (dir_), .title = (title_), .render_svg = 1, .remove_dot_after_svg = 0}

#define DBG_ARRAY_STATE_CONFIG(trace_ptr_, length_, name_)                     \
  {                                                                            \
    .type = Array, .trace = (trace_ptr_), .as.array = {                        \
      .length = (length_),                                                     \
      .name = (name_)                                                          \
    }                                                                          \
  }

#define DBG_MATRIX_STATE_CONFIG(trace_ptr_, rows_, cols_, name_)               \
  {                                                                            \
    .type = Matrix, .trace = (trace_ptr_), .as.matrix = {                      \
      .rows = (rows_),                                                         \
      .cols = (cols_),                                                         \
      .default_color = Default,                                                \
      .name = (name_)                                                          \
    }                                                                          \
  }

#define DBG_GRAPH_STATE_CONFIG(trace_ptr_, name_, directed_)                   \
  {.type = Graph,                                                              \
   .trace = (trace_ptr_),                                                      \
   .as.graph = {.name = (name_),                                               \
                .directed = (directed_),                                       \
                .infos = NULL,                                                 \
                .info_count = 0,                                               \
                .layers = NULL,                                                \
                .layer_count = 0}}

#define DBG_LIST_STATE_CONFIG(trace_ptr_, name_)                               \
  {.type = List,                                                               \
   .trace = (trace_ptr_),                                                      \
   .as.graph = {.name = (name_),                                               \
                .directed = 1,                                                 \
                .infos = NULL,                                                 \
                .info_count = 0,                                               \
                .layers = NULL,                                                \
                .layer_count = 0}}

#define DBG_TREE_STATE_CONFIG(trace_ptr_, name_)                               \
  {.type = Tree,                                                               \
   .trace = (trace_ptr_),                                                      \
   .as.graph = {.name = (name_),                                               \
                .directed = 1,                                                 \
                .infos = NULL,                                                 \
                .info_count = 0,                                               \
                .layers = NULL,                                                \
                .layer_count = 0}}

#endif /* ALGVIS_DEBUG_H */
