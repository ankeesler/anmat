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

//#define UTIL_DEBUG
#ifdef UTIL_DEBUG
  #define note(...) printf(__VA_ARGS__), fflush(0);
#else
  #define note(...)
#endif

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

// Find base raised to the power.
double anmatUtilPower(double base,
                      int power)
{
  double value = 1;

  if (anmatUtilNeighborhood(base, 0, ANMAT_EPSILON_DEFAULT) && power != 0) {
    value = 0;
  } else {
    if (power > 0) {
      while (power --) { value *= base; }
    } else if (power < 0) {
      while (power ++) { value /= base; }
    }
  }

  return value;
}

// Find the r'th root of a.
double anmatUtilRoot(double aReal,
                     unsigned int root,
                     double epsilon)
{
  // We are trying to find the positive root of x^root - aReal = 0.
  // Let's use Newton's Method.
  double baseGuess, aGuess, y, m, x, b;
  unsigned int tries;

  // TODO: what should this really be?
  baseGuess = aReal / 2.0;

  tries = ANMAT_ROOT_MAX_ITERATIONS;

  note("Finding the %d'th root of %lf within %lf and with %d tries\n",
       root, aReal, epsilon, tries);

  while (tries--) {
    aGuess = anmatUtilPower(baseGuess, root) - aReal;
    if (anmatUtilNeighborhood(aGuess, 0, epsilon)) {
      break;
    }

    // The derivative of our function is root * x^(root - 1).
    m = root * anmatUtilPower(baseGuess, root - 1);
    y = aGuess;
    x = baseGuess;

    // Find b in y = mx + b.
    b = y - (m * x);

    // Find the root of y = mx + b.
    x = -b / m;

    baseGuess = x;
    note("  aGuess = %lf, y = %lf, m = %lf, x = %lf, b = %lf, baseGuess = %lf\n",
         aGuess, y, m, x, b, baseGuess);
  }

  return baseGuess;
}
