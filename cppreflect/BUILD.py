from __main__ import LibATarget
import glob

class LibCppReflect(LibATarget):
    def __init__(self):
        super(LibCppReflect, self).__init__()

        self.name = 'cppreflect/libcppreflect.a'
        self.cxxflags = self.get_cxxflags()
        self.incs = self.get_includes()
        self.srcs = glob.glob('cppreflect/*.cpp')

LibCppReflect()