# Lua Binding

cppreflect提供一个简单的lua binding的功能。反射系统会将`ScriptCallable`标注的静态方法自动导出给lua。通过函数的参数以及返回值，lua和c++可以进行数据交换。这里需要说明的是，目前的数据交换全部是以值传递的方式进行。如c++的函数返回一个std::map给lua时，会在lua中构造一个新的table。

## 初始化LuaMgr

使用lua binding功能时，需要如下所示初始化LuaMgr模块：
```c++
// 初始化LuaCppModule
LuaMgr::GetInstance()->SetLuaState(L);
LuaMgr::GetInstance()->InitCppModule();
```

初始化完LuaMgr后，所有暴露给lua的静态方法会放在`_G.Cpp`表中

## Lua调用C++函数

```lua
-- call cpp class method: static int ServiceDemo::Func1(int x, int y, int z)
Cpp.ServiceDemo.Func1(1,2,3)
```

c++函数的参数可以是一个自定义的类对象，但是需要暴露给反射系统：
```lua
-- call cpp class method: void ServiceDemo::Func8(Vector3d v3d)
Cpp.ServiceDemo.Func8({x=10,y=20,z=30})
```

cppreflect已经为一些经常使用的std的数据结构实现了lua binding，包括了`std::vector`、`std::map`、`std::unordered_map`、`std::tuple`:
```lua

-- call cpp class method: static int ServiceDemo::Func6(std::vector<std::string> v)
Cpp.ServiceDemo.Func6({"something", "is", "cool", "!!!!"})

-- call cpp class method: int Func10(std::map<int, int> m);
ret = Cpp.ServiceDemo.Func10({[10]=100, [20]=200, [30]=300})
print("ret:", ret)

-- call cpp class method: int Func12(std::unordered_map<int, int> m);
ret = Cpp.ServiceDemo.Func12({[10]=11100, [20]=22200, [30]=33300})
print("ret:", ret)

-- call cpp class method: int Func13(std::tuple<int, double, std::string> t);
ret = Cpp.ServiceDemo.Func13({10, 2333.33, "hehe2077"})
print("ret:", ret)
```

更多用法请参考 **example/luacall/main.lua**