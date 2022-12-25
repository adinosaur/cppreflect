# Setup LLVM

如果有定制生成代码的需求可以修改 **tooling/clang-reflect** 目录下的代码，不过在此之前需要先在机器上安装好LLVM。

# Download LLVM
参考LLVM文档：[Getting the Source Code and Build LLVM](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm)

这里需要注意的是，用GCC编译最新的LLVM需要GCC的版本 **>=5.1.0** ，我的开发机GCC版本是 **4.8.5** ，因此我用了老一点版本：[LLVM 7.0.0](https://releases.llvm.org/7.0.0/docs/GettingStarted.html)


1. Checkout
```
git clone https://github.com/llvm/llvm-project.git
```

2. 切到分支 LLVM 7.x
```
cd llvm-project
git checkout remotes/origin/release/7.x
```

3. 将 **tooling/clang-refletc** 拷贝到 **clang-tools-extra** 目录下
```
cd clang-tools-extra
ln -s ../../tooling/clang-reflect .
```

4. 修改 **clang-tools-extra** 目录下的 **CMakeList.txt** 文件
```
echo "add_subdirectory(clang-reflect)" >> CMakeLists.txt
```

# Building LLVM
1. 创建 **build** 目录
```
cd /path/to/llvm-project/
mkdir build
```

2. 使用cmake生成ninja构建脚本
```
cd build
cmake -DLLVM_ENABLE_PROJECTS=clang -G Ninja ../llvm
```

3. 编译 **clang-reflect**
```
ninja clang-reflect
```

4. 将编译好的 **clang-reflect** 拷贝到 **thirdpart** 目录下
```
install bin/clang-reflect ../../thirdparty/llvm/bin/
cp -r lib/clang/7.1.0/ ../../thirdparty/llvm/lib/clang/
```

# RAQ
1. 如果编译的时候遇到 'stddef.h' 文件找不到的编译报错，确认一下反射工具 **clang-reflect** 的 `../lib/clang/7.1.0/include` 相对路径下是否有clang的相关头文件。

> Clang tools need their builtin headers and search for them the same way Clang does. Thus, the default location to look for builtin headers is in a path $(dirname /path/to/tool)/../lib/clang/3.3/include relative to the tool binary. This works out-of-the-box for tools running from llvm’s toplevel binary directory after building clang-resource-headers, or if the tool is running from the binary directory of a clang install next to the clang binary.

> Tips: if your tool fails to find stddef.h or similar headers, call the tool with -v and look at the search paths it looks through.

参考文档：https://clang.llvm.org/docs/LibTooling.html