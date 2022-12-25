#include "example/base/vector3d.h"
#include "example/base/service_demo.h"

#include <stdio.h>
#include <tuple>

int main()
{
    Vector3D v3d;
    v3d.x = 400;
    v3d.y = 500;
    v3d.z = 600;
    v3d.array = {5,6,7,8};
    v3d.name = "heheda";


    std::tuple<int, double> t{1, 3.14};
    printf("tuple: %s\n", cppreflect::GetType<std::tuple<int, double>>()->Dump(&t).c_str());

    // 通过反射将v3d的成员打印至屏幕
    {
        const cppreflect::Class* clazz = cppreflect::GetClass<Vector3D>();
        for (auto& field : clazz->GetFields())
        {
            const void* member = field->GetVoidPointer((void*)&v3d);
            std::string name = field->GetName();
            std::string value = field->GetType()->Dump(member);
            printf("v3d name:%s, value:%s\n", name.c_str(), value.c_str());
        }
    }

    ServiceDemo service_demo;

    {
        // static void Func1();
        const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func1");
        if (f)
        {
            cppreflect::AnyObject r;
            f->Invoke(NULL, {}, r);
        }
    }

    {
        // static void Func2(int x, unsigned int y, double z);
        const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func2");
        if (f)
        {
            cppreflect::AnyObject x = cppreflect::MakeAny<int>(-1);
            cppreflect::AnyObject y = cppreflect::MakeAny<unsigned int>(20);
            cppreflect::AnyObject z = cppreflect::MakeAny<double>(3.333);
            cppreflect::AnyObject r;
            f->Invoke(NULL, {x, y, z}, r);
        }
    }

    {
        // static void Func3(std::vector<int> v);
        const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func3");
        if (f)
        {
            cppreflect::AnyObject v = cppreflect::MakeAny<std::vector<int> >({1,2,3,9});
            cppreflect::AnyObject r;
            f->Invoke(NULL, {v,}, r);
        }
    }

    {
        // static void Func4(Vector3D v3d);
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
    }

    {
        // std::string Func5(std::string s);
        const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func5");
        if (f)
        {
            cppreflect::AnyObject s = cppreflect::MakeAny<std::string>("test string param");
            cppreflect::AnyObject r;
            int err = f->Invoke(NULL, {s,}, r);
            if (err == 0)
            {
                if (r.GetType() == cppreflect::GetType<std::string>())
                {
                    printf("ret: %s\n", r.Get<std::string>().c_str());
                }
            }
        }
    }

    {
        // std::string Func6(std::map<int, std::string> m);
        const cppreflect::Function* f = cppreflect::GetClass<ServiceDemo>()->GetFunctionByName("Func6");
        if (f)
        {
            cppreflect::AnyObject m = cppreflect::MakeAny<std::map<int, std::string>>({{2,"kkk"}, {5, "sss"}});
            cppreflect::AnyObject r;
            int err = f->Invoke(&service_demo, {m,}, r);
            if (err == 0)
            {
                if (r.GetType() == cppreflect::GetType<std::string>())
                {
                    printf("ret: %s\n", r.Get<std::string>().c_str());
                }
            }
        }
    }

    return 0;
}