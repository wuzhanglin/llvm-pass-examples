; RUN: opt -S -passes=loop-opt-tutorial -debug-only=loop-opt-tutorial < %s 2>&1 | FileCheck %s
; REQUIRES: asserts
;
; Opts: -correlated-propagation -mem2reg -instcombine -loop-simplify -indvars -instnamer
;
; Source:
;   int A[1024][1024];
;   int B[1024][1024];
;
;   #define EXPENSIVE_PURE_COMPUTATION(i) ((i - 3) * (i + 3) % i)
;
;   void dep_free() {
;     for (int i = 0; i < 100; i++)
;       for (int j = 0; j < 100; j++)
;         A[i][j] = EXPENSIVE_PURE_COMPUTATION(i);
;   }
;
@A = common dso_local global [1024 x [1024 x i32]] zeroinitializer, align 4
@B = common dso_local global [1024 x [1024 x i32]] zeroinitializer, align 4

; CHECK: Entering LoopOptTutorialPass::run
; CHECK: Loop at depth 2
; CHECK: Entering LoopOptTutorialPass::run
; CHECK: Loop at depth 1
; CHECK: Loop at depth 2
define dso_local void @dep_free() {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc7, %entry
  %indvars.iv1 = phi i64 [ %indvars.iv.next2, %for.inc7 ], [ 0, %entry ]
  %i.0 = phi i32 [ 0, %entry ], [ %inc8, %for.inc7 ]
  %exitcond5 = icmp ne i64 %indvars.iv1, 100
  br i1 %exitcond5, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  br label %for.end9

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.inc ], [ 0, %for.body ]
  %exitcond = icmp ne i64 %indvars.iv, 100
  br i1 %exitcond, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup3:                                ; preds = %for.cond1
  br label %for.end

for.body4:                                        ; preds = %for.cond1
  %sub = add nsw i32 %i.0, -3
  %tmp = add nuw nsw i64 %indvars.iv1, 3
  %tmp6 = trunc i64 %tmp to i32
  %mul = mul nsw i32 %sub, %tmp6
  %tmp7 = trunc i64 %indvars.iv1 to i32
  %rem = srem i32 %mul, %tmp7
  %arrayidx6 = getelementptr inbounds [1024 x [1024 x i32]], [1024 x [1024 x i32]]* @A, i64 0, i64 %indvars.iv1, i64 %indvars.iv
  store i32 %rem, i32* %arrayidx6, align 4, !tbaa !2
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond.cleanup3
  br label %for.inc7

for.inc7:                                         ; preds = %for.end
  %indvars.iv.next2 = add nuw nsw i64 %indvars.iv1, 1
  %inc8 = add nuw nsw i32 %i.0, 1
  br label %for.cond

for.end9:                                         ; preds = %for.cond.cleanup
  ret void
}

declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture)

declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture)


!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0 (https://github.com/kitbarton/llvm-project.git 8ec1040ded5afb786efc933363420571aa6ec5ea)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}