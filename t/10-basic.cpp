/*
 * 10-basic.cpp
 *
 * test more of the basic features of libtap++. these tests do not
 * cover any of the symbols defined when WANT_TEST_EXTRAS is 
 * defined
 */

#include "tap++.h"

using namespace TAP;

int main() {
  plan(7);

  ok(1+1 == 2, "1+1 == 2");

  skip(2, "tests not needed");

  not_ok(false, "not_ok expects a false condition");

  ok(1, "simply test truthiness of 1");

  todo("develop better test of isnt");
  isnt(1, 2, "1 is not the same as 2");

  is(1.0, 1.0, "1.0 is 1.0");

  pass("wunderbar! this test passed");

  fail("this test should fail");

  done_testing();

  return exit_status();
}
