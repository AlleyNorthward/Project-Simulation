#include "algvis_api.h"
#include <stdio.h>

static void die(void) { fprintf(stderr, "error: %s\n", last_error()); }

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
      .as.array =
          {
              .length = n,
              .name = "SortArray",
          },
  };

  State *s = new_state(&cfg);
  if (!s) {
    die();
    return 1;
  }

  for (size_t i = 0; i < n; ++i)
    array_set_int(s, i, a[i]);
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
    for (size_t k = 0; k <= i; ++k)
      array_set_color(s, k, Gray);
    snapshot(s, "前缀已经有序");
  }

  if (!write_index(s))
    die();
  free_state(s);

  printf("生成完成：out_api_array/index.html\n");
  return 0;
}
