/*
 * 20-extras.cpp
 *
 * demonstrate test extras
 */

#define WANT_TEST_EXTRAS
#include "tap++.h"

using namespace TAP;

int main() {
  plan(5);

  TRY_NOT_OK(throw 1, "throws an exception");

  skip(2);
  ok(false, "false is ok");
  note("the condition of a skipped test is not evaluated");

  SUBTEST(1, "anonymous") DO
    pass("running a test in an anonymous subtest");
  END

  SUBTEST(3, "second subtest") DO
    ok(1, "test one");
    todo();
    isnt(1, 0, "1 is not 0");
    is_convertible<int, double>("int convertible to double");
  END

  note("almost done testing");

  ok(true, "last second test");

  done_testing();

  return exit_status();
}
