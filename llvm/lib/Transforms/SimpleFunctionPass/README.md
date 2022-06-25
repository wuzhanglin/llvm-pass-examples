# A Simple LLVM Function Pass (With New PassManager)

### Simple Function Pass

1. Create a directory `llvm-project/llvm/lib/Transforms/SimpleFunctionPass`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/SimpleFunctionPass`
3. Add source file `SimpleFunctionPass.cpp` into `llvm-project/llvm/lib/Transforms/SimpleFunctionPass`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(SimpleFunctionPass)
```

### Build

```bash
cd /path/to/llvm-project
mkdir -p build
cd build

cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```

### Run

```bash
mv ../llvm/lib/Transforms/SimpleFunctionPass/Test.c.txt ../llvm/lib/Transforms/SimpleFunctionPass/Test.c
clang -S -emit-llvm ../llvm/lib/Transforms/SimpleFunctionPass/Test.c -o ../llvm/lib/Transforms/SimpleFunctionPass/Test.ll

./bin/opt -load-pass-plugin=lib/SimpleFunctionPass.dylib -passes="simple-function-pass" -disable-output ../llvm/lib/Transforms/SimpleFunctionPass/Test.ll
```

Output:

```bash
Function: Foo
    Instruction Count: 12

    Basic Block Count: 4
        BasicBlock: entry
        BasicBlock: if.then
        BasicBlock: if.else
        BasicBlock: if.end

    Argument Count: 1
        Argument: a

    Function Type: 
i32 (i32)

Function: Bar
    Instruction Count: 8

    Basic Block Count: 1
        BasicBlock: entry

    Argument Count: 2
        Argument: a
        Argument: b

    Function Type: 
i32 (i32, i32)

Function: Bead
    Instruction Count: 8

    Basic Block Count: 1
        BasicBlock: entry

    Argument Count: 2
        Argument: a
        Argument: b

    Function Type: 
i32 (i32, i32)
```
