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
  plan(8);

  ok(1+1 == 2, "1+1 == 2");

  skip(2, "tests not needed");

  not_ok(false, "not_ok expects a false condition");

  ok(1, "simply test truthiness of 1");

  isnt(1, 2, "1 is not the same as 2");

  pass("wunderbar! this test passed");

  fail("this test should fail");

  note("TAP internally keeps track of the number of tests it expects to find");

  done_testing();

  return 0;
}
