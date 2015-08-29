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

struct ANMAT_Vector_t {
  uint32_t count;
  double *data;
};

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a vector.
enum ANMAT_Status_t ANMAT_VectorAlloc(struct ANMAT_Vector_t *vector,
                                      uint32_t count);

// Free a vector.
void ANMAT_VectorFree(struct ANMAT_Vector_t *vector);

// -----------------------------------------------------------------------------
// Data Access

// Get count.
#define ANMAT_VectorCount(vector) ((vector)->count)

// Get the n'th value in the data.
#define ANMAT_VectorData(vector, n) ((vector)->data[n])

// -----------------------------------------------------------------------------
// Elementary Operations

// Calculate the average of the data in the vector.
double ANMAT_StatAverage(struct ANMAT_Vector_t *vector);
