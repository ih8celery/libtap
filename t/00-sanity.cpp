/*
 * 00-sanity.cpp
 *
 * perform the most basic tests of libtap, not going into great depth
 */

#include "headers/tap++.h"

using namespace TAP;

int main() {
  plan(no_plan);

  ok(1, "the value 1 is true");

  not_ok(0, "the value 0 is false");

  ok(true, "true is true");

  ok(1 == 1, "1 == 1");

  done_testing();

  return 0;
}
