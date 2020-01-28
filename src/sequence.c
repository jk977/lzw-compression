#include <stdlib.h>
#include <sequence.h>

Sequence* newSequence(unsigned char firstByte, unsigned int hashSize) {
    return NULL;
}

void deleteSequence(Sequence* sequence) {
}

Sequence* copySequenceAppend(Sequence* sequence, unsigned char addByte, unsigned int hashSize) {
    return NULL;
}

void outputSequence(Sequence* sequence,
                    void (*writeFunc)(unsigned char c, void* context), void* context) {
}

bool identicalSequences(Sequence* a, Sequence* b) {
    return true;
}
