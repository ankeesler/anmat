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
typedef struct {
  uint32_t rows, cols;
  double **data;
} AnmatMatrix_t;

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a matrix with a number of rows and a number of cols.
AnmatStatus_t ANMAT_MatrixAlloc(AnmatMatrix_t *matrix,
                                uint32_t rows,
                                uint32_t cols);

// Free a matrix from the heap.
void ANMAT_MatrixFree(AnmatMatrix_t *matrix);

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
bool ANMAT_MatrixEquals(AnmatMatrix_t *matrixA,
                        AnmatMatrix_t *matrixB);

// Add matrixA and matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t ANMAT_MatrixAdd(AnmatMatrix_t *matrixA,
                              AnmatMatrix_t *matrixB,
                              AnmatMatrix_t *matrixC);

// Subtract matrixB from matrixA and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t ANMAT_MatrixSubtract(AnmatMatrix_t *matrixA,
                                   AnmatMatrix_t *matrixB,
                                   AnmatMatrix_t *matrixC);

// Multiply matrixA by matrixB and put the result inside matrixC.
// The matrixC must already be allocated.
AnmatStatus_t ANMAT_MatrixMultiply(AnmatMatrix_t *matrixA,
                                   AnmatMatrix_t *matrixB,
                                   AnmatMatrix_t *matrixC);

// -----------------------------------------------------------------------------
// Matrix Operations

// Build the transpose of matrixA in matrixB.
// The matrixB must already be allocated.
AnmatStatus_t ANMAT_MatrixTranspose(AnmatMatrix_t *matrixA,
                                    AnmatMatrix_t *matrixB);

// -----------------------------------------------------------------------------
// I/O

// Print a matrix to a stream.
AnmatStatus_t ANMAT_MatrixPrint(AnmatMatrix_t *matrix,
                                FILE *stream);

// Scan a matrix from a stream.
// The matrix will be allocated for the user.
AnmatStatus_t ANMAT_MatrixScan(AnmatMatrix_t *matrix,
                               FILE *stream);

#endif /* __MATRIX_H__ */
