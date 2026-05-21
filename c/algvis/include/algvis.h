#ifndef ALGVIS_H
#define ALGVIS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AV_ID_MAX 32

typedef enum AvColor {
  AV_COLOR_DEFAULT = 0,
  AV_COLOR_ORANGE,
  AV_COLOR_RED,
  AV_COLOR_GREEN,
  AV_COLOR_PURPLE,
  AV_COLOR_TEAL,
  AV_COLOR_GRAY,
  AV_COLOR_BLUE
} AvColor;

const char *av_last_error(void);
void av_free(void *p);

/* 直接把 dot 文件渲染成 svg。要求系统中 graphviz 的 dot 命令可用。 */
int av_render_dot(const char *dot_path, const char *svg_path, int remove_dot);

/* ========================= Graph ========================= */

typedef struct AvGraph AvGraph;

typedef struct AvGraphConfig {
  const char *name; /* Graphviz 图名，默认 G */
  int directed;     /* 0: 无向图, 非 0: 有向图 */

  /*
   * 节点字段配置。
   * 例如 infos = {"i", "j", "min", "a[i]", "a[j]"}
   * layers = {3, 2}
   * 则节点 label 类似：{step1|{i:0|j:1|min:2}|{a[i]:4|a[j]:9}}
   *
   * 如果不需要结构化字段，可设置 info_count = 0。
   */
  const char **infos;
  size_t info_count;
  const int *layers;
  size_t layer_count;
} AvGraphConfig;

AvGraph *av_graph_new(const AvGraphConfig *cfg);
void av_graph_free(AvGraph *g);

/* 高级配置项：直接设置 Graphviz 属性。 */
int av_graph_set_graph_attr(AvGraph *g, const char *key, const char *value);
int av_graph_set_node_attr(AvGraph *g, const char *key, const char *value);
int av_graph_set_edge_attr(AvGraph *g, const char *key, const char *value);

/*
 * 添加节点。
 * values 数量必须等于 cfg.info_count。
 * 自动生成 node1/node2/...，写入 out_id。
 */
int av_graph_add_node(AvGraph *g, const char **values, AvColor color,
                      char out_id[AV_ID_MAX]);
int av_graph_add_node_ints(AvGraph *g, const int *values, size_t count,
                           AvColor color, char out_id[AV_ID_MAX]);

int av_graph_set_node_values(AvGraph *g, const char *node_id,
                             const char **values);
int av_graph_set_node_color(AvGraph *g, const char *node_id, AvColor color);
int av_graph_set_node_values_and_color(AvGraph *g, const char *node_id,
                                       const char **values, AvColor color);

int av_graph_add_edge(AvGraph *g, const char *from, const char *to,
                      const char *label);
int av_graph_add_edge_int(AvGraph *g, const char *from, const char *to,
                          int label);
int av_graph_pop_edge(AvGraph *g);

char *av_graph_to_dot(const AvGraph *g);
int av_graph_write_dot(const AvGraph *g, const char *path);
int av_graph_export_svg(const AvGraph *g, const char *dot_path,
                        const char *svg_path, int remove_dot);

/* ========================= Matrix ========================= */

typedef struct AvMatrix AvMatrix;

AvMatrix *av_matrix_new(int rows, int cols, AvColor default_color,
                        const char *name);
void av_matrix_free(AvMatrix *m);

int av_matrix_set_table_attr(AvMatrix *m, const char *key, const char *value);
int av_matrix_set(AvMatrix *m, int row, int col, const char *text);
int av_matrix_set_int(AvMatrix *m, int row, int col, int value);
int av_matrix_set_color(AvMatrix *m, int row, int col, AvColor color);
int av_matrix_mark(AvMatrix *m, int row, int col, const char *marker);
int av_matrix_clear_mark(AvMatrix *m, int row, int col);

char *av_matrix_to_dot(const AvMatrix *m);
int av_matrix_write_dot(const AvMatrix *m, const char *path);
int av_matrix_export_svg(const AvMatrix *m, const char *dot_path,
                         const char *svg_path, int remove_dot);

/* ========================= Array ========================= */

typedef struct AvArray AvArray;

AvArray *av_array_new(size_t length, const char *name);
void av_array_free(AvArray *a);

int av_array_set_table_attr(AvArray *a, const char *key, const char *value);
int av_array_set_text(AvArray *a, size_t index, const char *text);
int av_array_set_int(AvArray *a, size_t index, int value);
int av_array_set_color(AvArray *a, size_t index, AvColor color);
int av_array_mark(AvArray *a, size_t index, const char *marker);
int av_array_clear_mark(AvArray *a, size_t index);
int av_array_swap(AvArray *a, size_t i, size_t j);

char *av_array_to_dot(const AvArray *a);
int av_array_write_dot(const AvArray *a, const char *path);
int av_array_export_svg(const AvArray *a, const char *dot_path,
                        const char *svg_path, int remove_dot);

/* ========================= Trace ========================= */

typedef struct AvTrace AvTrace;

typedef struct AvTraceConfig {
  const char *dir;          /* 输出目录，例如 "out_array" */
  const char *title;        /* index.html 标题 */
  int render_svg;           /* 是否自动调用 dot 生成 svg */
  int remove_dot_after_svg; /* 成功生成 svg 后是否删除 dot */
} AvTraceConfig;

AvTrace *av_trace_new(const AvTraceConfig *cfg);
void av_trace_free(AvTrace *t);

int av_trace_add_graph(AvTrace *t, const AvGraph *g, const char *caption);
int av_trace_add_matrix(AvTrace *t, const AvMatrix *m, const char *caption);
int av_trace_add_array(AvTrace *t, const AvArray *a, const char *caption);

/* 所有 step 添加完后调用，生成 index.html。 */
int av_trace_write_index(const AvTrace *t);

#ifdef __cplusplus
}
#endif

#endif /* ALGVIS_H */
