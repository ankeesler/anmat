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
inline double anmatUtilMax(const double a,
                           const double b)
{
  return (a > b ? a : b);
}

// Find the absolute value of a number.
inline double anmatUtilAbs(const double a)
{
  return (a > 0 ? a : (-1 * a));
}

// Find out whether two floating point values are within a certain amount of
// each other.
inline bool anmatUtilNeighborhood(const double a,
                                  const double b,
                                  const double epsilon)
{
  return (anmatUtilAbs(a - b) <= epsilon);
}

// Find a raised to the power p.
double anmatUtilPower(double a,
                      uint32_t p)
{
  double value = 0;

  return value;
}

// Find the r'th root of a.
double anmatUtilRoot(double a,
                     uint32_t r)
{
  // We are trying to find x in x^r = a.
  double x = 0;

  return x;
}
