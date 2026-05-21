#include "algvis_api.h"
#include <stdio.h>

static void die(void) { fprintf(stderr, "error: %s\n", last_error()); }

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
      .as.matrix =
          {
              .rows = R,
              .cols = C,
              .default_color = Default,
              .name = "Grid",
          },
  };

  State *s = new_state(&cfg);
  if (!s) {
    die();
    return 1;
  }

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

  if (!write_index(s))
    die();
  free_state(s);

  printf("生成完成：out_api_matrix/index.html\n");
  return 0;
}
