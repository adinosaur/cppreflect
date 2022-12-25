# Tutorial

## 标注类型
为了将对象（Class/Struct/Function/Property）暴露给反射系统，需要用标签来标注对象。反射系统内部会识别标签，并生成对应的元信息。

### 1. 标注结构体
```c++
struct STRUCT() MyStruct
{
    META_OBJECT
    ...
};
```

### 2. 标注类
```c++
class CLASS() MyClass
{
    META_OBJECT
    ...
};
```

### 3. 标注枚举
```c++
enum ENUM() MyEnum
{
    ONE = 1,
    TWO = 2,
    THREE = 3,
};
```

### 4. 标注属性（类的属性成员）
```c++
struct STRUCT() MyStruct
{
   META_OBJECT

   PROPERTY()
   std::string m_MyProperty;
};
```

### 5. 标注方法（类的函数成员）
```c++
struct STRUCT() MyStruct
{
   META_OBJECT

   PROPERTY()
   static void MyFunc();
};
```

## 运行时调用函数
反射系统的一个常见的使用场景就是在运行时能动态的调用函数，cppreflect支持下面几种调用函数的方式：

### 1. 调用静态函数
```c++
// 函数Func1的签名是：static void ServiceDemo::Func1();
const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func1");
if (f)
{
    cppreflect::AnyObject r;
    f->Invoke(NULL, {}, r);
}
```

### 2. 给函数传递参数

调用Invoke时可以通过params（`const std::vector<AnyObject>&`）指定函数的调用参数。
```c++
// 函数Func2的签名是：static void ServiceDemo::Func2(int x, unsigned int y, double z);
const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func2");
if (f)
{
    cppreflect::AnyObject x = cppreflect::MakeAny<int>(-1);
    cppreflect::AnyObject y = cppreflect::MakeAny<unsigned int>(20);
    cppreflect::AnyObject z = cppreflect::MakeAny<double>(3.333);
    cppreflect::AnyObject r;
    f->Invoke(NULL, {x, y, z}, r);
}
```

### 3. std数据结构作为函数参数

函数参数还支持传递std::vector/std::string/std::map/std::unordered_map/std::tuple。
```c++
// 函数Func3的签名是：static void ServiceDemo::Func3(std::vector<int> v);
const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func3");
if (f)
{
    cppreflect::AnyObject v = cppreflect::MakeAny<std::vector<int> >({1,2,3,9});
    cppreflect::AnyObject r;
    f->Invoke(NULL, {v,}, r);
}
```

### 4. 结构体/类作为函数参数

参数的类型也可以是自定义的类或结构体，前提是需要该类或结构体也暴露给反射系统
```c++
// 函数Func4的签名是：static void ServiceDemo::Func4(Vector3D v3d);
const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func4");
if (f)
{
    Vector3D v3d;
    v3d.x = 991;
    v3d.y = 992;
    v3d.z = 993;
    cppreflect::AnyObject r;
    f->Invoke(NULL, {cppreflect::MakeAny<Vector3D>(v3d),}, r);
}
```

## 访问属性（property）

### 1. 属性读写
```c++
MyStruct mystruct;
mystruct.x = 400;
mystruct.y = 500;
mystruct.z = 600;

const cppreflect::Class* clazz = cppreflect::GetClass<MyStruct>();
const cppreflect::Field* field = clazz->GetFieldByName("x");
int x = field->GetAs<int>(&mystruct);
printf("x=%d\n", x); // output: x=400

field->SetValue<int>(&mystruct, 401);
printf("x=%d\n", mystruct.x); // output: x=401
```

### 2. 遍历所有属性
```c++
MyStruct mystruct;
mystruct.x = 400;
mystruct.y = 500;
mystruct.z = 600;

const cppreflect::Class* clazz = cppreflect::GetClass<MyStruct>();
for (auto& field : clazz->GetFields())
{
    const void* member = field->GetVoidPointer((void*)&mystruct);
    const char* name = field->GetName().c_str();
    const char* value = field->GetType()->Dump(member).c_str();
    printf("name:%s, value:%s\n", name, value);
}

// output:
// name:x, value:400
// name:y, value:500
// name:z, value:600
```