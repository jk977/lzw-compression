#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <stdbool.h>

typedef struct _sequence {
    int placeHolder;    // replace this line with your desired contents
} Sequence;

Sequence* newSequence(unsigned char firstByte, unsigned int hashSize);

void deleteSequence(Sequence* sequence);

Sequence* copySequenceAppend(Sequence* sequence,
                             unsigned char addByte, 
							 unsigned int hashSize);

void outputSequence(Sequence* sequence,
                    void (*writeFunc)(unsigned char c, void* context),
				   	void* context);

bool identicalSequences(Sequence* a, Sequence* b);

#endif
