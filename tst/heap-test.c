//
// heap-test.c
//
// Andrew Keesler
//
// August 14, 2015
//
// Heap management test.
//

#include <unit-test.h>

#include "src/heap.h"

#include "./test-util.h"

static int doubleTest(void)
{
  double *pointers[2] = {NULL,NULL,};

  // At the beginning of allocation, we should have all of the bytes.
  expectHeapEmpty();

  // Can't allocate 0 bytes!
  pointers[0] = heapAlloc(0);
  expect(pointers[0] == NULL);

  // The number of free bytes should still be the total heap.
  expectHeapEmpty();

  // Allocating 1 double (8 bytes) should be no problem.
  // The heap should automatically initialize itself the first time we alloc.
  pointers[0] = (double *)heapAlloc(1 * sizeof(double));
  expect(pointers[0] != NULL);
  pointers[0][0] = 12.345;
  expect(*pointers[0] == 12.345);

  // We should have 9 bytes missing from the heap, since allocations need one
  // extra byte for length purposes.
  expectHeapSize(HEAP_SIZE - 9);

  // If we free the memory, we should get back all of our bytes.
  heapFree(pointers[0]);
  expectHeapEmpty();

  // How about if we free one 16 byte chunk and one 24 byte chunk.
  pointers[0] = NULL;
  pointers[0] = (double *)heapAlloc(2 * sizeof(double));
  expect(pointers[0] != NULL);
  pointers[1] = (double *)heapAlloc(3 * sizeof(double));
  expect(pointers[1] != NULL);

  // We should have 16 + 1 + 24 + 1 bytes missing.
  expectHeapSize(HEAP_SIZE - 16 - 1 - 24 - 1);

  // If we free the 24 byte chunk, we should have only 16 + 1 bytes missing.
  heapFree(pointers[1]);
  expectHeapSize(HEAP_SIZE - 16 - 1);

  // After we free the 16 byte chunk, we should be back to 0 bytes missing.
  heapFree(pointers[0]);
  expectHeapEmpty();

  return 0;
}

static int stressTest(void)
{
  uint8_t *pointers[3] = {NULL, NULL, NULL,};

  // Initializing the heap should mean all the bytes are available.
  heapInit();
  expectHeapEmpty();
  
  // Let's allocate the whole heap with two chunks. One will be
  // (HEAP_SIZE / 4 - 1) bytes and the other will be
  // ((3 * HEAP_SIZE / 4) - 1) bytes.
  pointers[0] = (uint8_t *)heapAlloc((HEAP_SIZE / 4) - 1); 
  expect(pointers[0] != NULL);
  pointers[1] = (uint8_t *)heapAlloc(((3 * HEAP_SIZE) / 4) - 1); 
  expect(pointers[1] != NULL);

  // There should be no bytes left.
  expectHeapFull();

  // Therefore, we should not be able to allocate anymore.
  pointers[2] = (uint8_t *)heapAlloc(1);
  expect(pointers[2] == NULL);

  // If we free the first guy, we should only be missing the second big chunk.
  heapFree(pointers[0]);
  expectHeapSize(HEAP_SIZE - (((3 * HEAP_SIZE) / 4) - 1) - 1);
  
  // If we try to allocate some memory that is equal to the bytes left,
  // then we should fail, because we need any extra byte to mark the end of
  // the reference.
  pointers[2] = heapAlloc(heapFreeBytesCount);
  expect(pointers[2] == NULL);

  // Make sure when we free the second chunk, we have a full heap available.
  heapFree(pointers[1]);
  expectHeapEmpty();

  return 0;
}

static int structTest(void)
{
  struct SmallTestStruct_t {
    uint16_t member1;
    uint16_t member2;
    uint32_t member3;
  };
  struct LargeTestStruct_t {
    uint32_t member1;
    uint32_t member2;
    uint32_t member3;
    uint32_t member4;
  };

  // Make sure the small test struct warrants a small buffer (8 bytes)
  // and the large struct warrants a large buffer (32 bytes).
  expectEquals(sizeof(struct SmallTestStruct_t), 8);
  expectEquals(sizeof(struct LargeTestStruct_t), 16);

  heapInit();  

  // Allocate memory for a small struct and make sure it functions properly.
  struct SmallTestStruct_t *smallStructPointer
    = (struct SmallTestStruct_t *)
      heapAlloc(sizeof(struct SmallTestStruct_t));
  struct SmallTestStruct_t smallStruct;
  expect(smallStructPointer != NULL);
  smallStructPointer->member1 = 1;
  smallStructPointer->member2 = 2;
  smallStructPointer->member3 = 3;
  expectEquals(smallStructPointer->member1, 1);
  expectEquals(smallStructPointer->member2, 2);
  expectEquals(smallStructPointer->member3, 3);
  memcpy(&smallStruct, smallStructPointer, sizeof(struct SmallTestStruct_t));
  expectEquals(smallStruct.member1, 1);
  expectEquals(smallStruct.member2, 2);
  expectEquals(smallStruct.member3, 3);

  // Same thing with a large struct.
  struct LargeTestStruct_t *largeStructPointer
    = (struct LargeTestStruct_t *)
      heapAlloc(sizeof(struct LargeTestStruct_t));
  struct LargeTestStruct_t largeStruct;
  expect(largeStructPointer != NULL);
  largeStructPointer->member1 = 1;
  largeStructPointer->member2 = 2;
  largeStructPointer->member3 = 3;
  largeStructPointer->member4 = 4;
  expectEquals(largeStructPointer->member1, 1);
  expectEquals(largeStructPointer->member2, 2);
  expectEquals(largeStructPointer->member3, 3);
  expectEquals(largeStructPointer->member4, 4);
  memcpy(&largeStruct, largeStructPointer, sizeof(struct LargeTestStruct_t));
  expectEquals(largeStruct.member1, 1);
  expectEquals(largeStruct.member2, 2);
  expectEquals(largeStruct.member3, 3);
  expectEquals(largeStruct.member4, 4);

  return 0;
}

static int arrayTest(void)
{
  int **matrix, i;

  heapInit();
  expectHeapEmpty();

  // Allocate room for 3 pointers.
  matrix = (int **)heapAlloc(3 * sizeof(int *));
  expectHeapSize(HEAP_SIZE
                 - (3 * (sizeof(int *))) - 1 // 3 pointers and 1 alloc byte
                 - 0);
  
  // Allocate 5 integers in each array.
  for (i = 0; i < 3; i ++) {
    matrix[i] = (int *)heapAlloc(5 * sizeof(int));
  }
  expectHeapSize(HEAP_SIZE
                 // 3 pointers and 1 alloc byte
                 - (3 * (sizeof(int *))) - 1
                 // 3 arrays of 5 int's plus an alloc byte each
                 - 3 * ((5 * sizeof(int)) + 1) 
                 - 0);

  // Free the arrays.
  for (i = 0; i < 3; i ++) {
    heapFree(matrix[i]);
  }
  expectHeapSize(HEAP_SIZE
                 - (3 * (sizeof(int *))) - 1 // 3 pointers and 1 alloc byte
                 - 0);
  
  // Free the pointers.
  heapFree(matrix);
  expectHeapEmpty();

  return 0;
}

int main(void)
{
  announce();

  run(doubleTest);
  run(stressTest);
  run(structTest);
  run(arrayTest);

  return 0;
}
