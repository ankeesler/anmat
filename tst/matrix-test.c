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
#include <unistd.h>    // unlink()
#include <stdlib.h>    // srand(), rand()

#include "matrix.h"

#include "./test-util.h"

#define TMP_FILE "./tmp"
static FILE *oStream = NULL, *iStream = NULL;

static void failureHandler(void)
{
  //unlink(TMP_FILE);
}

static int allocTest(void)
{
  AnmatMatrix_t matrix1, matrix2;

  // Heap should be fresh.
  expectHeapEmpty();

  // No rows or cols is bad.
  expectEquals(anmatMatrixAlloc(&matrix1, 0, 0), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixAlloc(&matrix1, 1, 0), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixAlloc(&matrix1, 0, 1), ANMAT_BAD_ARG);

  // Still haven't touched the heap.
  expectHeapEmpty();

  // Rows and cols is good.
  expectEquals(anmatMatrixAlloc(&matrix1, 5, 5), ANMAT_SUCCESS);

  // Heap should be missing some bytes.
  expectHeapSize(HEAP_SIZE
                 // 5 rows, with 1 alloc byte
                 - (5 * sizeof(double *)) - 1
                 // 5 rows with 5 doubles plus an alloc byte each
                 - 5 * ((5 * sizeof(double)) + 1)
                 - 0);

  // Free.
  anmatMatrixFree(&matrix1);

  // Heap should be full.
  expectHeapEmpty();

  // Allocate two matrices.
  expectEquals(anmatMatrixAlloc(&matrix1, 3, 3), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrix2, 6, 6), ANMAT_SUCCESS);

  // Free them.
  anmatMatrixFree(&matrix1);
  anmatMatrixFree(&matrix2);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int dataTest(void)
{
  AnmatMatrix_t matrixA, matrixB;

  // Heap should be full.
  expectHeapEmpty();

  // Allocate.
  expectEquals(anmatMatrixAlloc(&matrixA, 6, 3), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixB, 2, 5), ANMAT_SUCCESS);

  // Check rows/cols.
  expectEquals(anmatMatrixRowCount(&matrixA), 6);
  expectEquals(anmatMatrixColCount(&matrixA), 3);
  expectEquals(anmatMatrixRowCount(&matrixB), 2);
  expectEquals(anmatMatrixColCount(&matrixB), 5);

  // Data.
  anmatMatrixData(&matrixA, 0, 0) = 5;
  expectEquals(matrixA.data[0][0], 5);
  expectEquals(anmatMatrixData(&matrixA, 0, 0), 5);
  anmatMatrixData(&matrixB, 1, 4) = 10.123;
  expectEquals(matrixB.data[1][4], 10.123);
  expectEquals(anmatMatrixData(&matrixB, 1, 4), 10.123);

  // Free.
  anmatMatrixFree(&matrixA);
  anmatMatrixFree(&matrixB);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int elemOpTest(void)
{
  AnmatMatrix_t matrixA, matrixB, matrixC, matrixD, matrixE;

  // Heap should be full.
  expectHeapEmpty();

  // Allocate.
  expectEquals(anmatMatrixAlloc(&matrixA, 3, 4), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixB, 3, 4), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixC, 4, 4), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixD, 4, 3), ANMAT_SUCCESS);

  // Can't do things with wrong dimensions.
  expect(!anmatMatrixEquals(&matrixA, &matrixC));
  expect(!anmatMatrixEquals(&matrixA, &matrixD));
  expectEquals(anmatMatrixAdd(&matrixA, &matrixC, &matrixE), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixAdd(&matrixA, &matrixD, &matrixE), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixSubtract(&matrixA, &matrixC, &matrixE), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixSubtract(&matrixA, &matrixD, &matrixE), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixMultiply(&matrixA, &matrixB, &matrixE), ANMAT_BAD_ARG);

  // Matrices should be equal coming out of the gate (all 0's).
  expect(anmatMatrixEquals(&matrixA, &matrixB));

  // Adding works as it should.
  anmatMatrixData(&matrixA, 0, 1) = 1;
  anmatMatrixData(&matrixB, 1, 0) = 2;
  anmatMatrixData(&matrixA, 2, 2) = 3;
  anmatMatrixData(&matrixB, 2, 2) = 3;
  expectEquals(anmatMatrixAlloc(&matrixE, 3, 4), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAdd(&matrixA, &matrixB, &matrixE), ANMAT_SUCCESS);
  expect(anmatMatrixData(&matrixE, 1, 1) == 0);
  expect(anmatMatrixData(&matrixE, 0, 1) == 1);
  expect(anmatMatrixData(&matrixE, 1, 0) == 2);
  expect(anmatMatrixData(&matrixE, 2, 2) == 6);

  // Subtraction works as it should.
  anmatMatrixData(&matrixA, 0, 1) = 1;
  anmatMatrixData(&matrixB, 1, 0) = 2;
  anmatMatrixData(&matrixA, 2, 2) = 5;
  anmatMatrixData(&matrixB, 2, 2) = 3;
  expectEquals(anmatMatrixSubtract(&matrixA, &matrixB, &matrixE), ANMAT_SUCCESS);
  expect(anmatMatrixData(&matrixE, 1, 1) == 0);
  expect(anmatMatrixData(&matrixE, 0, 1) == 1);
  expect(anmatMatrixData(&matrixE, 1, 0) == -2);
  expect(anmatMatrixData(&matrixE, 2, 2) == 2);

  // Multiplication works as it should.
  anmatMatrixData(&matrixA, 0, 0) = 1;
  anmatMatrixData(&matrixA, 0, 1) = -1;
  anmatMatrixData(&matrixA, 0, 2) = 0;
  anmatMatrixData(&matrixA, 2, 0) = 0;
  anmatMatrixData(&matrixA, 2, 1) = 1;
  anmatMatrixData(&matrixA, 2, 2) = -1;
  anmatMatrixData(&matrixC, 0, 0) = 2;
  anmatMatrixData(&matrixC, 1, 0) = 3;
  anmatMatrixData(&matrixC, 2, 0) = 4;
  anmatMatrixData(&matrixC, 0, 2) = 5;
  anmatMatrixData(&matrixC, 1, 2) = 6;
  anmatMatrixData(&matrixC, 2, 2) = 7;
  expectEquals(anmatMatrixMultiply(&matrixA, &matrixC, &matrixE), ANMAT_SUCCESS);
  expect(anmatMatrixData(&matrixE, 0, 0) == -1);
  expect(anmatMatrixData(&matrixE, 0, 2) == -1);
  expect(anmatMatrixData(&matrixE, 2, 0) == -1);
  expect(anmatMatrixData(&matrixE, 2, 2) == -1);

  // Free.
  anmatMatrixFree(&matrixA);
  anmatMatrixFree(&matrixB);
  anmatMatrixFree(&matrixC);
  anmatMatrixFree(&matrixD);
  anmatMatrixFree(&matrixE);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int transposeTest(void)
{
  AnmatMatrix_t matrixA, matrixB, matrixC, matrixD;

  // Heap should be full.
  expectHeapEmpty();

  // Alloc.
  expectEquals(anmatMatrixAlloc(&matrixA, 2, 3), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixB, 3, 2), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixC, 2, 2), ANMAT_SUCCESS);
  expectEquals(anmatMatrixAlloc(&matrixD, 3, 3), ANMAT_SUCCESS);

  // Can't transpose with bad dimensions.
  expectEquals(anmatMatrixTranspose(&matrixA, &matrixC), ANMAT_BAD_ARG);
  expectEquals(anmatMatrixTranspose(&matrixA, &matrixD), ANMAT_BAD_ARG);

  // Transposition works as expected.
  anmatMatrixData(&matrixA, 0, 0) = 1;
  anmatMatrixData(&matrixA, 0, 1) = 2;
  anmatMatrixData(&matrixA, 0, 2) = 3;
  anmatMatrixData(&matrixA, 1, 0) = 4;
  anmatMatrixData(&matrixA, 1, 1) = 5;
  anmatMatrixData(&matrixA, 1, 2) = 6;
  expectEquals(anmatMatrixTranspose(&matrixA, &matrixB), ANMAT_SUCCESS);
  expect(anmatMatrixData(&matrixB, 0, 0) == 1);
  expect(anmatMatrixData(&matrixB, 0, 1) == 4);
  expect(anmatMatrixData(&matrixB, 1, 0) == 2);
  expect(anmatMatrixData(&matrixB, 1, 1) == 5);
  expect(anmatMatrixData(&matrixB, 2, 0) == 3);
  expect(anmatMatrixData(&matrixB, 2, 1) == 6);

  // Free.
  anmatMatrixFree(&matrixA);
  anmatMatrixFree(&matrixB);
  anmatMatrixFree(&matrixC);
  anmatMatrixFree(&matrixD);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

