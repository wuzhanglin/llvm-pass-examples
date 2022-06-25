# A Simple LLVM Module Pass (With New PassManager)

### Simple Module Pass

1. Create a directory `llvm-project/llvm/lib/Transforms/SimpleModulePass`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/SimpleModulePass`
3. Add source file `SimpleModulePass.cpp` into `llvm-project/llvm/lib/Transforms/SimpleModulePass`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(SimpleModulePass)
```

### Build

```bash
cd /path/to/llvm-project
mkdir -p build
cd build

cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```

Output:

```
[4/4] Linking CXX shared module lib/SimpleModulePass.dylib
```

### Run

```bash
mv ../llvm/lib/Transforms/SimpleModulePass/Test.c.txt ../llvm/lib/Transforms/SimpleModulePass/Test.c
clang -S -emit-llvm ../llvm/lib/Transforms/SimpleModulePass/Test.c -o ../llvm/lib/Transforms/SimpleModulePass/Test.ll

./bin/opt -load-pass-plugin=lib/SimpleModulePass.dylib -passes="simple-module-pass" -disable-output ../llvm/lib/Transforms/SimpleModulePass/Test.ll
```

Output:

```
Module: ../llvm/lib/Transforms/SimpleModulePass/Test.ll
    ID: ../llvm/lib/Transforms/SimpleModulePass/Test.ll
    Source File Name: ../llvm/lib/Transforms/SimpleModulePass/Test.c
    Instruction Count: 28

    Gloabl Variable Count: 4
        Global Variable: globalInt
        Global Variable: globalShort
        Global Variable: .str
        Global Variable: globalString

    Function Count: 3
        Function: Foo
        Function: Bar
        Function: Bead
```
