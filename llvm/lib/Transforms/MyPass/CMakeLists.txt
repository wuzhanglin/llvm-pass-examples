# CMakeLists.txt

add_llvm_library(MyPass MODULE BUILDTREE_ONLY
    MyPass.cpp

    PLUGIN_TOOL
    opt
)
