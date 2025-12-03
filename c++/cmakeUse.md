# 基本编译
## ninji 编译方式.
- 首先, 这都是`mkdir`一个`build`文件夹, 然后, 进入, 输入一下两个命令行即可.
~~~shell
    cmake ..
    cmake --build .
~~~
- 此时, 我们在`CMakeLists.txt`中, 写好生成`.json`文件(用于方便自动补全的), 但是生成于`build`文件夹中, 需要提上上一级, 可以用下面命令行.
~~~shell
    move-item .\compile_commands.json ..\
~~~
- 如果, 动态添加了模块, 需要`cmake ..`, 重新整合一下项目(注意, 新生成的`.json`文件, 也需要重新添加)
- 另外, 如何清除呢? 直接删掉`build`, 重新弄一个`build`即可.

- 调试状态.
~~~shell
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
gdb MyCppApp.exe
~~~

- 另外, 还有, 就是在`$env:USERPROFILE`中, 有个`.gdbinit`文件, 据说是`gdb`调用时, 默认启动的. 注意, 终端管理员权限, 才能进入, 修改.

- 我写了个`bash`脚本. 加载命令如下.
~~~shell
. .\bash.ps1
~~~
- 前面的`. `表示在当前会话中加载. 要是直接运行`bash`, 似乎没有用.
