//
// util-test.c
//
// Andrew Keesler
//
// August 28, 2015
//
// Tests for utilities.
//

#include <unit-test.h>

#include "util.h"

static int utilTest(void)
{
  expectEquals(ANMAT_UtilMax(5, 6), 6);
  expectEquals(ANMAT_UtilMax(5.5, 5.6), 5.6);
  expectEquals(ANMAT_UtilMax(5.0, 5.0 + .001), 5.0 + .001);
  expectEquals(ANMAT_UtilMax(-1, 1), 1);

  expectEquals(ANMAT_UtilAbs(5), 5);
  expectEquals(ANMAT_UtilAbs(5.4321), 5.4321);
  expectEquals(ANMAT_UtilAbs(-5.4321), 5.4321);
  expectEquals(ANMAT_UtilAbs(0), 0);
  expectEquals(ANMAT_UtilAbs(-0.1), 0.1);

  expect(ANMAT_UtilNeighborhood(1, 1.5, .5));
  expect(ANMAT_UtilNeighborhood(1, 1.5, .75));
  expect(ANMAT_UtilNeighborhood(-1, 2, 3));
  expect(ANMAT_UtilNeighborhood(-1, 2, 3.1));

  return 0;
}

int main(void)
{
  announce();

  run(utilTest);

  return 0;
}
