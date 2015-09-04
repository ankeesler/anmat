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

static inline double dotProduct(double *u, double *v, uint32_t length)
{
  double total = 0;

  while (length--) {
    total += u[length] * v[length];
  }

  return total;
}

// -----------------------------------------------------------------------------
// Memory Management

AnmatStatus_t anmatMatrixAlloc(AnmatMatrix_t *matrix,
                               uint32_t rows,
                               uint32_t cols)
{
  AnmatStatus_t status = ANMAT_BAD_ARG;
  uint32_t rowI, colI;

  if (rows && cols) {
    status = ANMAT_SUCCESS;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (double **)heapAlloc(rows * sizeof(double *));
    if (!matrix->data) {
      status = ANMAT_MEM_ERR;
    }

    for (rowI = 0; rowI < rows && status == ANMAT_SUCCESS; rowI ++) {
      matrix->data[rowI] = (double *)heapAlloc(cols * sizeof(double));
      if (!matrix->data[rowI]) {
        status = ANMAT_MEM_ERR;
        anmatMatrixFree(matrix);
      } else {
        FOR_COL(matrix, colI) {
          matrix->data[rowI][colI] = 0;
        }
      }
    }
  }

  return status;
}

void anmatMatrixFree(AnmatMatrix_t *matrix)
{
  uint32_t rowI;

  for (rowI = 0; rowI < matrix->rows; rowI ++) {
    if (matrix->data[rowI]) {
      heapFree(matrix->data[rowI]);
    }
  }

  if (matrix->data) {
    heapFree(matrix->data);
  }
}

// -----------------------------------------------------------------------------
// Elementary operations

bool anmatMatrixEquals(AnmatMatrix_t *matrixA,
                       AnmatMatrix_t *matrixB)
{
  uint32_t rowI, colI;

  if (!dimensionsAreEqual(matrixA, matrixB)) {
    note("anmatMatrixEquals: dimensions are not equal.\n");
    return false;
  }

  FOR_ROW(matrixA, rowI) {
    FOR_COL(matrixA, colI) {
      if (!anmatUtilNeighborhood(matrixA->data[rowI][colI],
                                 matrixB->data[rowI][colI],
                                 1e-6)) {
        note("anmatMatrixEquals: values are not equal: ");
        note("(matrixA->data[%d][%d] = %lf) != (matrixB[%d][%d] = %lf)\n",
             rowI, colI, matrixA->data[rowI][colI],
             rowI, colI, matrixB->data[rowI][colI]);
        return false;
      }
    }
  }

  return true;
}

