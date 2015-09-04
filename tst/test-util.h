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
#define expectHeapSize(size) (expectEquals(heapFreeBytesCount, (size)))
#define expectHeapFull()     (expectEquals(heapFreeBytesCount, 0))
#define expectHeapEmpty()    (expectEquals(heapFreeBytesCount, HEAP_SIZE))

// -----------------------------------------------------------------------------
// Util

#define expectNeighborhood(a, b, e) expect(anmatUtilNeighborhood(a, b, e))
