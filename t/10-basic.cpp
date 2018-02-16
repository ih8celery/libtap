/*
 * 10-basic.cpp
 *
 * test more of the basic features of libtap++. these tests do not
 * cover any of the symbols defined when WANT_TEST_EXTRAS is 
 * defined
 */

#include "headers/tap++.h"

using namespace TAP;

int main() {
  plan(10);

  ok(1+1 == 2, "1+1 == 2");

  skip(2, "tests not needed");

  not_ok(true, "not_ok expects a false condition");

  ok(1, "simply test truthiness of 1");

  isnt(1, 2, "1 is not the same as 2");

  pass("wunderbar! this test passed");

  fail("this test has been failed");

  note("TAP internally keeps track of the number of tests it expects to find");

  std::cout << "planned for " << planned() << "tests" << std::endl;
  std::cout << "encountered " << encountered() << "tests" << std::endl;

  done_testing();

  return exit_status();
}
