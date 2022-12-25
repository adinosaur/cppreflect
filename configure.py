#!/usr/bin/env python
# -*- coding=utf-8 -*-

import os
import sys
import re
import time
import glob
import argparse
import subprocess
import imp
import json


# 执行进程的命令行参数
Args = None

# 所有构建目标
Targets = []

cc = 'gcc'
cxx = 'g++'
ld = 'g++'
ar = 'ar -r'
rm = 'rm -rf'
protoc = 'thirdparty/protobuf/protoc-3.5.1'
clang_reflect = 'thirdparty/llvm/bin/clang-reflect'
clang_reflect_args = ''

# =========
# 头文件包含路径
# =========
def _get_includes():
    return [
        '.',
        # lua
        'thirdparty/lua-5.3.5/include',
        # msgpack
        'thirdparty/msgpack-c/include',
    ]


# =========
# 编译参数
# =========
def _get_cxxflags():
    cxxflags = [
        '-std=c++11',
        '-g',
        '-m64',
        '-fPIC',
        '-pipe', 
        '-fwrapv',
        '-fno-strict-aliasing',
        '-Wstrict-aliasing=2',
        '-DMSGPACK_ENDIAN_LITTLE_BYTE', # https://github.com/msgpack/msgpack-c/issues/734
    ]

    if Args.warning:
        cxxflags += ['-Wall', '-Wextra']

    if Args.release:
        cxxflags += ['-O2']
    else:
        cxxflags += ['-D_DEV_DEBUG']
    
    if Args.cov:
        cxxflags += ['-D_CODE_COV', '-fprofile-arcs', '-ftest-coverage']
    
    return cxxflags


# =========
# 链接参数
# =========
def _get_ldflags():
    return [
        '-rdynamic',
        '-fprofile-arcs',
    ]


# =========
# Makefile构建目标
# =========
class MakefileTarget(object):
    def __init__(self):
        Targets.append(self)
        self.name = None
    
    def get_name(self):
        return self.name

    def generate_build_rule(self):
        pass

    def generate_clean_rule(self):
        target = self.name
        rules = []
        rules.append('# clean target: {target}'.format(target=target))
        rules.append('\t{rm} {target}'.format(rm=rm, target=target))
        return rules

    def generate_compilation_db(self):
        return []


