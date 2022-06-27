; ModuleID = '../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.c'
source_filename = "../llvm/lib/Transforms/SimpleLoopPass/Tests/SimpleLoopTest.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx11.0.0"

@array = dso_local local_unnamed_addr global [1024 x i32] zeroinitializer, align 16

; Function Attrs: nofree norecurse nounwind ssp uwtable
define dso_local void @SimpleLoopTest() local_unnamed_addr #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.body
  %indvars.iv24 = phi i64 [ 0, %entry ], [ %indvars.iv.next25, %for.body ]
  %arrayidx = getelementptr inbounds [1024 x i32], [1024 x i32]* @array, i64 0, i64 %indvars.iv24
  %0 = trunc i64 %indvars.iv24 to i32
  store i32 %0, i32* %arrayidx, align 4, !tbaa !3
  %indvars.iv.next25 = add nuw nsw i64 %indvars.iv24, 1
  %exitcond26.not = icmp eq i64 %indvars.iv.next25, 100
  br i1 %exitcond26.not, label %for.body5, label %for.body, !llvm.loop !7

for.cond.cleanup4:                                ; preds = %for.body5
  ret void

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body5 ], [ 101, %for.body ]
  %arrayidx7 = getelementptr inbounds [1024 x i32], [1024 x i32]* @array, i64 0, i64 %indvars.iv
  %1 = load i32, i32* %arrayidx7, align 4, !tbaa !3
  %2 = trunc i64 %indvars.iv to i32
  %mul = mul nsw i32 %1, %2
  store i32 %mul, i32* %arrayidx7, align 4, !tbaa !3
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 200
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !10
}

attributes #0 = { nofree norecurse nounwind ssp uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8, !9}
!8 = !{!"llvm.loop.mustprogress"}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !8, !9}
