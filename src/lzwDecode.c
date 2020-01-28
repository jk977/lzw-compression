#include <sequence.h>
#include <bitStream.h>
#include <lzw.h>

bool lzwDecode(unsigned int bits, unsigned int maxBits,
               int  (*readFunc )(void* context),
               void (*writeFunc)(unsigned char c, void* context),
               void* context) {
    return true;
}
