//
// test-util.h
//
// Andrew Keesler
//
// August 17, 2015
//
// Testing utilities.
//

// -----------------------------------------------------------------------------
// Heap

#include "src/heap.h"

#define HEAP_SIZE (1 << ANMAT_HEAP_SIZE_LOG)
#define expectHeapSize(size) (expectEquals(freeHeapBytes, (size)))
#define expectHeapEmpty()    (expectEquals(freeHeapBytes, 0))
#define expectHeapFull()     (expectEquals(freeHeapBytes, HEAP_SIZE))
