#include "StdAfx.h"
#include "RC5Decoder.h"
#include "time.h"

RC5Decoder::RC5Decoder(IWordConsumer& consumer)
    : consumer(consumer)
{
    frequency = TimerFrequency();

    transition_interval = (__int64)(frequency * TRANSITION_INTERVAL_S);
    word_interval       = (__int64)(frequency * WORD_INTERVAL_S);
}

RC5Decoder::~RC5Decoder()
{
}

void RC5Decoder::Transition(bool up)
{
    __int64 now = TimerCounter();

    if (last_transition + transition_interval <= now)
    {
        if (last_transition + word_interval <= now)
            NewWord();

        Bit(up);

        last_transition = now;
    }
}

void RC5Decoder::NewWord()
{
    bit_ctr = 0;
}

void RC5Decoder::Bit(bool one)
{
    if (bit_ctr >= WORD_LENGTH_BITS) return; // FIXME: WARNING, too many bits!

    word_bits[bit_ctr++] = one;
    if (bit_ctr == WORD_LENGTH_BITS)
        WordComplete();
}

/**
 * Accumulate the indicated bit at the LSB side into the given int
 */
inline void AccumulateBit(int& into, bool bit)
{
    into = (into << 1) | (bit ? 1 : 0);
}

void RC5Decoder::WordComplete()
{
    if (!word_bits[0]) return; // FIXME: WARNING, first bit should be 1

    if (last_toggle != word_bits[TOGGLE_BIT])
    {
        rep_ctr     = 0;
        last_toggle = word_bits[TOGGLE_BIT];
    }

    int system = 0,
        command = 0;

    for (int i = 3; i < 8; i++) AccumulateBit(system, word_bits[i]);

    AccumulateBit(command, !word_bits[FIELD_BIT]);
    for (int i = 8; i < 14; i++) AccumulateBit(command, word_bits[i]);

    RC5Word word(system, command, rep_ctr++);
    consumer.WordReceived(word);
}