# =========
# C++构建目标
# =========
class CXXTarget(MakefileTarget):
    def __init__(self):
        super(CXXTarget, self).__init__()
        
        self.cxxflags = []      # 编译参数
        self.incs = []          # 头文件搜索路径
        self.srcs = []          # 源文件列表
        self.objs = []          # object文件列表
        self.hdrs = []          # 头文件列表（目前没有使用）
        
        self.generated = None
    
    def get_includes(self):
        return _get_includes()
    
    def get_cxxflags(self):
        return _get_cxxflags()
    
    def get_generated_files(self):
        if self.generated is None:
            self.generated = []
            for src in self.srcs:
                if src.endswith('.cpp'):
                    with open(src, 'r') as f:
                        # 检查.cpp源文件中有无#include "xxx.gnerated.h"这种代码
                        for line in f.readlines():
                            generated_file = src.replace('.cpp', '.generated.h')
                            if '#include \"%s\"' % generated_file in line:
                                self.generated.append(generated_file)
                                break
        return self.generated
    
    def generate_build_rule(self):
        target = self.name
        cxxflags = ' '.join(self.cxxflags)
        incs = ' '.join(['-I' + p for p in self.incs])
        objs = ' '.join([src.replace('.cpp', '.o').replace('.cc', '.o') for src in self.srcs])

        rules = []
        rules.append('# === build cxx target: {target} ==='.format(target=target))
        for src in self.srcs:
            obj = src.replace('.cpp', '.o').replace('.cc', '.o')
            dep = src.replace('.cpp', '.d').replace('.cc', '.d')
            rules.append('# ====== {src} ====='.format(src=src))

            # 关于GCC预处理option: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
            # If you use the -E option, nothing is done except preprocessing. 
            # Some of these options make sense only together with -E 
            # because they cause the preprocessor output to be unsuitable for actual compilation.
            rules.append('{dep}: {src}\n\t{cxx} -E -MM -MG -MT \'{obj}\' -MF {dep} -c {src} {cxxflags} {incs}\n'.format(cxx=cxx, obj=obj, src=src, cxxflags=cxxflags, incs=incs, dep=dep))
            rules.append('include {dep}\n'.format(dep=dep))
            rules.append('{obj}: {src}\n\t{cxx} -o {obj} -c {src} {cxxflags} {incs}\n'.format(cxx=cxx, obj=obj, src=src, cxxflags=cxxflags, incs=incs, dep=dep))
        
        # .generated.h文件生成规则
        generateds = self.get_generated_files()
        for generated in generateds:
            hdr = generated.replace('.generated.h', '.h')
            rules.append('# ====== {generated} ====='.format(generated=generated))
            rules.append('{generated}: {tool} {hdr}\n\t{tool} {args} {hdr} -p=. > {generated}\n'.format(tool=clang_reflect, args=clang_reflect_args, hdr=hdr, generated=generated))

        return rules
    
    def generate_clean_rule(self):
        target = self.name
        objs = ' '.join([src.replace('.cpp', '.o').replace('.cc', '.o') for src in self.srcs])
        deps = ' '.join([src.replace('.cpp', '.d').replace('.cc', '.d')for src in self.srcs])
        gcnos = ' '.join([src.replace('.cpp', '.gcno').replace('.cc', '.gcno')for src in self.srcs])
        gcdas = ' '.join([src.replace('.cpp', '.gcda').replace('.cc', '.gcda')for src in self.srcs])

        rules = []
        rules.append('# clean target: {target}'.format(target=target))
        rules.append('\t{rm} {target}'.format(rm=rm, target=target))
        rules.append('\t{rm} {objs}'.format(rm=rm, objs=objs))
        rules.append('\t{rm} {deps}'.format(rm=rm, deps=deps))
        rules.append('\t{rm} {gcnos}'.format(rm=rm, gcnos=gcnos))
        rules.append('\t{rm} {gcdas}'.format(rm=rm, gcdas=gcdas))

        generated_files = self.get_generated_files()
        if generated_files:
            rules.append('\t{rm} {generateds}'.format(rm=rm, generateds=' '.join(generated_files)))
        return rules
    
    def generate_compilation_db(self):
        cxxflags = ' '.join(self.cxxflags + ['-D__CPPREFLECT__',])
        incs = ' '.join(['-I' + p for p in self.incs])

        commands = []
        for src in self.srcs:
            obj = src.replace('.cpp', '.o').replace('.cc', '.o')
            command = {
                'directory': os.getcwd(),
                'file': src,
                'command': '{cxx} -c {src} -o {obj} {cxxflags} {incs}'.format(cxx=cxx, obj=obj, src=src, cxxflags=cxxflags, incs=incs)
            }
            commands.append(command)
        
        generateds = self.get_generated_files()
        for generated in generateds:
            hdr = generated.replace('.generated.h', '.h')
            command = {
                'directory': os.getcwd(),
                'file': hdr,
                'command': '{cxx} -x c++-header {hdr} -o {hdr}.gch {cxxflags} {incs}'.format(cxx=cxx, hdr=hdr, cxxflags=cxxflags, incs=incs)
            }
            commands.append(command)
        return commands


# =========
# libx.a构建目标
# =========
class LibATarget(CXXTarget):
    def __init__(self):
        super(LibATarget, self).__init__()

        self.deps = []          # 依赖的文件
    
    def generate_build_rule(self):
        target = self.name
        deps = ' '.join(self.deps)
        objs = ' '.join([src.replace('.cpp', '.o').replace('.cc', '.o') for src in self.srcs])

        rules = super(LibATarget, self).generate_build_rule()
        rules.append('{target}: {objs} {deps}\n\t{ar} {target} {objs}\n'.format(ar=ar, target=target, objs=objs, deps=deps))
        return rules


