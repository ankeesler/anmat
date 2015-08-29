//
// util.c
//
// Andrew Keesler
//
// August 28, 2015
//
// Utilities functions.
//

#include "util.h"

// Find the max of two values.
inline double ANMAT_UtilMax(const double a,
                            const double b)
{
  return (a > b ? a : b);
}

// Find the absolute value of a number.
inline double ANMAT_UtilAbs(const double a)
{
  return (a > 0 ? a : (-1 * a));
}

// Find out whether two floating point values are within a certain amount of
// each other.
inline bool ANMAT_UtilNeighborhood(const double a,
                                   const double b,
                                   const double epsilon)
{
  return (ANMAT_UtilAbs(a - b) < epsilon);
}
