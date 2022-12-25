from __main__ import ExeTarget
import glob

class RpcDemo(ExeTarget):
    def __init__(self):
        super(RpcDemo, self).__init__()
        self.name = 'example/rpc/a.out'
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
        ]
        self.srcs = glob.glob('example/rpc/*.cpp')

RpcDemo()