# =========
# libx.so构建目标
# =========
class LibSOTarget(CXXTarget):
    def __init__(self):
        super(LibSOTarget, self).__init__()
        
        self.deps = []          # 依赖的文件
        self.ldflags = []       # 链接参数
        self.libs = []          # 链接的库文件
    
    def get_ldflags(self):
        return _get_ldflags()
    
    def generate_build_rule(self):
        target = self.name
        ldflags = ' '.join(self.ldflags)
        deps = ' '.join(self.deps)
        libs = ' '.join(self.libs)
        objs = ' '.join([src.replace('.cpp', '.o').replace('.cc', '.o') for src in self.srcs])

        rules = super(LibSOTarget, self).generate_build_rule()
        rules.append('{target}: {objs} {deps}\n\t{ld} -o {target} {objs} {libs} {ldflags} -shared\n'.format(ld=ld, target=target, objs=objs, deps=deps, libs=libs, ldflags=ldflags))
        return rules


# =========
# 可执行程序
# =========
class ExeTarget(CXXTarget):
    def __init__(self):
        super(ExeTarget, self).__init__()
        
        self.deps = []          # 依赖的文件
        self.ldflags = []       # 链接参数
        self.libs = []          # 链接的库文件
    
    def get_ldflags(self):
        return _get_ldflags()
    
    def generate_build_rule(self):
        target = self.name
        ldflags = ' '.join(self.ldflags)
        deps = ' '.join(self.deps)
        libs = ' '.join(self.libs)
        objs = ' '.join([src.replace('.cpp', '.o').replace('.cc', '.o') for src in self.srcs])

        rules = super(ExeTarget, self).generate_build_rule()
        rules.append('{target}: {objs} {deps}\n\t{ld} -o {target} {objs} {libs} {ldflags}\n'.format(ld=ld, target=target, objs=objs, deps=deps, libs=libs, ldflags=ldflags))
        return rules


# =========
# Protobuf目标
# =========
class ProtobufTarget(MakefileTarget):
    def __init__(self):
        super(ProtobufTarget, self).__init__()
        
        self.imports = []       # import目录
        self.proto = None       # .proto文件
        self.cpp_out = None     # 生成的c++文件目录
    
    def generate_build_rule(self):
        target = self.name
        imports = ':'.join(self.imports)

        rules = []
        rules.append('# build protobuf target: {target}'.format(target=target))
        rules.append('{target}: {proto}'.format(target=target, proto=self.proto))
        rules.append('\t{protoc} -I{imports} --cpp_out={cpp_out} {proto}\n'.format(protoc=protoc, imports=imports, cpp_out=self.cpp_out, proto=self.proto))
        return rules


class ProtobufDescriptorTarget(MakefileTarget):
    def __init__(self):
        super(ProtobufDescriptorTarget, self).__init__()
        self.imports = []       # import目录
        self.protos = None       # .proto文件
    
    def generate_build_rule(self):
        target = self.name
        imports = ':'.join(self.imports)
        protos = ' '.join(self.protos)

        rules = []
        rules.append('# build protobuf descriptor target: {target}'.format(target=target))
        rules.append('{target}: {protos}'.format(target=target, protos=protos))
        rules.append('\t{protoc} -I{imports} -o {target} {protos}\n'.format(protoc=protoc, imports=imports, target=target, protos=protos))
        return rules

