//
// anmat.h
//
// Andrew Keesler
//
// A math library.
//

#ifndef __ANMAT_H__
#define __ANMAT_H__

// Status codes.
typedef enum {
  // Success.
  ANMAT_SUCCESS       = 0,

  // Memory management error.
  ANMAT_MEM_ERR       = 1,

  // Bad argument.
  ANMAT_BAD_ARG       = 2,

} AnmatStatus_t;

// Matrix API.
#include "matrix.h"

#endif /* __ANMAT_H__ */
