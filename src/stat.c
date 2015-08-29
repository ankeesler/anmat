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
enum ANMAT_Status_t ANMAT_VectorAlloc(struct ANMAT_Vector_t *vector,
                                      uint32_t count)
{
  enum ANMAT_Status_t status = ANMAT_BAD_ARG;

  if (count) {
    vector->data = (double *)ANMAT_HeapAlloc(count * sizeof(double));
    status = (vector->data ? ANMAT_SUCCESS : ANMAT_MEM_ERR);
    vector->count = count;
  }

  return status;
}

// Free a vector.
void ANMAT_VectorFree(struct ANMAT_Vector_t *vector)
{
  if (vector->data) {
    ANMAT_HeapFree(vector->data);
  }
}

// -----------------------------------------------------------------------------
// Elementary Operations

double ANMAT_StatAverage(struct ANMAT_Vector_t *vector)
{
  double total = 0;
  uint32_t valueI;

  FOR_VALUE(vector, valueI) {
    total += vector->data[valueI];
  }

  return (total / vector->count);
}
