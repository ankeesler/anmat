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

// -----------------------------------------------------------------------------
// Elementary Memory Manipulation

// Copy length bytes from source to destination.
void anmatMemcpy(void *destination,
                 void *source,
                 unsigned int length)
{
  unsigned char *destByte, *sourceByte;

  // If the source overlapps the destination from the bottom, we
  // copy from the end. Else, we can copy normally.
  if (source < destination) {
    for (destByte = destination + length - 1, sourceByte = source + length - 1;
         length --;
         *destByte-- = *sourceByte--) { }
  } else {
    for (destByte = destination, sourceByte = source;
         length --;
         *destByte++ = *sourceByte++) { }
  }
}

// -----------------------------------------------------------------------------
// Elementary Math Functions

// Find the max of two values.
inline double anmatUtilMax(double a,
                           double b)
{
  return (a > b ? a : b);
}

// Find the absolute value of a number.
inline double anmatUtilAbs(double a)
{
  return (a > 0 ? a : (-1 * a));
}

// Find out whether two floating point values are within a certain amount of
// each other.
inline bool anmatUtilNeighborhood(double a,
                                  double b,
                                  double epsilon)
{
  return (anmatUtilAbs(a - b) <= epsilon);
}

// Find a raised to the power p.
double anmatUtilPower(double a,
                      unsigned int p)
{
  double value = 0;

  if (!p) {
    value = 1;
  } else if (p > 0) {
    value = 1;
    while (p --) { value *= a; }
  }

  return value;
}

// Find the r'th root of a.
double anmatUtilRoot(double a,
                     unsigned int r)
{
  // We are trying to find x in x^r = a.
  double x = 0;

  return x;
}
