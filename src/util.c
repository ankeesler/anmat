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
double ANMAT_UtilMax(const double a,
                     const double b)
{
  return (a > b ? a : b);
}

// Find the absolute value of a number.
double ANMAT_UtilAbs(const double a)
{
  return (a > 0 ? a : (-1 * a));
}

// Find out whether two floating point values are within a certain amount of
// each other.
bool ANMAT_UtilNeighborhood(const double a,
                            const double b,
                            const double epsilon)
{
  return (ANMAT_UtilAbs(a - b) < epsilon);
}
