/*
 * 00-sanity.cpp
 *
 * perform the most basic tests of libtap, not going into great depth
 */

#include "tap++.h"

using namespace TAP;

int main() {
  plan(5);

  ok(1, "the value 1 is true");

  not_ok(0, "the value 0 is false");

  ok(true, "true is true");

  ok(1 == 1, "1 == 1");

  note("is is defined as a template");
  is(5, 5, "5 is 5");

  done_testing();

  return exit_status();
}
