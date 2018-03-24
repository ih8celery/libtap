/*
 * \file 10-basic.cpp
 *
 * \author Adam Marshall (ih8celery)
 *
 * \brief test more of the basic features of libtap++. these tests<br>
 * do not cover any of the symbols defined when WANT_TEST_EXTRAS is<br> 
 * defined
 */

#include "tap++.h"

using namespace TAP;

int main() {
  plan(10);

  ok(1+1 == 2, "1+1 == 2");

  skip(2, "tests not needed");

  not_ok(false, "not_ok expects a false condition");

  ok(1, "simply test truthiness of 1");

  todo("develop better test of isnt");
  isnt(1, 2, "1 is not the same as 2");

  is(1.0, 1.0, "1.0 is 1.0");

  is(1.0, 0.5, "1.0 is close to 0.5", 1.0);

  pass("wunderbar! this test passed");

  fail("this test should fail");

  not_ok(summary(), "all tests passed, except one");

  not_ok(get_finished_testing(), "tests not yet finished");

  done_testing();

  return exit_status() - 1;
}
