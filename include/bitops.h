/*
 * bitops.h: Bit operations used throughout the project.
 *           (Previously contained more, then I found out
 *           everything I had was already in stdint.h)
 */
#ifndef BITOPS_H_
#define BITOPS_H_

#include <limits.h>

/*
 * BITS_IN(x): Evaluates to the number of bits in x.
 */
#define BITS_IN(x) (CHAR_BIT * sizeof(x))

#endif // BITOPS_H_
