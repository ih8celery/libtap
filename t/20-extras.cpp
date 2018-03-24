/*
 * \file 20-extras.cpp
 *
 * \author Adam Marshall (ih8celery)
 *
 * \brief demonstrate test extras
 */

#define WANT_TEST_EXTRAS
#include "tap++.h"

using namespace TAP;

int main() {
  plan(5);

  note("tests are running in the outermost test group");

  TRY_NOT_OK(throw 1, "throws an exception");

  skip(2);
  ok(false, "false is ok");
  note("the condition of a skipped test is not evaluated");

  SUBTEST(1, "first subtest") DO
    pass("running a test in a subtest");
  END

  SUBTEST(4, "second subtest") DO
    ok(1, "test one");

    todo();
    isnt(1, 0, "1 is not 0");
    
    is_convertible<int, double>("int convertible to double");

    DO
      note("in nested anonymous subtest");
      not_ok(false, "false is actually not ok");
      ok(true, "true is ok");
    END
  END

  note("almost done testing");

  ok(true, "last-second test");

  done_testing();

  return exit_status();
}
