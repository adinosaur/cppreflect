from __main__ import ExeTarget
import glob

class EnumExample(ExeTarget):
    def __init__(self):
        super(EnumExample, self).__init__()
        self.name = 'example/enum/a.out'
        self.cxxflags = self.get_cxxflags()
        self.ldflags = self.get_ldflags()
        self.incs = self.get_includes()
        self.deps = [
            'cppreflect/libcppreflect.a',
        ]
        self.libs = [
            'cppreflect/libcppreflect.a',
            'thirdparty/msgpack-c/libmsgpackc.a',
            'thirdparty/lua-5.3.5/liblua.a',
            '-ldl',
        ]
        self.srcs = glob.glob('example/enum/*.cpp')

EnumExample()