; ModuleID = 'Test.ll'
source_filename = "Test.ll"

define i32 @Test(i32 %a, i32 %b) {
  %c = add i32 1, 2
  %d = add i32 5, 6
  %e = add i32 %a, %b
  %f = add i32 %c, %d
  %g = add i32 %e, %f
  ret i32 %g
}
