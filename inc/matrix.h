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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

// Returns true iff matrixA is equal to matrixB.
bool ANMAT_MatrixEquals(struct ANMAT_Matrix_t *matrixA,
                        struct ANMAT_Matrix_t *matrixB);

// Add matrixA and matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
enum ANMAT_Status_t ANMAT_MatrixAdd(struct ANMAT_Matrix_t *matrixA,
                                    struct ANMAT_Matrix_t *matrixB,
                                    struct ANMAT_Matrix_t *matrixC);

// Subtract matrixB from matrixA and put the result inside matrixC.
// The matrixC must already be allocated.
enum ANMAT_Status_t ANMAT_MatrixSubtract(struct ANMAT_Matrix_t *matrixA,
                                         struct ANMAT_Matrix_t *matrixB,
                                         struct ANMAT_Matrix_t *matrixC);

// Multiply matrixA by matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
enum ANMAT_Status_t ANMAT_MatrixMultiply(struct ANMAT_Matrix_t *matrixA,
                                         struct ANMAT_Matrix_t *matrixB,
                                         struct ANMAT_Matrix_t *matrixC);

// -----------------------------------------------------------------------------
// Matrix Operations

// Build the transpose of matrixA in matrixB.
// The matrixB must already be allocated.
enum ANMAT_Status_t ANMAT_MatrixTranspose(struct ANMAT_Matrix_t *matrixA,
                                          struct ANMAT_Matrix_t *matrixB);

// -----------------------------------------------------------------------------
// I/O

// Print a matrix to a stream.
enum ANMAT_Status_t ANMAT_MatrixPrint(struct ANMAT_Matrix_t *matrix,
                                      FILE *stream);

// Scan a matrix from a stream.
// The matrix will be allocated for the user.
enum ANMAT_Status_t ANMAT_MatrixScan(struct ANMAT_Matrix_t *matrix,
                                     FILE *stream);

#endif /* __MATRIX_H__ */
