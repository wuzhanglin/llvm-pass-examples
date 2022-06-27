# LLVM Pass Examples

Some simple LLVM Pass examples:

```c
llvm-project/llvm
├── ...
├── lib
│   └── Transforms
│       │── MyPass
│       │── SimpleModulePass
│       └── SimpleFunctionPass
│       └── SimpleLoopPass
│── ...
```

具体编译及使用步骤，请参考：

- [LLVM系列第十八章：写一个简单的IR处理流程Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125383746)
- [LLVM系列第十九章：写一个简单的Module Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125466341)
- [LLVM系列第二十章：写一个简单的Function Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125438188)
- [LLVM系列第二十一章：写一个简单的Loop Pass](https://blog.csdn.net/zhanglin_wu/category_11835780.html)
