# LLVM Pass: Compile Time Function Call Counter (With New PassManager)

### Compile Time Function Call Counter

1. Create a directory `llvm-project/llvm/lib/Transforms/CompileTimeFunctionCallCounter`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/CompileTimeFunctionCallCounter`
3. Add source file `CompileTimeFunctionCallCounter.cpp` into `llvm-project/llvm/lib/Transforms/CompileTimeFunctionCallCounter`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(CompileTimeFunctionCallCounter)
```

### Build

```bash
cd /path/to/llvm-project
mkdir -p build
cd build

# Format the source code
clang-format -i ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/*.cpp ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/**/*.c

# Set up C++ standard library and header path
export SDKROOT=$(xcrun --sdk macosx --show-sdk-path)

# Generate projects
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

# Compile
ninja
```

Output:

```
[4/4] Linking CXX shared module lib/CompileTimeFunctionCallCounter.dylib
```

### Run

Commands:

```bash
cp ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.c.txt ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.c

clang -S -emit-llvm ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.c -o ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.ll

./bin/opt -load-pass-plugin=lib/CompileTimeFunctionCallCounter.dylib -passes="compile-time-function-call-counter" -disable-output ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.ll
```

Output:

```
Function: Foo, called 3 times
Function: Bar, called 2 times
Function: Fez, called 1 times
```
