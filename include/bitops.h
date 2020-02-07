#ifndef BITOPS_H_
#define BITOPS_H_

#include <limits.h>

/*
 * BITS_IN(x): Evaluates to the number of bits in x.
 */
#define BITS_IN(x) (CHAR_BIT * sizeof(x))

/*
 * SH_LEFT(x,n): Shifts x to the left n bits, evaluating to
 *               0 when an overflow would occur.
 *
 * SH_RIGHT(x,n): Same behavior as SH_LEFT(x,n) except shifts right.
 */
#define SH_LEFT(x,n)    (((n) >= BITS_IN(x)) ? 0 : ((x) << (n)))
#define SH_RIGHT(x,n)   (((n) >= BITS_IN(x)) ? 0 : ((x) >> (n)))

#endif // BITOPS_H_
