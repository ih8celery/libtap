/**
 * \file 00-sanity.cpp
 * 
 * \author Adam Marshall (ih8celery)
 *
 * \brief perform the most basic tests of libtap, not going into great depth
 */

#include "tap++.h"

using namespace TAP;

int main() {
  plan(9);

  ok(1, "the value 1 is true");

  not_ok(0, "the value 0 is false");

  ok(true, "true is true");

  ok(encountered() == 3, "found 3 tests here");

  ok(1 == 1, "1 == 1");

  ok(planned() == 9, "planned 9 tests");

  note("'is' is defined as a template");
  is(6, 6, "6 is 6");

  isnt(6, 7, "6 is not 7");

  ok(details::test_status, "previous test succeeded");

  note("\'", details::separator, "\' is the separator string");

  done_testing();

  return exit_status();
}
