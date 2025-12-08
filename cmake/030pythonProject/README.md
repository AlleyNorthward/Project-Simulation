pybind11, 似乎是必须要有的. python中下载的, 只有头文件, 导入的话, 似乎也没有太大的帮助. 所以问题是什么呢, 有些稍微地复杂了. 有点大..
```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release

cmake -G "Visual Studio 17 2022" -S . -B build
cmake --build build --config Release
~~~

