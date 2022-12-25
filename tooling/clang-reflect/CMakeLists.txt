set(LLVM_LINK_COMPONENTS Support)

add_clang_executable(clang-reflect
    ClangReflectMain.cpp
    ClassFinder.cpp
)

target_link_libraries(clang-reflect
    PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
)