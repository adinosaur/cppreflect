// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "ClassFinder.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory ClangReflectCategory("clang-reflect options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv)
{
    // CommonOptionsParser constructor will parse arguments and create a
    // CompilationDatabase.  In case of error it will terminate the program.
    CommonOptionsParser OptionsParser(argc, argv, ClangReflectCategory);

    // Use OptionsParser.getCompilations() and OptionsParser.getSourcePathList()
    // to retrieve CompilationDatabase and the list of input file paths.
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    /* The classFinder class is invoked as callback. */
    ClassFinder classFinder;


    classFinder.SetSouceFileName(OptionsParser.getSourcePathList().front());

    MatchFinder finder;

    /* Search for all records (class/struct) with an 'annotate' attribute. */
    static const DeclarationMatcher classMatcher = cxxRecordDecl(decl().bind("id"), hasAttr(attr::Annotate));
    finder.addMatcher(classMatcher, &classFinder);

    /* Search for all fields with an 'annotate' attribute. */
    static const DeclarationMatcher propertyMatcher = fieldDecl(decl().bind("id"), hasAttr(attr::Annotate));
    finder.addMatcher(propertyMatcher, &classFinder);

    /* Search for all functions with an 'annotate' attribute. */
    static const DeclarationMatcher functionMatcher = functionDecl(decl().bind("id"), hasAttr(attr::Annotate));
    finder.addMatcher(functionMatcher, &classFinder);

    /* Search for all enums with an 'annotate' attribute. */
    static const DeclarationMatcher enumMacher = enumDecl(decl().bind("id"), hasAttr(attr::Annotate));
    finder.addMatcher(enumMacher, &classFinder);

    // The ClangTool needs a new FrontendAction for each translation unit we run
    // on.  Thus, it takes a FrontendActionFactory as parameter.  To create a
    // FrontendActionFactory from a given FrontendAction type, we call
    // newFrontendActionFactory<clang::SyntaxOnlyAction>().
    int Result = Tool.run(newFrontendActionFactory(&finder).get());

    return Result;
}

