#include "algvis_api.h"
#include <stdio.h>

static void die(void) { fprintf(stderr, "error: %s\n", last_error()); }

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
      .as.graph =
          {
              .name = "LinkedList",
          },
  };

  State *s = new_state(&cfg);
  if (!s) {
    die();
    return 1;
  }

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

  if (!write_index(s))
    die();
  free_state(s);

  printf("生成完成：out_api_list/index.html\n");
  return 0;
}
