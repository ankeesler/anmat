//
// stat.h
//
// Andrew Keesler
//
// August 22, 2015
//
// Statistics API.
//

#include <string.h> // memset()

#include "anmat.h"

// -----------------------------------------------------------------------------
// Structs

typedef struct {
  uint32_t count;
  double *data;
} AnmatVector_t;

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a vector.
AnmatStatus_t ANMAT_VectorAlloc(AnmatVector_t *vector,
                                uint32_t count);

// Free a vector.
void ANMAT_VectorFree(AnmatVector_t *vector);

// -----------------------------------------------------------------------------
// Data Access

// Get count.
#define ANMAT_VectorCount(vector) ((vector)->count)

// Get the n'th value in the data.
#define ANMAT_VectorData(vector, n) ((vector)->data[n])

// -----------------------------------------------------------------------------
// Elementary Operations

// Calculate the average of the data in the vector.
double ANMAT_StatAverage(AnmatVector_t *vector);
