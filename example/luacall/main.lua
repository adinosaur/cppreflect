local ret

-- call cpp class method: static int ServiceDemo::Func1(int x, int y, int z)
Cpp.ServiceDemo.Func1(1,2,3)

-- call cpp class method: static int ServiceDemo::Func2(std::vector<int> v)
Cpp.ServiceDemo.Func2({4,5,6})

-- call cpp class method: static int ServiceDemo::Func3(std::string s)
Cpp.ServiceDemo.Func3("test string param")

-- call cpp class method: static std::string ServiceDemo::Func4()
ret = Cpp.ServiceDemo.Func4()
print("ret:", ret)

-- call cpp class method: static std::vector<std::string> ServiceDemo::Func5()
ret = Cpp.ServiceDemo.Func5()
print("ret:", ret)
for k, v in ipairs(ret) do
    print(string.format("[%s]=%s", k, v))
end

-- call cpp class method: static int ServiceDemo::Func6(std::vector<std::string> v)
Cpp.ServiceDemo.Func6({"something", "is", "cool", "!!!!"})

-- call cpp class method: static void ServiceDemo::Func7()
Cpp.ServiceDemo.Func7()

-- call cpp class method: void ServiceDemo::Func8(Vector3d v3d)
Cpp.ServiceDemo.Func8({x=10,y=20,z=30})

-- call cpp class method: Vector3d ServiceDemo::Func9()
ret = Cpp.ServiceDemo.Func9()
print("ret:", ret)
for k, v in pairs(ret) do
    print(string.format("[\"%s\"]=%s", k, v))
end

-- call cpp class method: int Func10(std::map<int, int> m);
ret = Cpp.ServiceDemo.Func10({[10]=100, [20]=200, [30]=300})
print("ret:", ret)

-- call cpp class method: int Func11(std::map<int, int> m);
ret = Cpp.ServiceDemo.Func11({hehe=100, keke=200, dada=300})
print("ret:", ret)

-- call cpp class method: int Func12(std::unordered_map<int, int> m);
ret = Cpp.ServiceDemo.Func12({[10]=11100, [20]=22200, [30]=33300})
print("ret:", ret)

-- call cpp class method: int Func13(std::tuple<int, double, std::string> t);
ret = Cpp.ServiceDemo.Func13({10, 2333.33, "hehe2077"})
print("ret:", ret)

-- call cpp class method: std::tuple<int, double> Func14();
ret = Cpp.ServiceDemo.Func14()
print("ret:", ret)
for k, v in pairs(ret) do
    print(string.format("[\"%s\"]=%s", k, v))
end

-- local err = pcall(Cpp.ServiceDemo.Func3, {})
-- print(err)