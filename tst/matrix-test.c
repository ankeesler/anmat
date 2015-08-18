//
// matrix-test.c
//
// Andrew Keesler
//
// August 14, 2015
//
// Matrix API.
//

#include <unit-test.h>

#include "matrix.h"

#include "./test-util.h"

static int allocTest(void)
{
  struct ANMAT_Matrix_t matrix1, matrix2;

  // Heap should be fresh.
  expectHeapFull();

  // No rows or cols is bad.
  expectEquals(ANMAT_MatrixAlloc(&matrix1, 0, 0), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixAlloc(&matrix1, 1, 0), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixAlloc(&matrix1, 0, 1), ANMAT_BAD_ARG);

  // Still haven't touched the heap.
  expectHeapFull();

  // Rows and cols is good.
  expectEquals(ANMAT_MatrixAlloc(&matrix1, 5, 5), ANMAT_SUCCESS);

  // Heap should be missing some bytes.
  expectHeapSize(HEAP_SIZE
                 // 5 rows, with 1 alloc byte
                 - (5 * sizeof(double *)) - 1
                 // 5 rows with 5 doubles plus an alloc byte each
                 - 5 * ((5 * sizeof(double)) + 1)
                 - 0);

  // Free.
  ANMAT_MatrixFree(&matrix1);

  // Heap should be full.
  expectHeapFull();

  // Allocate two matrices.
  expectEquals(ANMAT_MatrixAlloc(&matrix1, 3, 3), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrix2, 6, 6), ANMAT_SUCCESS);

  // Free them.
  ANMAT_MatrixFree(&matrix1);
  ANMAT_MatrixFree(&matrix2);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

static int dataTest(void)
{
  struct ANMAT_Matrix_t matrixA, matrixB;

  // Heap should be full.
  expectHeapFull();

  // Allocate.
  expectEquals(ANMAT_MatrixAlloc(&matrixA, 6, 3), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixB, 2, 5), ANMAT_SUCCESS);

  // Check rows/cols.
  expectEquals(ANMAT_MatrixRowCount(&matrixA), 6);
  expectEquals(ANMAT_MatrixColCount(&matrixA), 3);
  expectEquals(ANMAT_MatrixRowCount(&matrixB), 2);
  expectEquals(ANMAT_MatrixColCount(&matrixB), 5);

  // Data.
  ANMAT_MatrixData(&matrixA, 0, 0) = 5;
  expectEquals(matrixA.data[0][0], 5);
  expectEquals(ANMAT_MatrixData(&matrixA, 0, 0), 5);
  ANMAT_MatrixData(&matrixB, 1, 4) = 10.123;
  expectEquals(matrixB.data[1][4], 10.123);
  expectEquals(ANMAT_MatrixData(&matrixB, 1, 4), 10.123);

  // Free.
  ANMAT_MatrixFree(&matrixA);
  ANMAT_MatrixFree(&matrixB);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

