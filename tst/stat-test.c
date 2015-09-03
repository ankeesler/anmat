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
  expectHeapFull();

  // Cannot allocate vector of 0 length.
  expectEquals(ANMAT_VectorAlloc(&vector, 0), ANMAT_BAD_ARG);

  // Allocate a vector for real.
  expectEquals(ANMAT_VectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Some bytes should be gone from heap.
  expectHeapSize(HEAP_SIZE
                 // 5 doubles, plus an alloc byte
                 - (5 * sizeof(double)) - 1
                 - 0);

  // Free.
  ANMAT_VectorFree(&vector);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

static int dataTest(void)
{
  AnmatVector_t vector;

  // Heap should be full.
  expectHeapFull();
  
  // Alloc.
  expectEquals(ANMAT_VectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Count.
  expectEquals(ANMAT_VectorCount(&vector), 5);

  // Data.
  ANMAT_VectorData(&vector, 0) = 1;
  ANMAT_VectorData(&vector, 1) = 3;
  ANMAT_VectorData(&vector, 4) = 10;
  expectEquals(ANMAT_VectorData(&vector, 0), 1);
  expectEquals(ANMAT_VectorData(&vector, 1), 3);
  expectEquals(ANMAT_VectorData(&vector, 4), 10);
  ANMAT_VectorData(&vector, 0) = 10.1;
  ANMAT_VectorData(&vector, 1) = 3.2;
  ANMAT_VectorData(&vector, 4) = -19.33;
  expectEquals(ANMAT_VectorData(&vector, 0), 10.1);
  expectEquals(ANMAT_VectorData(&vector, 1), 3.2);
  expectEquals(ANMAT_VectorData(&vector, 4), -19.33);

  // Free.
  ANMAT_VectorFree(&vector);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

static int averageTest(void)
{
  AnmatVector_t vector;

  // Heap should be full.
  expectHeapFull();
  
  // Alloc.
  expectEquals(ANMAT_VectorAlloc(&vector, 5), ANMAT_SUCCESS);

  // Average.
  ANMAT_VectorData(&vector, 0) = 1;
  ANMAT_VectorData(&vector, 1) = 3;
  ANMAT_VectorData(&vector, 2) = 3;
  ANMAT_VectorData(&vector, 3) = 3;
  ANMAT_VectorData(&vector, 4) = 5;
  expectEquals(ANMAT_StatAverage(&vector), 3);

  ANMAT_VectorData(&vector, 0) = -2;
  ANMAT_VectorData(&vector, 1) = 1;
  ANMAT_VectorData(&vector, 2) = -1;
  ANMAT_VectorData(&vector, 3) = 0;
  ANMAT_VectorData(&vector, 4) = 2;
  expectEquals(ANMAT_StatAverage(&vector), 0);

  ANMAT_VectorData(&vector, 0) = 1.2;
  ANMAT_VectorData(&vector, 1) = 1.0;
  ANMAT_VectorData(&vector, 2) = 1.4;
  ANMAT_VectorData(&vector, 3) = 0.8;
  ANMAT_VectorData(&vector, 4) = 0.6;
  expectEquals(ANMAT_StatAverage(&vector), 1.0);

  // Free.
  ANMAT_VectorFree(&vector);

  // Heap should be full.
  expectHeapFull();

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