static int ioTest(void)
{
  AnmatMatrix_t matrixA, matrixB;
  uint32_t rowI, colI;

  // Heap should be full.
  expectHeapEmpty();

  // Alloc.
  expectEquals(anmatMatrixAlloc(&matrixA, 11, 11), ANMAT_SUCCESS);

  // We should be missing some bytes from the heap.
  expectHeapSize(HEAP_SIZE
                 // 11 rows, with 1 alloc byte
                 - (11 * sizeof(double)) - 1
                 // 11 numbers in each row, each with an alloc byte
                 - (11 * ((11 * sizeof(double)) + 1))
                 - 0);

  // Set data.
  srand(1);
  for (rowI = 0; rowI < anmatMatrixRowCount(&matrixA); rowI ++) {
    for (colI = 0; colI < anmatMatrixColCount(&matrixA); colI ++) {
      anmatMatrixData(&matrixA, rowI, colI) = (rand() % 100) / 1.5;
    }
  }

  // Open up out stream.
  expect((oStream = fopen(TMP_FILE, "w")) != NULL);

  // A bad stream should result in a bad scan.
  expectEquals(anmatMatrixScan(&matrixB, oStream), ANMAT_BAD_ARG);

  // Print and close the out stream.
  expectEquals(anmatMatrixPrint(&matrixA, oStream), ANMAT_SUCCESS);
  fclose(oStream);

  // Open up in stream and scan.
  expect((iStream = fopen(TMP_FILE, "r")) != NULL);
  expectEquals(anmatMatrixScan(&matrixB, iStream), ANMAT_SUCCESS);

  // They should be equal.
  expect(anmatMatrixEquals(&matrixA, &matrixB));

  // Close in stream and remove file.
  fclose(iStream);
  unlink(TMP_FILE);

  // Free.
  anmatMatrixFree(&matrixA);
  anmatMatrixFree(&matrixB);

  // Heap should be full.
  expectHeapEmpty();

  return 0;
}

int main(void)
{
  announce();

  setFailureHandler(failureHandler);

  run(allocTest);
  run(dataTest);
  run(elemOpTest);
  run(transposeTest);
  run(ioTest);

  return 0;
}
