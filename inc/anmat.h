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
enum ANMAT_Status_t {
  // Success.
  ANMAT_SUCCESS       = 0,

  // Memory management error.
  ANMAT_MEM_ERR       = 1,

  // Bad argument.
  ANMAT_BAD_ARG       = 2,

};

// Matrix API.
#include "matrix.h"

#endif /* __ANMAT_H__ */
