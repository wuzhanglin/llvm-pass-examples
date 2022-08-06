# A Simple LLVM Transform Pass (With New PassManager)

### Simple Module Pass

1. Create a directory `llvm-project/llvm/lib/Transforms/ConstantAdditionCombiner`
2. Add `CMakeLists.txt` file `llvm-project/llvm/lib/Transforms/ConstantAdditionCombiner`
3. Add source file `ConstantAdditionCombiner.cpp` into `llvm-project/llvm/lib/Transforms/ConstantAdditionCombiner`

4. Add the following code into `llvm-project/llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(ConstantAdditionCombiner)
```

### Build

```bash
cd /path/to/llvm-project/llvm
mkdir -p build
cd build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..

ninja
```

Output:

```
[2/2] Linking CXX shared module lib/ConstantAdditionCombiner.dylib
```

### Run

#### Find the constant addition instructions

```bash
./bin/opt -load-pass-plugin=lib/ConstantAdditionCombiner.dylib -passes="constant-addition-printer" -disable-output ../lib/Transforms/ConstantAdditionCombiner/Test.ll
```

Output:

```
Function: Test
  %c = add i32 1, 2
  %d = add i32 5, 6
```

#### Combome the constant addition instructions

```
./bin/opt -load-pass-plugin=lib/ConstantAdditionCombiner.dylib -passes="constant-addition-combiner" -S ../lib/Transforms/ConstantAdditionCombiner/Test.ll
```

Output:

```
; ModuleID = '../lib/Transforms/ConstantAdditionCombiner/Test.ll'
source_filename = "Test.ll"

define i32 @Test(i32 %a, i32 %b) {
  %e = add i32 %a, %b
  %f = add i32 3, 11
  %g = add i32 %e, %f
  ret i32 %g
}
```
