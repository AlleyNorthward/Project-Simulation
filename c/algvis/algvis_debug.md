~~~C
#include "algvis_debug.h"

int main(void) {
  int a[] = {5, 2, 9, 1};
  int n = 4;

  TraceConfig trace = DBG_TRACE_CONFIG("out", "数组调试");

  StateConfig cfg = DBG_ARRAY_STATE_CONFIG(&trace, n, "ArrayDebug");

  State *vis = DBG_VIS_NEW(&cfg);

  for (int i = 0; i < n; ++i) {
    DBG_ARRAY_SET_INT(vis, i, a[i]);
  }

  DBG_VIS_SNAPSHOT(vis, "初始化数组");

  DBG_ARRAY_SET_COLOR(vis, 1, Red);
  DBG_ARRAY_MARK(vis, 1, "i");
  DBG_VIS_SNAPSHOT(vis, "标记下标 i");

  DBG_VIS_WRITE(vis);
  DBG_VIS_FREE(vis);

  return 0;
}
~~~

~~~powershell
cmake -S . -B build -DALGVIS_DEBUG=ON
~~~









