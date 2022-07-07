# LLVM Pass Examples

Some simple LLVM Pass examples:

```c
llvm-project/llvm
├── ...
├── lib
│   └── Transforms
│       │── 1.MyPass
│       │── 2.SimpleModulePass
│       └── 3.SimpleFunctionPass
│       └── 4.SimpleLoopPass
│       └── 5.CompileTimeFunctionCallCounter
│── ...
```

具体编译及使用步骤，请参考：

1. [LLVM系列第十八章：写一个简单的IR处理流程Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125383746)
2. [LLVM系列第十九章：写一个简单的Module Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125466341)
3. [LLVM系列第二十章：写一个简单的Function Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125438188)
4. [LLVM系列第二十一章：写一个简单的Loop Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125494996)
5. [LLVM系列第二十二章：写一个简单的编译时函数调用统计器Pass](https://blog.csdn.net/Zhanglin_Wu/article/details/125670416)
