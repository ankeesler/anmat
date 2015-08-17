//
// matrix.c
//
// Andrew Keesler
//
// August 14, 2015
//
// Matrix API.
//

#include "matrix.h"
#include "src/heap.h"

#include <string.h> // memset()

// -----------------------------------------------------------------------------
// Private Functionality

//#define MATRIX_DEBUG
#ifdef MATRIX_DEBUG
  #define note(...) printf(__VA_ARGS__), fflush(0);
#else
  #define note(...)
#endif

#define FOR_ROW(matrix, rowI) for (rowI = 0; rowI < (matrix)->rows; rowI ++) 
#define FOR_COL(matrix, colI) for (colI = 0; colI < (matrix)->cols; colI ++) 

#define dimensionsAreEqual(matrixA, matrixB)                                 \
  ((matrixA)->rows == (matrixB)->rows && (matrixA)->cols == (matrixB)->cols)

// -----------------------------------------------------------------------------
// Memory Management

enum ANMAT_Status_t ANMAT_MatrixAlloc(struct ANMAT_Matrix_t *matrix,
                                      uint32_t rows,
                                      uint32_t cols)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;
  uint32_t rowI, colI;

  if (rows && cols) {
    status = ANMAT_SUCCESS;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (double **)ANMAT_HeapAlloc(rows * sizeof(double *));
    if (!matrix->data) {
      status = ANMAT_MEM_ERR;
    }

    for (rowI = 0; rowI < rows && status == ANMAT_SUCCESS; rowI ++) {
      matrix->data[rowI] = (double *)ANMAT_HeapAlloc(cols * sizeof(double));
      if (!matrix->data[rowI]) {
        status = ANMAT_MEM_ERR;
        ANMAT_MatrixFree(matrix);
      } else {
        FOR_COL(matrix, colI) {
          matrix->data[rowI][colI] = 0;
        }
      }
    }
  }

  return status;
}

void ANMAT_MatrixFree(struct ANMAT_Matrix_t *matrix)
{
  uint32_t rowI;

  for (rowI = 0; rowI < matrix->rows; rowI ++) {
    if (matrix->data[rowI]) {
      ANMAT_HeapFree(matrix->data[rowI]);
    }
  }

  if (matrix->data) {
    ANMAT_HeapFree(matrix->data);
  }
}

// -----------------------------------------------------------------------------
// Elementary operations

bool ANMAT_MatrixEquals(struct ANMAT_Matrix_t *matrixA,
                        struct ANMAT_Matrix_t *matrixB)
{
  uint32_t rowI, colI;

  if (!dimensionsAreEqual(matrixA, matrixB)) {
    note("ANMAT_MatrixEquals: dimensions are not equal.");
    return false;
  }

  FOR_ROW(matrixA, rowI) {
    FOR_COL(matrixA, colI) {
      if (matrixA->data[rowI][colI] != matrixB->data[rowI][colI]) {
        note("ANMAT_MatrixEquals: values are not equal: ");
        note("matrixA->data[%d][%d] = %lf) != (matrixB[%d][%d] = %lf)",
             rowI, colI, matrixA->data[rowI][colI],
             rowI, colI, matrixB->data[rowI][colI]);
        return false;
      }
    }
  }

  return true;
}

static enum ANMAT_Status_t addOrSubtractMatrices(struct ANMAT_Matrix_t *matrixA,
                                                 struct ANMAT_Matrix_t *matrixB,
                                                 struct ANMAT_Matrix_t *matrixC,
                                                 bool add)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;
  uint32_t rowI, colI;

  if (dimensionsAreEqual(matrixA, matrixB)
      && dimensionsAreEqual(matrixB, matrixC)) {
    status = ANMAT_SUCCESS;
    FOR_ROW(matrixC, rowI) {
      FOR_COL(matrixC, colI) {
        matrixC->data[rowI][colI]
          = (add
             ? matrixA->data[rowI][colI] + matrixB->data[rowI][colI]
             : matrixA->data[rowI][colI] - matrixB->data[rowI][colI]);
      }
    }
  }

  return status;
}

enum ANMAT_Status_t ANMAT_MatrixAdd(struct ANMAT_Matrix_t *matrixA,
                                    struct ANMAT_Matrix_t *matrixB,
                                    struct ANMAT_Matrix_t *matrixC)
{
  return addOrSubtractMatrices(matrixA, matrixB, matrixC, true); // add?
}

enum ANMAT_Status_t ANMAT_MatrixSubtract(struct ANMAT_Matrix_t *matrixA,
                                         struct ANMAT_Matrix_t *matrixB,
                                         struct ANMAT_Matrix_t *matrixC)
{
  return addOrSubtractMatrices(matrixA, matrixB, matrixC, false); // add?
}

// -----------------------------------------------------------------------------
// I/O

enum ANMAT_Status_t ANMAT_MatrixPrint(struct ANMAT_Matrix_t *matrix,
                                      FILE *stream)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;

  return status;
}

enum ANMAT_Status_t ANMAT_MatrixScan(struct ANMAT_Matrix_t *matrix,
                                     FILE *stream)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;

  return status;
}
