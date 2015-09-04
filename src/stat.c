//
// stat.c
//
// Andrew Keesler
//
// August 22, 2015
//
// Statistics API.
//

#include "stat.h"
#include "src/heap.h"

// -----------------------------------------------------------------------------
// Private Stuff

#define FOR_VALUE(vector, valueI)                       \
  for (valueI = 0; valueI < (vector)->count; valueI ++)

// -----------------------------------------------------------------------------
// Memory Management

// Allocate a vector.
AnmatStatus_t anmatVectorAlloc(AnmatVector_t *vector,
                                uint32_t count)
{
  AnmatStatus_t status = ANMAT_BAD_ARG;

  if (count) {
    vector->data = (double *)heapAlloc(count * sizeof(double));
    status = (vector->data ? ANMAT_SUCCESS : ANMAT_MEM_ERR);
    vector->count = count;
  }

  return status;
}

// Free a vector.
void anmatVectorFree(AnmatVector_t *vector)
{
  if (vector->data) {
    heapFree(vector->data);
  }
}

// -----------------------------------------------------------------------------
// Elementary Operations

double anmatStatAverage(AnmatVector_t *vector)
{
  double total = 0;
  uint32_t valueI;

  FOR_VALUE(vector, valueI) {
    total += vector->data[valueI];
  }

  return (total / vector->count);
}
