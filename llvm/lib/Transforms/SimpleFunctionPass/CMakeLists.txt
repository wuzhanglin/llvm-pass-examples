# CMakeLists.txt

add_llvm_library(SimpleFunctionPass MODULE BUILDTREE_ONLY
    SimpleFunctionPass.cpp

    PLUGIN_TOOL
    opt
)
