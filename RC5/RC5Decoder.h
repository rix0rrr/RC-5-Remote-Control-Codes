#pragma once

#include "IWordConsumer.h"

// The width of a single bit (1778us)
#define BIT_TIME_S 1778E-6

// The minimum interval between two bits (slightly more than half
// of the bit frequency)
#define TRANSITION_INTERVAL_S (0.6 * BIT_TIME_S)

// The distance between repeat words
#define WORD_INTERVAL_S 30E-3

// The length of a word (including header bits etc.)
#define WORD_LENGTH_BITS 14

#define TOGGLE_BIT 2

#define FIELD_BIT 1

/**
 * RC5 Decoder
 *
 * Works by ignoring transitions that are too close together.
 * (Fewer than 1000 usec, as defined by the RC5 protocol)
 */
class RC5Decoder
{
private:
    __int64 frequency; 
    __int64 last_transition;
    __int64 transition_interval;
    __int64 word_interval;

    __int64 Frequency();
    __int64 Counter();

    IWordConsumer& consumer;

    bool word_bits[WORD_LENGTH_BITS];
    int bit_ctr;

    bool last_toggle;
    int  rep_ctr;

    void NewWord();
    void Bit(bool one);
    void WordComplete();
public:
    RC5Decoder(IWordConsumer& consumer);
    ~RC5Decoder(void);

    void Transition(bool up);
};

