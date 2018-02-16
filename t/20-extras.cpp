/*
 * 20-extras.cpp
 *
 * demonstrate test extras
 */

#define WANT_TEST_EXTRAS
#include "headers/tap++.h"

int main() {
  TEST_START(3);

  TRY(throw 1, "throws an exception");

  TRY_DECL(int age = 5, "declare age to be 5");

  FAIL(int level{9000.001},"should fail for some reason");

  TEST_END();

  return 0;
}
