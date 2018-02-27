/*
 * 20-extras.cpp
 *
 * demonstrate test extras
 */

#define WANT_TEST_EXTRAS
#include "tap++.h"

using namespace TAP;

int main() {
  plan(1);

  TRY_NOT_OK(throw 1, "throws an exception");

  //TRY_DECL(int age = 5, "declare age to be 5");

  done_testing();

  return exit_status();
}
