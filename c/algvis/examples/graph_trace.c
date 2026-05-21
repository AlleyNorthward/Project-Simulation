#include "algvis.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "algvis error: %s\n", av_last_error());
}

int main(void) {
  const char *infos[] = {"name", "dist", "visited"};
  int layers[] = {3};
  AvGraphConfig cfg;
  cfg.name = "DijkstraLike";
  cfg.directed = 1;
  cfg.infos = infos;
  cfg.info_count = 3;
  cfg.layers = layers;
  cfg.layer_count = 1;

  AvGraph *g = av_graph_new(&cfg);
  if (!g) {
    die();
    return 1;
  }
  av_graph_set_graph_attr(g, "rankdir", "LR");

  char A[AV_ID_MAX], B[AV_ID_MAX], C[AV_ID_MAX], D[AV_ID_MAX];
  const char *va[] = {"A", "0", "no"};
  const char *vb[] = {"B", "inf", "no"};
  const char *vc[] = {"C", "inf", "no"};
  const char *vd[] = {"D", "inf", "no"};

  av_graph_add_node(g, va, AV_COLOR_ORANGE, A);
  av_graph_add_node(g, vb, AV_COLOR_DEFAULT, B);
  av_graph_add_node(g, vc, AV_COLOR_DEFAULT, C);
  av_graph_add_node(g, vd, AV_COLOR_DEFAULT, D);
  av_graph_add_edge_int(g, A, B, 2);
  av_graph_add_edge_int(g, A, C, 5);
  av_graph_add_edge_int(g, B, C, 1);
  av_graph_add_edge_int(g, B, D, 4);
  av_graph_add_edge_int(g, C, D, 1);

  AvTraceConfig tc = {"out_graph", "图算法状态切片", 1, 0};
  AvTrace *tr = av_trace_new(&tc);
  if (!tr) {
    die();
    av_graph_free(g);
    return 1;
  }

  av_trace_add_graph(tr, g, "初始化：A 的距离是 0，其余点未知");

  const char *va2[] = {"A", "0", "yes"};
  const char *vb2[] = {"B", "2", "no"};
  const char *vc2[] = {"C", "5", "no"};
  av_graph_set_node_values_and_color(g, A, va2, AV_COLOR_GRAY);
  av_graph_set_node_values_and_color(g, B, vb2, AV_COLOR_GREEN);
  av_graph_set_node_values_and_color(g, C, vc2, AV_COLOR_GREEN);
  av_trace_add_graph(tr, g, "访问 A，松弛 A->B 和 A->C");

  const char *vb3[] = {"B", "2", "yes"};
  const char *vc3[] = {"C", "3", "no"};
  const char *vd3[] = {"D", "6", "no"};
  av_graph_set_node_values_and_color(g, B, vb3, AV_COLOR_GRAY);
  av_graph_set_node_values_and_color(g, C, vc3, AV_COLOR_GREEN);
  av_graph_set_node_values_and_color(g, D, vd3, AV_COLOR_GREEN);
  av_trace_add_graph(tr, g, "访问 B，C 被更新为 3，D 被更新为 6");

  const char *vc4[] = {"C", "3", "yes"};
  const char *vd4[] = {"D", "4", "no"};
  av_graph_set_node_values_and_color(g, C, vc4, AV_COLOR_GRAY);
  av_graph_set_node_values_and_color(g, D, vd4, AV_COLOR_GREEN);
  av_trace_add_graph(tr, g, "访问 C，D 被更新为 4");

  const char *vd5[] = {"D", "4", "yes"};
  av_graph_set_node_values_and_color(g, D, vd5, AV_COLOR_GRAY);
  av_trace_add_graph(tr, g, "访问 D，算法结束");

  av_trace_write_index(tr);
  av_trace_free(tr);
  av_graph_free(g);
  printf("生成完成：out_graph/index.html\n");
  return 0;
}
