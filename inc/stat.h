//
// stat.h
//
// Andrew Keesler
//
// August 22, 2015
//
// Statistics API.
//

#ifndef __STAT_H__
#define __STAT_H__

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
AnmatStatus_t anmatVectorAlloc(AnmatVector_t *vector,
                               uint32_t count);

// Free a vector.
void anmatVectorFree(AnmatVector_t *vector);

// -----------------------------------------------------------------------------
// Data Access

// Get count.
#define anmatVectorCount(vector) ((vector)->count)

// Get the n'th value in the data.
#define anmatVectorData(vector, n) ((vector)->data[n])

// -----------------------------------------------------------------------------
// Elementary Operations

// Calculate the average of the data in the vector.
double anmatStatAverage(AnmatVector_t *vector);

#endif /* __STAT_H__ */
