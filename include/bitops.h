#ifndef BITOPS_H_
#define BITOPS_H_

#include <limits.h>

#define BITS_IN(x) (CHAR_BIT * sizeof(x))

#define SH_LEFT(x,n) (((n) >= BITS_IN(x)) ? 0 : ((x) << (n)))
#define SH_RIGHT(x,n) (((n) >= BITS_IN(x)) ? 0 : ((x) >> (n)))

#endif // BITOPS_H_
