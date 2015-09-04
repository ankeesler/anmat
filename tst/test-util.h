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
#define expectHeapEmpty()    (expectEquals(heapFreeBytesCount, 0))
#define expectHeapFull()     (expectEquals(heapFreeBytesCount, HEAP_SIZE))
