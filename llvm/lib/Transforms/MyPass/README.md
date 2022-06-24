# Write an LLVM Pass With New PassManager

## My Pass

1. Create a directory `lib/Transforms/MyPass`
2. Add `CMakeLists.txt` file `lib/Transforms/MyPass`
3. Add source file `MyPass.cpp` into `lib/Transforms/MyPass`

4. Add the following code into `lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(MyPass)
```

5. Build

```bash
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```

6. Run

```bash
./bin/opt -load-pass-plugin=lib/MyPass.dylib -passes="my-pass" -disable-output test.ll
```

## Notes

Test code in `test.ll`:

```c
define i32 @foo() {
  %a = add i32 2, 3
  ret i32 %a
}

define void @bar() {
  ret void
}
```
