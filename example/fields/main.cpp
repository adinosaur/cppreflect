#include "example/fields/my_struct.h"

int main()
{
    // fields get/set
    {
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
    }

    // fields loop
    {
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
    }
}