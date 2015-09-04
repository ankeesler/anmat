//
// heap.h
//
// Andrew Keesler
//
// August 14, 2015
//
// Heap management for anmat library.
//

#include <stdint.h>
#include <stdio.h>

// The log base 2 size of the heap.
#define ANMAT_HEAP_SIZE_LOG (12)

// Initialize the heap.
// Note that the heap is automagically initliazed
// with the first call to heapAlloc.
void heapInit(void);

// Allocate bytes.
// Returns NULL on failure.
// The heap is automagically initialized the first time this is called.
// Not thread safe.
void *heapAlloc(uint32_t count);

// Free memory.
// Not thread safe.
void heapFree(void *memory);

// The number of free bytes in the heap.
// Useful for debugging.
extern uint32_t heapFreeBytesCount;

// Print the heap.
// Useful for debugging.
void heapPrint(FILE *stream);
