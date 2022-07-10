# LLVM Pass: Run-Time Function Call Counter (With New PassManager)

### Run-Time Function Call Counter

1. Create a directory `llvm-project/llvm/lib/Transforms/RunTimeFunctionCallCounter`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/RunTimeFunctionCallCounter`
3. Add source file `RunTimeFunctionCallCounter.cpp` into `llvm-project/llvm/lib/Transforms/RunTimeFunctionCallCounter`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(RunTimeFunctionCallCounter)
```

### Build

```bash
cd /path/to/llvm-project
mkdir -p build
cd build

# Format the source code
clang-format -i ../llvm/lib/Transforms/RunTimeFunctionCallCounter/*.cpp ../llvm/lib/Transforms/RunTimeFunctionCallCounter/**/*.c

# Set up C++ standard library and header path
export SDKROOT=$(xcrun --sdk macosx --show-sdk-path)

# Generate projects
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

# Compile
ninja
```

Output:

```
[4/4] Linking CXX shared module lib/RunTimeFunctionCallCounter.dylib
```

### Run

Commands:

```bash
mv ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.c.txt ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.c

clang -S -emit-llvm ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.c -o ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.ll

./bin/opt -debug -load-pass-plugin=lib/RunTimeFunctionCallCounter.dylib -passes="runtime-function-call-counter" ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.ll -o Test.bin

./bin/lli ./Test.bin
```

Output:

```
Args: ./bin/opt -debug -load-pass-plugin=lib/RunTimeFunctionCallCounter.dylib -passes=runtime-function-call-counter ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/Test.ll -o Test.bin 
Instrumented: Foo
Instrumented: Bar
Instrumented: Fez
Instrumented: main
Function: Foo, called 8 times
Function: Bar, called 2 times
Function: Fez, called 1 times
Function: main, called 1 times
```

### Inspect

Commands:

```
./bin/llvm-dis Test.bin -o ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Tests/TestWithCounter.ll
```
