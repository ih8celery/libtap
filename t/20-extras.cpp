/*
 * 20-extras.cpp
 *
 * demonstrate test extras
 */

#define WANT_TEST_EXTRAS
#include "tap++.h"

using namespace TAP;

int main() {
  TEST_START(1);

  //TRY(throw 1, "throws an exception");

  TRY_DECL(int age = 5, "declare age to be 5");

  TEST_END(1);

  return 0;
}