static int elemOpTest(void)
{
  struct ANMAT_Matrix_t matrixA, matrixB, matrixC, matrixD, matrixE;

  // Heap should be full.
  expectHeapFull();

  // Allocate.
  expectEquals(ANMAT_MatrixAlloc(&matrixA, 3, 4), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixB, 3, 4), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixC, 4, 4), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixD, 4, 3), ANMAT_SUCCESS);

  // Can't do things with wrong dimensions.
  expect(!ANMAT_MatrixEquals(&matrixA, &matrixC));
  expect(!ANMAT_MatrixEquals(&matrixA, &matrixD));
  expectEquals(ANMAT_MatrixAdd(&matrixA, &matrixC, &matrixE), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixAdd(&matrixA, &matrixD, &matrixE), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixSubtract(&matrixA, &matrixC, &matrixE), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixSubtract(&matrixA, &matrixD, &matrixE), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixMultiply(&matrixA, &matrixB, &matrixE), ANMAT_BAD_ARG);

  // Matrices should be equal coming out of the gate (all 0's).
  expect(ANMAT_MatrixEquals(&matrixA, &matrixB));

  // Adding works as it should.
  ANMAT_MatrixData(&matrixA, 0, 1) = 1;
  ANMAT_MatrixData(&matrixB, 1, 0) = 2;
  ANMAT_MatrixData(&matrixA, 2, 2) = 3;
  ANMAT_MatrixData(&matrixB, 2, 2) = 3;
  expectEquals(ANMAT_MatrixAlloc(&matrixE, 3, 4), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAdd(&matrixA, &matrixB, &matrixE), ANMAT_SUCCESS);
  expect(ANMAT_MatrixData(&matrixE, 1, 1) == 0);
  expect(ANMAT_MatrixData(&matrixE, 0, 1) == 1);
  expect(ANMAT_MatrixData(&matrixE, 1, 0) == 2);
  expect(ANMAT_MatrixData(&matrixE, 2, 2) == 6);

  // Subtraction works as it should.
  ANMAT_MatrixData(&matrixA, 0, 1) = 1;
  ANMAT_MatrixData(&matrixB, 1, 0) = 2;
  ANMAT_MatrixData(&matrixA, 2, 2) = 5;
  ANMAT_MatrixData(&matrixB, 2, 2) = 3;
  expectEquals(ANMAT_MatrixSubtract(&matrixA, &matrixB, &matrixE), ANMAT_SUCCESS);
  expect(ANMAT_MatrixData(&matrixE, 1, 1) == 0);
  expect(ANMAT_MatrixData(&matrixE, 0, 1) == 1);
  expect(ANMAT_MatrixData(&matrixE, 1, 0) == -2);
  expect(ANMAT_MatrixData(&matrixE, 2, 2) == 2);

  // Multiplication works as it should.
  ANMAT_MatrixData(&matrixA, 0, 0) = 1;
  ANMAT_MatrixData(&matrixA, 0, 1) = -1;
  ANMAT_MatrixData(&matrixA, 0, 2) = 0;
  ANMAT_MatrixData(&matrixA, 2, 0) = 0;
  ANMAT_MatrixData(&matrixA, 2, 1) = 1;
  ANMAT_MatrixData(&matrixA, 2, 2) = -1;
  ANMAT_MatrixData(&matrixC, 0, 0) = 2;
  ANMAT_MatrixData(&matrixC, 1, 0) = 3;
  ANMAT_MatrixData(&matrixC, 2, 0) = 4;
  ANMAT_MatrixData(&matrixC, 0, 2) = 5;
  ANMAT_MatrixData(&matrixC, 1, 2) = 6;
  ANMAT_MatrixData(&matrixC, 2, 2) = 7;
  expectEquals(ANMAT_MatrixMultiply(&matrixA, &matrixC, &matrixE), ANMAT_SUCCESS);
  expect(ANMAT_MatrixData(&matrixE, 0, 0) == -1);
  expect(ANMAT_MatrixData(&matrixE, 0, 2) == -1);
  expect(ANMAT_MatrixData(&matrixE, 2, 0) == -1);
  expect(ANMAT_MatrixData(&matrixE, 2, 2) == -1);

  // Free.
  ANMAT_MatrixFree(&matrixA);
  ANMAT_MatrixFree(&matrixB);
  ANMAT_MatrixFree(&matrixC);
  ANMAT_MatrixFree(&matrixD);
  ANMAT_MatrixFree(&matrixE);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

static int transposeTest(void)
{
  struct ANMAT_Matrix_t matrixA, matrixB, matrixC, matrixD;

  // Heap should be full.
  expectHeapFull();

  // Alloc.
  expectEquals(ANMAT_MatrixAlloc(&matrixA, 2, 3), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixB, 3, 2), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixC, 2, 2), ANMAT_SUCCESS);
  expectEquals(ANMAT_MatrixAlloc(&matrixD, 3, 3), ANMAT_SUCCESS);

  // Can't transpose with bad dimensions.
  expectEquals(ANMAT_MatrixTranspose(&matrixA, &matrixC), ANMAT_BAD_ARG);
  expectEquals(ANMAT_MatrixTranspose(&matrixA, &matrixD), ANMAT_BAD_ARG);

  // Transposition works as expected.
  ANMAT_MatrixData(&matrixA, 0, 0) = 1;
  ANMAT_MatrixData(&matrixA, 0, 1) = 2;
  ANMAT_MatrixData(&matrixA, 0, 2) = 3;
  ANMAT_MatrixData(&matrixA, 1, 0) = 4;
  ANMAT_MatrixData(&matrixA, 1, 1) = 5;
  ANMAT_MatrixData(&matrixA, 1, 2) = 6;
  expectEquals(ANMAT_MatrixTranspose(&matrixA, &matrixB), ANMAT_SUCCESS);
  expect(ANMAT_MatrixData(&matrixB, 0, 0) == 1);
  expect(ANMAT_MatrixData(&matrixB, 0, 1) == 4);
  expect(ANMAT_MatrixData(&matrixB, 1, 0) == 2);
  expect(ANMAT_MatrixData(&matrixB, 1, 1) == 5);
  expect(ANMAT_MatrixData(&matrixB, 2, 0) == 3);
  expect(ANMAT_MatrixData(&matrixB, 2, 1) == 6);

  // Free.
  ANMAT_MatrixFree(&matrixA);
  ANMAT_MatrixFree(&matrixB);
  ANMAT_MatrixFree(&matrixC);
  ANMAT_MatrixFree(&matrixD);

  // Heap should be full.
  expectHeapFull();

  return 0;
}

int main(void)
{
  announce();

  run(allocTest);
  run(dataTest);
  run(elemOpTest);
  run(transposeTest);

  return 0;
}
