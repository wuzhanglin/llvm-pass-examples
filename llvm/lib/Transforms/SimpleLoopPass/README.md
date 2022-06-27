# A Simple LLVM Loop Pass (With New PassManager)

### Simple Loop Pass

1. Create a directory `llvm-project/llvm/lib/Transforms/SimpleLoopPass`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/SimpleLoopPass`
3. Add source file `SimpleLoopPass.cpp` into `llvm-project/llvm/lib/Transforms/SimpleLoopPass`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(SimpleLoopPass)
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
[4/4] Linking CXX shared module lib/SimpleLoopPass.dylib
```

### Run

Simple loop test:

```bash
mv ../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.c.txt ../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.c

clang -S -emit-llvm -O1 ../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.c -o ../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.ll

./bin/opt -load-pass-plugin=lib/SimpleLoopPass.dylib -passes="simple-loop-pass" -disable-output ../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.ll
```

Output:

```
Loop: for.body

    Sub-Loop Count: 0

    Basic Block Count: 1
        Basic Block: for.body


Loop: for.body5

    Sub-Loop Count: 0

    Basic Block Count: 1
        Basic Block: for.body5
```

Nested loop test:

```bash
mv ../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.c.txt ../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.c

clang -S -emit-llvm -O1 ../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.c -o ../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.ll

./bin/opt -load-pass-plugin=lib/SimpleLoopPass.dylib -passes="simple-loop-pass" -disable-output ../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.ll
```

Output:

```
Loop: for.body4

    Sub-Loop Count: 0

    Basic Block Count: 1
        Basic Block: for.body4


Loop: for.cond1.preheader

    Sub-Loop Count: 1
        Sub-Loop: for.body4

    Basic Block Count: 3
        Basic Block: for.cond1.preheader
        Basic Block: for.body4
        Basic Block: for.cond.cleanup3
```
