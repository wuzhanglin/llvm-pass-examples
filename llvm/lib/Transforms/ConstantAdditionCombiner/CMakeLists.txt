# CMakeLists.txt

add_llvm_library(ConstantAdditionCombiner MODULE BUILDTREE_ONLY
    ConstantAdditionCombiner.cpp

    PLUGIN_TOOL
    opt
)
