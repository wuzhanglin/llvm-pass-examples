# CMakeLists.txt

add_llvm_library(RunTimeFunctionCallCounter MODULE BUILDTREE_ONLY
    RunTimeFunctionCallCounter.cpp

    PLUGIN_TOOL
    opt
)
