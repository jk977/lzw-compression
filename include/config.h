/*
 * config.h: LZW-related configurations.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define LZW_MINIMUM_BITS 8
#define LZW_MAXIMUM_BITS 24
#define LZW_CHAR_RANGE 256

typedef int32_t code_t;

#endif // CONFIG_H_
