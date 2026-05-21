#include "algvis.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "algvis error: %s\n", av_last_error());
}

int main(void) {
  const int R = 5, C = 7;
  int grid[5][7] = {
      {0, 0, 0, 1, 0, 0, 0}, {0, 1, 0, 1, 0, 1, 0}, {0, 1, 0, 0, 0, 1, 0},
      {0, 0, 0, 1, 0, 0, 0}, {1, 1, 0, 0, 0, 1, 0},
  };

  AvMatrix *m = av_matrix_new(R, C, AV_COLOR_DEFAULT, "GridBFS");
  if (!m) {
    die();
    return 1;
  }

  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (grid[r][c] == 1) {
        av_matrix_set(m, r, c, "#");
        av_matrix_set_color(m, r, c, AV_COLOR_GRAY);
      } else {
        av_matrix_set(m, r, c, ".");
      }
    }
  }

  AvTraceConfig tc = {"out_matrix", "网格 BFS 过程", 1, 0};
  AvTrace *tr = av_trace_new(&tc);
  if (!tr) {
    die();
    av_matrix_free(m);
    return 1;
  }

  int qr[128], qc[128], head = 0, tail = 0;
  int vis[5][7] = {0};
  int sr = 0, sc = 0;
  int gr = 4, gc = 6;

  qr[tail] = sr;
  qc[tail] = sc;
  tail++;
  vis[sr][sc] = 1;

  av_matrix_set(m, sr, sc, "S");
  av_matrix_set_color(m, sr, sc, AV_COLOR_ORANGE);
  av_matrix_set(m, gr, gc, "T");
  av_matrix_set_color(m, gr, gc, AV_COLOR_PURPLE);
  av_trace_add_matrix(tr, m, "初始化：S 是起点，T 是终点");

  int dr[4] = {-1, 1, 0, 0};
  int dc[4] = {0, 0, -1, 1};

  while (head < tail) {
    int r = qr[head], c = qc[head];
    head++;

    av_matrix_mark(m, r, c, "cur");
    av_matrix_set_color(m, r, c, AV_COLOR_RED);
    av_trace_add_matrix(tr, m, "弹出队头，准备扩展四个方向");
    av_matrix_clear_mark(m, r, c);

    if (r == gr && c == gc)
      break;

    for (int k = 0; k < 4; ++k) {
      int nr = r + dr[k], nc = c + dc[k];
      if (nr < 0 || nr >= R || nc < 0 || nc >= C)
        continue;
      if (grid[nr][nc] == 1 || vis[nr][nc])
        continue;

      vis[nr][nc] = 1;
      qr[tail] = nr;
      qc[tail] = nc;
      tail++;

      if (!(nr == gr && nc == gc))
        av_matrix_set(m, nr, nc, "v");
      av_matrix_set_color(m, nr, nc, AV_COLOR_GREEN);
      av_matrix_mark(m, nr, nc, "push");
      av_trace_add_matrix(tr, m, "访问新格子，并加入队列");
      av_matrix_clear_mark(m, nr, nc);
    }

    if (!(r == sr && c == sc))
      av_matrix_set_color(m, r, c, AV_COLOR_BLUE);
  }

  av_trace_write_index(tr);
  av_trace_free(tr);
  av_matrix_free(m);
  printf("生成完成：out_matrix/index.html\n");
  return 0;
}
