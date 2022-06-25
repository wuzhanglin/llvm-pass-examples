# CMakeLists.txt

add_llvm_library(SimpleModulePass MODULE BUILDTREE_ONLY
    SimpleModulePass.cpp

    PLUGIN_TOOL
    opt
)
