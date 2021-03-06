//
// anmat.h
//
// Andrew Keesler
//
// A math library.
//

#ifndef __ANMAT_H__
#define __ANMAT_H__

// Standard C definitions.
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Status codes.
typedef enum {
  // Success.
  ANMAT_SUCCESS       = 0,

  // Memory management error.
  ANMAT_MEM_ERR       = 1,

  // Bad argument.
  ANMAT_BAD_ARG       = 2,

} AnmatStatus_t;

// Default epsilon definition.
// TODO: what should this really be?
#define ANMAT_EPSILON_DEFAULT (1e-6)

// How many times the square root algorithm should iterate.
#define ANMAT_ROOT_MAX_ITERATIONS 32

// Utilities API.
#include "util.h"

// Matrix API.
#include "matrix.h"

// Statistics API.
#include "stat.h"

#endif /* __ANMAT_H__ */
