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
  return (ANMAT_UtilAbs(a - b) <= epsilon);
}

// Find a raised to the power p.
double ANMAT_UtilPower(double a,
                       uint32_t p)
{
  double value = 0;

  return value;
}

// Find the r'th root of a.
double ANMAT_UtilRoot(double a,
                      uint32_t r)
{
  // We are trying to find x in x^r = a.
  double x = 0;

  return x;
}
