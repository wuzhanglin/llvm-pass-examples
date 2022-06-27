# CMakeLists.txt

add_llvm_library(SimpleLoopPass MODULE BUILDTREE_ONLY
    SimpleLoopPass.cpp

    PLUGIN_TOOL
    opt
)
