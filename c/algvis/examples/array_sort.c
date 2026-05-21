#include "algvis.h"
#include <stdio.h>

static void die(void) {
  fprintf(stderr, "algvis error: %s\n", av_last_error());
}

static void clear_colors(AvArray *a, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    av_array_set_color(a, i, AV_COLOR_DEFAULT);
    av_array_clear_mark(a, i);
  }
}

int main(void) {
  int data[] = {5, 2, 9, 1, 6, 3};
  size_t n = sizeof(data) / sizeof(data[0]);

  AvArray *arr = av_array_new(n, "ArraySort");
  if (!arr) {
    die();
    return 1;
  }

  for (size_t i = 0; i < n; ++i)
    av_array_set_int(arr, i, data[i]);

  AvTraceConfig tc = {"out_array", "选择排序过程", 1, 0};
  AvTrace *tr = av_trace_new(&tc);
  if (!tr) {
    die();
    av_array_free(arr);
    return 1;
  }

  av_trace_add_array(tr, arr, "初始数组");

  for (size_t i = 0; i < n; ++i) {
    size_t min_i = i;
    clear_colors(arr, n);
    av_array_set_color(arr, i, AV_COLOR_ORANGE);
    av_array_mark(arr, i, "i/min");
    av_trace_add_array(tr, arr, "开始寻找当前位置的最小值");

    for (size_t j = i + 1; j < n; ++j) {
      clear_colors(arr, n);
      av_array_set_color(arr, i, AV_COLOR_ORANGE);
      av_array_set_color(arr, min_i, AV_COLOR_GREEN);
      av_array_set_color(arr, j, AV_COLOR_RED);
      av_array_mark(arr, i, "i");
      av_array_mark(arr, min_i, "min");
      av_array_mark(arr, j, "j");
      av_trace_add_array(tr, arr, "比较 a[j] 和当前最小值");

      if (data[j] < data[min_i]) {
        min_i = j;
        clear_colors(arr, n);
        av_array_set_color(arr, i, AV_COLOR_ORANGE);
        av_array_set_color(arr, min_i, AV_COLOR_GREEN);
        av_array_mark(arr, i, "i");
        av_array_mark(arr, min_i, "new min");
        av_trace_add_array(tr, arr, "发现新的最小值");
      }
    }

    if (min_i != i) {
      int tmp = data[i];
      data[i] = data[min_i];
      data[min_i] = tmp;
      av_array_swap(arr, i, min_i);
      clear_colors(arr, n);
      av_array_set_color(arr, i, AV_COLOR_BLUE);
      av_array_set_color(arr, min_i, AV_COLOR_BLUE);
      av_array_mark(arr, i, "swap");
      av_array_mark(arr, min_i, "swap");
      av_trace_add_array(tr, arr, "交换 a[i] 和最小值");
    }

    clear_colors(arr, n);
    for (size_t k = 0; k <= i; ++k)
      av_array_set_color(arr, k, AV_COLOR_GRAY);
    av_trace_add_array(tr, arr, "当前位置已经有序");
  }

  if (!av_trace_write_index(tr))
    die();

  av_trace_free(tr);
  av_array_free(arr);
  printf("生成完成：out_array/index.html\n");
  return 0;
}
