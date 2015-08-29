//
// heap.c
//
// Andrew Keesler
//
// August 14, 2015
//
// Heap management for anmath library.
//

#include "heap.h"

#include <stdlib.h>  // NULL
#include <stdint.h>  // uint8_t, uint16_t, uint32_t
#include <stdbool.h> // bool, true, false

//#define HEAP_DEBUG
#ifdef HEAP_DEBUG
  #define note(...) printf(__VA_ARGS__), fflush(0);
#else
  #define note(...)
#endif

// -----------------------------------------------------------------------------
// Definitions

// The heap size is defined by a compile time symbol in heap.h.

#define HEAP_SIZE (1 << ANMAT_HEAP_SIZE_LOG)

// There is 1 contiguous chunk of memory for the heap.

static uint8_t datHeapDoe[HEAP_SIZE];

// We keep track of each byte of the heap wth a bit. We mark the end of
// an allocation with a '0' bit.
// We add a byte on the end of the reference counts so it is easy and safe
// to find the end of the array.

#define REF_COUNTS_SIZE (HEAP_SIZE >> 3)
static uint8_t refCounts[REF_COUNTS_SIZE + 1];
#define REF_COUNTS_END (&refCounts[REF_COUNTS_SIZE])

#define BIT(i) (1UL << (i))

// We also keep track of free bytes for debugging/testing purposes.

uint32_t freeHeapBytes = HEAP_SIZE;

// -----------------------------------------------------------------------------
// API

void ANMAT_HeapInit(void)
{
  uint8_t *refCount = &refCounts[0];

  while (refCount != REF_COUNTS_END) {
    *refCount++ = 0x00;
  }

  freeHeapBytes = HEAP_SIZE;
}

static void markRefCounts(uint8_t *alloc, uint32_t count)
{
  long heapOffset         = alloc - &datHeapDoe[0];
  uint8_t *refCount       = &refCounts[heapOffset >> 3];
  uint16_t refCountsMask  = BIT(heapOffset & 0x7);
  
  note("ANMAT_HeapAlloc: marking heap from 0x%p to 0x%p\n",
       alloc, alloc + count);

  while (count --) {
    *refCount |= refCountsMask;
    freeHeapBytes -= 1;

    if ((refCountsMask <<= 1) == BIT(8)) {
      refCount ++;
      refCountsMask = BIT(0);
    }
  }

  // Mark an extra '0' bit for the end of this allocation.
  // This bit should already be cleared...
  *refCount &= ~refCountsMask;
  freeHeapBytes -= 1;
}

void *ANMAT_HeapAlloc(uint32_t count)
{
  uint8_t *alloc = NULL, *heapPos = NULL, *refCount = NULL;
  uint32_t  bitI;
  bool thisByteUsed, lastByteUsed;
  static bool heapInitialized = false;

  if (!heapInitialized) {
    ANMAT_HeapInit();
    heapInitialized = true;
  }

  note("ANMAT_HeapAlloc: allocating %d bytes\n", count + 1); // alloc byte

  if (count) {
    for (refCount = refCounts; refCount < REF_COUNTS_END; refCount++) {
      if (*refCount == 0xFF) {
        lastByteUsed = true;
        continue;
      }
      for (bitI = 0; bitI < 8; bitI ++) {
        thisByteUsed = *refCount & BIT(bitI);
        if (!thisByteUsed && !lastByteUsed) {
          heapPos = &(datHeapDoe[((refCount - &refCounts[0]) << 3) + bitI]);
          if (!alloc) {
            alloc = heapPos;
          } else if (heapPos - alloc == count) {
            markRefCounts(alloc, count);
            goto done;
          }
        } else {
          alloc = NULL;
        }
        lastByteUsed = thisByteUsed;
      }
    }
  }

 done:
  return alloc;
}

void ANMAT_HeapFree(void *memory)
{
  uint8_t *alloc          = (uint8_t *)memory; // stupid compiler grumble...
  long heapOffset         = alloc - &datHeapDoe[0];
  uint32_t refCountsIndex = heapOffset >> 3;
  uint16_t refCountsMask  = BIT(heapOffset & 0x7);

  if (alloc >= &datHeapDoe[0] && alloc < &datHeapDoe[HEAP_SIZE]) {
    while (refCounts[refCountsIndex] & refCountsMask) {
      refCounts[refCountsIndex] &= ~refCountsMask;
      freeHeapBytes ++;

      if ((refCountsMask <<= 1) == BIT(8)) {
        refCountsIndex ++;
        refCountsMask = BIT(0);
      }
    }
    freeHeapBytes ++; // for the extra '0' bit at the end of the allocation
  }
}

void ANMAT_HeapPrint(FILE *stream)
{
  uint8_t *refCount, *alloc;
  uint32_t refCountMask;

  for (refCount = &refCounts[0], refCountMask = BIT(0), alloc = &datHeapDoe[0];
       refCount != REF_COUNTS_END;
       refCount ++, alloc ++) {
    if (refCountMask == BIT(0)) {
      fprintf(stream, "(%ld) ", refCount - &refCounts[0]);
    }
    if (*refCount & refCountMask) {
      fprintf(stream, " 0x%02X", *alloc);
    } else {
      fprintf(stream, " ____");
    }
    if ((refCountMask <<= 1) == BIT(8)) {
      refCountMask = BIT(0);
      fprintf(stream, "\n");
    }
  }

  fflush(stream);
}
