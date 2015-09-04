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

#include "anmat.h"

// -----------------------------------------------------------------------------
// Structs

// An m x n matrix.
typedef struct {
  uint32_t rows, cols;
  double **data;
} AnmatMatrix_t;

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a matrix with a number of rows and a number of cols.
AnmatStatus_t anmatMatrixAlloc(AnmatMatrix_t *matrix,
                               uint32_t rows,
                               uint32_t cols);

// Free a matrix from the heap.
void anmatMatrixFree(AnmatMatrix_t *matrix);

// -----------------------------------------------------------------------------
// Data Access

// Get count of rows and cols.
#define anmatMatrixRowCount(matrix) ((matrix)->rows)
#define anmatMatrixColCount(matrix) ((matrix)->cols)

// Get pointer to the m'th row.
#define anmatMatrixRow(matrix, m) ((matrix)->data[m])

// Get the value on the m'th row and the n'th column.
#define anmatMatrixData(matrix, m, n) ((matrix)->data[m][n])

// -----------------------------------------------------------------------------
// Elementary operations

// Returns true iff matrixA is equal to matrixB.
bool anmatMatrixEquals(AnmatMatrix_t *matrixA,
                       AnmatMatrix_t *matrixB);

// Add matrixA and matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t anmatMatrixAdd(AnmatMatrix_t *matrixA,
                             AnmatMatrix_t *matrixB,
                             AnmatMatrix_t *matrixC);

// Subtract matrixB from matrixA and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t anmatMatrixSubtract(AnmatMatrix_t *matrixA,
                                  AnmatMatrix_t *matrixB,
                                  AnmatMatrix_t *matrixC);

// Multiply matrixA by matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t anmatMatrixMultiply(AnmatMatrix_t *matrixA,
                                  AnmatMatrix_t *matrixB,
                                  AnmatMatrix_t *matrixC);

// -----------------------------------------------------------------------------
// Matrix Operations

// Build the transpose of matrixA in matrixB.
// The matrixB must already be allocated.
AnmatStatus_t anmatMatrixTranspose(AnmatMatrix_t *matrixA,
                                   AnmatMatrix_t *matrixB);

// -----------------------------------------------------------------------------
// I/O

// Print a matrix to a stream.
AnmatStatus_t anmatMatrixPrint(AnmatMatrix_t *matrix,
                               FILE *stream);

// Scan a matrix from a stream.
// The matrix will be allocated for the user.
AnmatStatus_t anmatMatrixScan(AnmatMatrix_t *matrix,
                              FILE *stream);

#endif /* __MATRIX_H__ */
