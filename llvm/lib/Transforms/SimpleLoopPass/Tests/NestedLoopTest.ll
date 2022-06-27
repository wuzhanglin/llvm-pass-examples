; ModuleID = '../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.c'
source_filename = "../llvm/lib/Transforms/SimpleLoopPass/Tests/NestedLoopTest.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx11.0.0"

@a = dso_local local_unnamed_addr global [1024 x [1024 x i32]] zeroinitializer, align 16
@b = dso_local local_unnamed_addr global [1024 x [1024 x i32]] zeroinitializer, align 16

; Function Attrs: nofree norecurse nounwind ssp uwtable writeonly
define dso_local void @NestedLoopTest() local_unnamed_addr #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.cond.cleanup3
  %indvars.iv21 = phi i64 [ 0, %entry ], [ %indvars.iv.next22, %for.cond.cleanup3 ]
  %i.020 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup3 ]
  %sub = add nsw i32 %i.020, -3
  %add = add nuw nsw i32 %i.020, 3
  %mul = mul nsw i32 %sub, %add
  %0 = trunc i64 %indvars.iv21 to i32
  %rem = srem i32 %mul, %0
  br label %for.body4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup3
  ret void

for.cond.cleanup3:                                ; preds = %for.body4
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %inc8 = add nuw nsw i32 %i.020, 1
  %exitcond26.not = icmp eq i64 %indvars.iv.next22, 100
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond1.preheader, !llvm.loop !3

for.body4:                                        ; preds = %for.cond1.preheader, %for.body4
  %indvars.iv = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next, %for.body4 ]
  %arrayidx6 = getelementptr inbounds [1024 x [1024 x i32]], [1024 x [1024 x i32]]* @a, i64 0, i64 %indvars.iv21, i64 %indvars.iv
  store i32 %rem, i32* %arrayidx6, align 4, !tbaa !6
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 100
  br i1 %exitcond.not, label %for.cond.cleanup3, label %for.body4, !llvm.loop !10
}

attributes #0 = { nofree norecurse nounwind ssp uwtable writeonly "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 12.0.1 (https://github.com/llvm/llvm-project fed41342a82f5a3a9201819a82bf7a48313e296b)"}
!3 = distinct !{!3, !4, !5}
!4 = !{!"llvm.loop.mustprogress"}
!5 = !{!"llvm.loop.unroll.disable"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !4, !5}
