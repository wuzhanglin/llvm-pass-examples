# CMakeLists.txt

add_llvm_library(CompileTimeFunctionCallCounter MODULE BUILDTREE_ONLY
    CompileTimeFunctionCallCounter.cpp

    PLUGIN_TOOL
    opt
)
