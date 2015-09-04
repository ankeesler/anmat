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
// Elementary Math Functions

// Find the max of two values.
double anmatUtilMax(const double a,
                    const double b);

// Find the absolute value of a number.
double anmatUtilAbs(const double a);

// Find out whether two floating point values are within a certain amount of
// each other.
bool anmatUtilNeighborhood(const double a,
                           const double b,
                           const double epsilon);

// Find a raised to the power p.
double anmatUtilPower(double a,
                      uint32_t p);

// Find the r'th root of a.
double anmatUtilRoot(double a,
                     uint32_t r);

#endif /* __UTIL_H__ */
