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

#include <stdbool.h>

// Find the max of two values.
double ANMAT_UtilMax(const double a,
                     const double b);

// Find the absolute value of a number.
double ANMAT_UtilAbs(const double a);

// Find out whether two floating point values are within a certain amount of
// each other.
bool ANMAT_UtilNeighborhood(const double a,
                            const double b,
                            const double epsilon);

#endif /* __UTIL_H__ */