static AnmatStatus_t addOrSubtractMatrices(AnmatMatrix_t *matrixA,
                                           AnmatMatrix_t *matrixB,
                                           AnmatMatrix_t *matrixC,
                                           bool add)
{
  AnmatStatus_t status = ANMAT_BAD_ARG;
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

AnmatStatus_t anmatMatrixAdd(AnmatMatrix_t *matrixA,
                             AnmatMatrix_t *matrixB,
                             AnmatMatrix_t *matrixC)
{
  return addOrSubtractMatrices(matrixA, matrixB, matrixC, true); // add?
}

AnmatStatus_t anmatMatrixSubtract(AnmatMatrix_t *matrixA,
                                  AnmatMatrix_t *matrixB,
                                  AnmatMatrix_t *matrixC)
{
  return addOrSubtractMatrices(matrixA, matrixB, matrixC, false); // add?
}

AnmatStatus_t anmatMatrixMultiply(AnmatMatrix_t *matrixA,
                                  AnmatMatrix_t *matrixB,
                                  AnmatMatrix_t *matrixC)
{
  AnmatStatus_t status = ANMAT_BAD_ARG;
  AnmatMatrix_t matrixBT;
  uint32_t rowI, colI;

  if (matrixA->cols == matrixB->rows
      && matrixC->rows == matrixA->rows
      && matrixC->cols == matrixB->cols) {
    status = anmatMatrixAlloc(&matrixBT, matrixB->cols, matrixB->rows);
    if (status == ANMAT_SUCCESS) {
      status = anmatMatrixTranspose(matrixB, &matrixBT);
      if (status == ANMAT_SUCCESS) {
        FOR_ROW(matrixC, rowI) {
          FOR_COL(matrixC, colI) {
            matrixC->data[rowI][colI] = dotProduct(matrixA->data[rowI],
                                                   matrixBT.data[rowI],
                                                   matrixA->rows);
          }
        }
      }
      anmatMatrixFree(&matrixBT);
    }
  }

  return status;
}

// -----------------------------------------------------------------------------
// Matrix Operations

AnmatStatus_t anmatMatrixTranspose(AnmatMatrix_t *matrixA,
                                   AnmatMatrix_t *matrixB)
{
  AnmatStatus_t status = ANMAT_BAD_ARG;
  uint32_t rowI, colI;

  if (matrixA->rows == matrixB->cols && matrixA->cols == matrixB->rows) {
    status = ANMAT_SUCCESS;
    FOR_ROW(matrixA, rowI) {
      FOR_COL(matrixA, colI) {
        matrixB->data[colI][rowI] = matrixA->data[rowI][colI];
      }
    }
  }

  return status;
}

// -----------------------------------------------------------------------------
// I/O

AnmatStatus_t anmatMatrixPrint(AnmatMatrix_t *matrix,
                               FILE *stream)
{
  AnmatStatus_t status = ANMAT_SUCCESS;
  uint32_t rowI, colI;

  fprintf(stream, "{\n");
  FOR_ROW(matrix, rowI) {
    FOR_COL(matrix, colI) {
      fprintf(stream, " %06lf", matrix->data[rowI][colI]);
    }
    fprintf(stream, "\n");
  }

  fprintf(stream, "}\n");
  fflush(stream);

  return status;
}

static void appendValue(double value,
                        double **list,
                        uint32_t *pos,
                        uint32_t *listSize)
{
  if (*pos == *listSize) {
    double *newList = (double *)heapAlloc((*listSize <<= 1) * sizeof(double));
    anmatMemcpy(newList, *list, *pos * sizeof(double));
    heapFree(*list);
    *list = newList;
  }

  (*list)[*pos] = value;
  (*pos) += 1;
}

AnmatStatus_t anmatMatrixScan(AnmatMatrix_t *matrix,
                              FILE *stream)
{
  AnmatStatus_t status = ANMAT_SUCCESS;
  uint32_t pos, listSize;
  double value, *list;
  uint32_t rows, rowI, cols, colI;

  // Initialize the list.
  listSize = 5;
  pos = 0;
  list = heapAlloc(listSize * sizeof(double));
  if (!list) {
    status = ANMAT_MEM_ERR;
  }

  if (status == ANMAT_SUCCESS) {
    do {
      switch (fgetc(stream)) {
      case '{':
        colI = cols = rows = 0;
        break;
      case ' ':
        fscanf(stream, "%lf" , &value);
        appendValue(value, &list, &pos, &listSize);
        colI ++;
        break;
      case '\n':
        // If the dimensions are messed up, then we die.
        if (cols != 0 && colI != cols) {
          status = ANMAT_BAD_ARG;
          goto done;
        }
        cols = colI;
        colI = 0;
        break;
      case '}':
        goto done;
      default:
        status = ANMAT_BAD_ARG;
        goto done;
      }
    } while (1);
  }

 done:
  if (list) {
    if (status == ANMAT_SUCCESS) {
      rows = pos / cols;
      status = anmatMatrixAlloc(matrix, rows, cols);
      if (status == ANMAT_SUCCESS) {
        uint32_t i = 0;
        FOR_ROW(matrix, rowI) {
          FOR_COL(matrix, colI) {
            matrix->data[rowI][colI] = list[i++];
          }
        }
      }
    }
    heapFree(list);
  }

  return status;
}
