//
// matrix.h
//
// Andrew Keesler
//
// August 14, 2015
//
// Matrix API.
//

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdint.h>
#include <stdio.h>

#include "anmat.h"

// -----------------------------------------------------------------------------
// Structs

// An m x n matrix.
struct ANMAT_Matrix_t {
  uint32_t rows, cols;
  double **data;
};

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a matrix with a number of rows and a number of cols.
// Returns NULL if the memory cannot be allocated or rows or cols is 0.
enum ANMAT_Status_t ANMAT_MatrixAlloc(struct ANMAT_Matrix_t *matrix,
                                      uint32_t rows,
                                      uint32_t cols);

// Free a matrix from the heap.
void ANMAT_MatrixFree(struct ANMAT_Matrix_t *matrix);

// -----------------------------------------------------------------------------
// Data Access

// Get count of rows and cols.
#define ANMAT_MatrixRowCount(matrix) ((matrix)->rows)
#define ANMAT_MatrixColCount(matrix) ((matrix)->cols)

// Get pointer to the m'th row.
#define ANMAT_MatrixRow(matrix, m) ((matrix)->data[m])

// Get the value on the m'th row and the n'th column.
#define ANMAT_MatrixData(matrix, m, n) ((matrix)->data[m][n])

// -----------------------------------------------------------------------------
// Elementary operations

// Add matrix A and matrix B and put the result inside matrixC.
// The matrixC must already be allocated.
enum ANMAT_Status_t ANMAT_MatrixAdd(struct ANMAT_Matrix_t *matrixA,
                                    struct ANMAT_Matrix_t *matrixB,
                                    struct ANMAT_Matrix_t *matrixC);

// -----------------------------------------------------------------------------
// I/O

// Print a matrix to a stream.
enum ANMAT_Status_t ANMAT_MatrixPrint(struct ANMAT_Matrix_t *matrix,
                                      FILE *stream);

// Scan a matrix from a stream.
enum ANMAT_Status_t ANMAT_MatrixScan(struct ANMAT_Matrix_t *matrix,
                                     FILE *stream);

#endif /* __MATRIX_H__ */
