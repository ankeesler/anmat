//
// stat-test.c
//
// Andrew Keesler
//
// August 28, 2015
//
// Statistics unit test.
//

#include <unit-test.h>

#include "stat.h"

#include "./test-util.h"

static int allocTest(void)
{
  AnmatVector_t vector;

  // Heap should be full.
  expectHeapEmpty();

  // Cannot allocate vector of 0 length.
  expectEquals(anmatVectorAlloc(&vector, 0), ANMAT_BAD_ARG);

  // Allocate a vector for real.
  expectEquals(anmatVectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Some bytes should be gone from heap.
  expectHeapSize(HEAP_SIZE
                 // 5 doubles, plus an alloc byte
                 - (5 * sizeof(double)) - 1
                 - 0);

  // Free.
  anmatVectorFree(&vector);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int dataTest(void)
{
  AnmatVector_t vector;

  // Heap should be full.
  expectHeapEmpty();
  
  // Alloc.
  expectEquals(anmatVectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Count.
  expectEquals(anmatVectorCount(&vector), 5);

  // Data.
  anmatVectorData(&vector, 0) = 1;
  anmatVectorData(&vector, 1) = 3;
  anmatVectorData(&vector, 4) = 10;
  expectEquals(anmatVectorData(&vector, 0), 1);
  expectEquals(anmatVectorData(&vector, 1), 3);
  expectEquals(anmatVectorData(&vector, 4), 10);
  anmatVectorData(&vector, 0) = 10.1;
  anmatVectorData(&vector, 1) = 3.2;
  anmatVectorData(&vector, 4) = -19.33;
  expectEquals(anmatVectorData(&vector, 0), 10.1);
  expectEquals(anmatVectorData(&vector, 1), 3.2);
  expectEquals(anmatVectorData(&vector, 4), -19.33);

  // Free.
  anmatVectorFree(&vector);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int averageTest(void)
{
  AnmatVector_t vector;

  // Heap should be full.
  expectHeapEmpty();
  
  // Alloc.
  expectEquals(anmatVectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Average.
  anmatVectorData(&vector, 0) = 1;
  anmatVectorData(&vector, 1) = 3;
  anmatVectorData(&vector, 2) = 3;
  anmatVectorData(&vector, 3) = 3;
  anmatVectorData(&vector, 4) = 5;
  expectEquals(anmatStatAverage(&vector), 3);

  anmatVectorData(&vector, 0) = -2;
  anmatVectorData(&vector, 1) = 1;
  anmatVectorData(&vector, 2) = -1;
  anmatVectorData(&vector, 3) = 0;
  anmatVectorData(&vector, 4) = 2;
  expectEquals(anmatStatAverage(&vector), 0);

  anmatVectorData(&vector, 0) = 1.2;
  anmatVectorData(&vector, 1) = 1.0;
  anmatVectorData(&vector, 2) = 1.4;
  anmatVectorData(&vector, 3) = 0.8;
  anmatVectorData(&vector, 4) = 0.6;
  expectEquals(anmatStatAverage(&vector), 1.0);

  // Free.
  anmatVectorFree(&vector);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

int main(void)
{
  announce();

  run(allocTest);
  run(dataTest);
  run(averageTest);

  return 0;
}
