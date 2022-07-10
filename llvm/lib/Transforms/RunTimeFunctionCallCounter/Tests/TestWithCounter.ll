; ModuleID = 'Test.bin'
source_filename = "../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Tests/Test.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx11.0.0"

@counter_Foo = common global i32 0, align 4
@name_Foo = private unnamed_addr constant [4 x i8] c"Foo\00", align 1
@counter_Bar = common global i32 0, align 4
@name_Bar = private unnamed_addr constant [4 x i8] c"Bar\00", align 1
@counter_Fez = common global i32 0, align 4
@name_Fez = private unnamed_addr constant [4 x i8] c"Fez\00", align 1
@counter_main = common global i32 0, align 4
@name_main = private unnamed_addr constant [5 x i8] c"main\00", align 1
@0 = global [31 x i8] c"Function: %s, called %d times\0A\00"
@llvm.global_dtors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 0, void ()* @PrintfWrapper, i8* null }]

; Function Attrs: noinline nounwind optnone ssp uwtable
define dso_local void @Foo() #0 {
entry:
  %0 = load i32, i32* @counter_Foo, align 4
  %1 = add i32 1, %0
  store i32 %1, i32* @counter_Foo, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define dso_local void @Bar() #0 {
entry:
  %0 = load i32, i32* @counter_Bar, align 4
  %1 = add i32 1, %0
  store i32 %1, i32* @counter_Bar, align 4
  call void @Foo()
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define dso_local void @Fez() #0 {
entry:
  %0 = load i32, i32* @counter_Fez, align 4
  %1 = add i32 1, %0
  store i32 %1, i32* @counter_Fez, align 4
  call void @Bar()
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define dso_local i32 @main() #0 {
entry:
  %0 = load i32, i32* @counter_main, align 4
  %1 = add i32 1, %0
  store i32 %1, i32* @counter_main, align 4
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @Foo()
  call void @Bar()
  call void @Fez()
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %2, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  call void @Foo()
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %3 = load i32, i32* %i, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !3

for.end:                                          ; preds = %for.cond
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @PrintfWrapper() {
enter:
  %0 = load i32, i32* @counter_Foo, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @name_Foo, i32 0, i32 0), i32 %0)
  %2 = load i32, i32* @counter_Bar, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @name_Bar, i32 0, i32 0), i32 %2)
  %4 = load i32, i32* @counter_Fez, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @name_Fez, i32 0, i32 0), i32 %4)
  %6 = load i32, i32* @counter_main, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @name_main, i32 0, i32 0), i32 %6)
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.mustprogress"}
