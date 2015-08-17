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

#define FOR_ROW(matrix, rowI) for (rowI = 0; rowI < (matrix)->rows; rowI ++) 
#define FOR_COL(matrix, colI) for (colI = 0; colI < (matrix)->cols; colI ++) 

// -----------------------------------------------------------------------------
// Memory Management

enum ANMAT_Status_t ANMAT_MatrixAlloc(struct ANMAT_Matrix_t *matrix,
                                      uint32_t rows,
                                      uint32_t cols)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;
  uint32_t rowI;

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
        memset(matrix->data[rowI], 0, cols);
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

enum ANMAT_Status_t ANMAT_MatrixAdd(struct ANMAT_Matrix_t *matrixA,
                                    struct ANMAT_Matrix_t *matrixB,
                                    struct ANMAT_Matrix_t *matrixC)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;
  uint32_t rowI, colI;

  if (matrixA->rows == matrixB->rows
      && matrixA->cols == matrixB->cols
      && matrixB->rows == matrixC->rows
      && matrixB->cols == matrixC->cols) {
    status = ANMAT_SUCCESS;
    FOR_ROW(matrixC, rowI) {
      FOR_COL(matrixC, colI) {
        matrixC->data[rowI][colI]
          = matrixA->data[rowI][colI] + matrixB->data[rowI][colI];
      }
    }
  }

  return status;
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
