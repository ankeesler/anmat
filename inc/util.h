//
// util.h
//
// Andrew Keesler
//
// August 28, 2015
//
// Utilities functions.
//

#ifndef __UTIL_H__
#define __UTIL_H__

#include "anmat.h"

// -----------------------------------------------------------------------------
// IEEE 754 Floating Point Definitions

#define ANMAT_IEEE_754_SIGN_OFFSET   (63)
#define ANMAT_IEEE_754_SIGN_MASK     (1UL << ANMAT_IEEE_754_SIGN_OFFSET)
#define ANMAT_IEEE_754_SIGN_POSITIVE (0UL << ANMAT_IEEE_754_SIGN_OFFSET)
#define ANMAT_IEEE_754_SIGN_NEGATIVE (1UL << ANMAT_IEEE_754_SIGN_OFFSET)

#define anmatUtilIsPositive(a)                      \
  ((((uint64_t)(a)) & ANMAT_IEEE_754_SIGN_MASK) \
   == ANMAT_IEEE_754_SIGN_POSITIVE)
#define anmatUtilIsNegative(a)                      \
  ((((uint64_t)(a)) & ANMAT_IEEE_754_SIGN_MASK) \
   == ANMAT_IEEE_754_SIGN_NEGATIVE)

// -----------------------------------------------------------------------------
// Elementary Memory Memory Manipulation

// Copy length bytes from source to destination.
void anmatMemcpy(void *destination,
                 void *source,
                 unsigned int length);

// -----------------------------------------------------------------------------
// Elementary Math Functions

// Find the max of two values.
double anmatUtilMax(double a,
                    double b);

// Find the absolute value of a number.
double anmatUtilAbs(double a);

// Find out whether two floating point values are within a certain amount of
// each other.
bool anmatUtilNeighborhood(double a,
                           double b,
                           double epsilon);

// Find base raised to the power.
double anmatUtilPower(double base,
                      int power);

// Find the r'th root of a.
double anmatUtilRoot(double a,
                     unsigned int r);

#endif /* __UTIL_H__ */
