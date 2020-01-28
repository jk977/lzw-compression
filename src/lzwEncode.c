#include <sequence.h>
#include <bitStream.h>
#include <dict.h>
#include <lzw.h>

bool lzwEncode(unsigned int bits, unsigned int maxBits,
               int (*readFunc)(void* context),
               void (*writeFunc)(unsigned char c, void* context),
               void* context) {
    return true;
}
