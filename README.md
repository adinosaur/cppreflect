# cppreflect

cppreflect是一个c++的运行时反射系统。原理类似QT的moc (Meta-Object Compiler)或是UE的UBT/UHT (UnrealBuildTool/UnrealHeaderTool)。与它们不同的是，cppreflect没有选择自己实现一个c++ parser，而是通过借助LLVM clang来编译头文件，并根据编译后的AST来生成反射元信息的c++代码。
此外在反射系统之上，cppreflect还提供了一个基于msgpack的c++ rpc系统、以及将lua binding等功能。

# Getting Started
## 1. 在头文件中对Struct/Class标注
**example/base/vector3d.h:**
```c++
#include "cppreflect/cppreflect.h"

struct STRUCT() Vector3D
{
    META_OBJECT

    PROPERTY()
    int x;

    PROPERTY()
    int y;

    PROPERTY()
    int z;
};
```

## 2. 在源文件中包含自动生成的代码（.generated.h）
**example/base/vector3d.cpp:**
```c++
#include "example/base/vector3d.h"
#include "example/base/vector3d.generated.h"
```

## 3. 运行clang-reflect编译头文件
```bash
thirdparty/llvm/bin/clang-reflect example/base/vector3d.h -p=. > example/base/vector3d.generated.h
```

## 4. GetClass()获取元对象
```c++
const cppreflect::Class* clazz = cppreflect::GetClass<Vector3D>();
for (auto& field : clazz->GetFields())
{
    const void* member = field->GetVoidPointer((void*)&v3d);
    const char* name = field->GetName().c_str();
    const char* value = field->GetType()->Dump(member).c_str();
    printf("v3d name:%s, value:%s\n", name, value);
}
```

更多功能参考 **example/** 目录的例子，以及 **doc/** 目录下的文档

#  Run example
`configure.py`是一个轻量级的c++构建系统，cppreflect使用它来编译example目录下的c++代码：
```bash
$ ./configure.py
$ ./example/base/a.out
```

# Clang-Reflect
`clang-reflect` 是基于clang的libtooling实现的代码生成工具，运行它的时候需要提供一个 `compile_commands.json` 文件，用于告诉 `clang-reflect` 编译的文件名以及编译参数等信息。使用 `configure.py` 构建系统时会自动生成该文件。

git仓库中提供了一个编译好的代码生成工具，在目录 `thirdpart/llvm/bin/clang-reflect` 下。如果你有需求（如定制生成的代码等），也可以选择自己编译。具体操作可以参考文档：[SetupLLVM](/doc/SetupLLVM.md),

# 参考资料
1. [把qt或ue4的反射系统裁剪出来是否可行？](https://mk.woa.com/q/256542)
2. [CppCon 2018: Arvid Gerstmann “Building a C++ Reflection System in One Weekend Using Clang and LLVM”](https://www.youtube.com/watch?v=DUiUBt-fqEY)