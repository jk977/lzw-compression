#include <stdlib.h>
#include <bitStream.h>

BitStream* openInputBitStream(int (*readFunc)(void* context), void* context) {
    return NULL;
}

BitStream* openOutputBitStream(void (*writeFunc)(unsigned char c,void* context),void* context) {
    return NULL;
}

void closeAndDeleteBitStream(BitStream* bs) {
}

void outputBits(BitStream* bs, unsigned int nBits, unsigned int code) {
}

bool readInBits(BitStream* bs, unsigned int nBits, unsigned int* code) {
    return true;
}