class ProtobufLibATarget(LibATarget):
    def __init__(self):
        super(ProtobufLibATarget, self).__init__()
        self.protos = []
        self.imports = []
    
    def generate_build_rule(self):
        target = self.name
        rules = []
        rules.append('# === build protobuf lib.a: {target} ==='.format(target=target))
        for proto in self.protos:
            src = proto.replace('.proto', '.pb.cc')
            hdr = proto.replace('.proto', '.pb.h')
            cpp_out = os.path.split(proto)[0]
            imports = ':'.join(self.imports)
            rules.append('# ====== build protobuf source: {proto} ====='.format(proto=proto))
            rules.append('{src} {hdr} : {proto}\n\t{protoc} -I{imports} --cpp_out={cpp_out} {proto}\n'.format(src=src, hdr=hdr, cpp_out=cpp_out, imports=imports, proto=proto, protoc=protoc))
        
        self.srcs = [proto.replace('.proto', '.pb.cc') for proto in self.protos]
        self.deps = self.srcs
        self.incs = self.imports
        self.cxxflags = self.get_cxxflags()
        rules.extend(super(ProtobufLibATarget, self).generate_build_rule())
        return rules
    
    def generate_clean_rule(self):
        target = self.name
        srcs = ' '.join([proto.replace('.proto', '.pb.cc') for proto in self.protos])
        hdrs = ' '.join([proto.replace('.proto', '.pb.h') for proto in self.protos])

        rules = []
        rules.extend(super(ProtobufLibATarget, self).generate_clean_rule())
        rules.append('\t{rm} {srcs}'.format(rm=rm, srcs=srcs))
        rules.append('\t{rm} {hdrs}'.format(rm=rm, hdrs=hdrs))
        return rules

# =========
# 加载目标
# =========
def load_targets():
    target_cnt = 0
    for path, dir_list, file_list in os.walk(os.getcwd()):
        for file_name in file_list:
            if file_name == 'BUILD.py':
                target_cnt += 1
                imp.load_source('__yggd_target%s' % target_cnt, os.path.join(path, file_name))


# =========
# 生成Makefile
# =========
def generate_makefile_rule(targets):

    rules = []
    rules.append('# THIS FILE IS AUTO-GENERATED BY configure.py, DO NOT EDIT!\n')
    rules.append('.PHONY: all clean')
    rules.append('')

    rules.append('all: %s' % ' '.join([target.get_name() for target in targets]))
    rules.append('\t@echo "[1;32mBuild success ...[0m"\n')

    for target in targets:
        rules.extend(target.generate_build_rule())
        rules.append('')
    rules.append('')

    rules.append('clean:')
    for target in targets:
        rules.extend(target.generate_clean_rule())
        rules.append('')
    rules.append('')

    return rules


# =========
# 生成compile_commands.json
# =========
def generate_compilation_db(targets):
    commands = []
    for target in targets:
        commands.extend(target.generate_compilation_db())
    return commands


# =========
# Main
# =========
def main():
    parser = argparse.ArgumentParser(description='Clash server engine configure file.')
    parser.add_argument('--debug', action='store_true', help='debug build.')
    parser.add_argument('--release', action='store_true', help='release build.')
    parser.add_argument('--warning', action='store_true', help='show gcc warning.')
    parser.add_argument('-j', '--jobs', type=int, default=16, help='Allow N jobs at once; infinite jobs with no arg.')
    parser.add_argument('--stop-after', choices=['generate-makefile', 'generate-commands', 'generate'])
    parser.add_argument('--cov', action='store_true', help='gen code coverage')
    parser.add_argument('--rebuild', action='store_true', help='rebuild all target')
    
    args = parser.parse_args()

    global Args
    Args = args

    # 加载target
    load_targets()
    
    # 为target生成makefile
    rules = generate_makefile_rule(Targets)
    with open('Makefile', 'w') as f:
        f.write('\n'.join(rules))
    print('\033[1;34mGenerate: makefile...\033[0m')

    if args.stop_after == 'generate-makefile':
        return 0

    # 为target生成compile_commands.json
    commands = generate_compilation_db(Targets)
    with open('compile_commands.json', 'w') as f:
        f.write(json.dumps(commands, indent=4))
    print('\033[1;34mGenerate: compile_commands.json ...\033[0m')
    
    if args.stop_after in ['generate-commands', 'generate']:
        return 0

    # configure之后自动运行make
    if args.rebuild:
        p = subprocess.Popen('make clean', shell=True)
        p.communicate()
    
    cmd = 'make -j%s' % Args.jobs
    print(cmd)
    p = subprocess.Popen(cmd, shell=True)
    p.communicate()
    return p.returncode


if __name__ == '__main__':
    sys.exit(main())