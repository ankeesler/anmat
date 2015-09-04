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
  expectEquals(anmatUtilMax(5, 6), 6);
  expectEquals(anmatUtilMax(5.5, 5.6), 5.6);
  expectEquals(anmatUtilMax(5.0, 5.0 + .001), 5.0 + .001);
  expectEquals(anmatUtilMax(-1, 1), 1);

  expectEquals(anmatUtilAbs(5), 5);
  expectEquals(anmatUtilAbs(5.4321), 5.4321);
  expectEquals(anmatUtilAbs(-5.4321), 5.4321);
  expectEquals(anmatUtilAbs(0), 0);
  expectEquals(anmatUtilAbs(-0.1), 0.1);

  expect(anmatUtilNeighborhood(1, 1.5, .5));
  expect(anmatUtilNeighborhood(1, 1.5, .75));
  expect(anmatUtilNeighborhood(-1, 2, 3));
  expect(anmatUtilNeighborhood(-1, 2, 3.1));

  //expectEquals(anmatUtilPower(1, 0), 1);
  //expectEquals(anmatUtilPower(1, 1), 1);
  //expectEquals(anmatUtilPower(1, 2), 1);
  //expectEquals(anmatUtilPower(1, 5), 1);
  //expectEquals(anmatUtilPower(0, 0), );
  //expectEquals(anmatUtilPower(0, 1), 0);
  //expectEquals(anmatUtilPower(0, 2), 0);
  //expectEquals(anmatUtilPower(0, 5), 0);

  return 0;
}

static int memTest(void)
{
  unsigned char buffer1[10] = {1,2,3,4,5,6,7,8,9,10,}, buffer2[10] = {0,};
  unsigned char *src, *dst;
  unsigned int i;

  anmatMemcpy(buffer2, buffer1, 10);
  for (i = 0; i <= 9; i ++) { expectEquals(buffer2[i], buffer1[i]); }

  src = &buffer1[0];
  dst = &buffer1[2];
  anmatMemcpy(dst, src, 5);
  expectEquals(dst[0], 1);
  expectEquals(dst[1], 2);
  expectEquals(dst[2], 3);
  expectEquals(dst[3], 4);
  expectEquals(dst[4], 5);

  src = &buffer1[7];
  dst = &buffer1[6];
  anmatMemcpy(dst, src, 3);
  expectEquals(dst[0], 8);
  expectEquals(dst[1], 9);
  expectEquals(dst[2], 10);

  return 0;
}

int main(void)
{
  announce();

  run(utilTest);
  run(memTest);

  return 0;
}
