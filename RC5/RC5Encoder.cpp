#include "StdAfx.h"
#include "RC5Encoder.h"
#include "time.h"

const int PULSES_PER_BIT = 64;
const int BITS_PER_WORD  = 14;
const double PULSE_TIME  = 1 / (double)CARRIER_HZ;
const double DUTY_CYCLE_FRAC = 0.5;
const double WORD_TIME = 113778 * US;

RC5Encoder::RC5Encoder(SerialPort& port, bool initial_toggle_bit)
	: port(port), toggle_bit(initial_toggle_bit), word_spacer(WORD_TIME),
      pulse_spacer(PULSE_TIME), duty_cycle_spacer(DUTY_CYCLE_FRAC * PULSE_TIME),
      log("rc5.log", ios_base::trunc)
{
}


RC5Encoder::~RC5Encoder(void)
{
}

void RC5Encoder::Transmit(RC5Word& word, int repeat)
{
	for (int i = 0; i < repeat; i++)
	{
		word_spacer.Next();
		TransmitWord(word, toggle_bit);
	}
	toggle_bit = !toggle_bit;
}

void Priority(int priority)
{
    if (!SetThreadPriority(GetCurrentThread(), priority)) throw Win32Exception();
}

void RC5Encoder::TransmitWord(RC5Word& word, bool toggle)
{
    Priority(THREAD_PRIORITY_TIME_CRITICAL);

    bool bits[BITS_PER_WORD] = {
        // Header
	    true,
	    (word.Command() & 0x40) == 0, // Note: bit inverted
    	toggle,
        // System
	    (word.System() & 0x10) != 0,
	    (word.System() & 0x08) != 0,
	    (word.System() & 0x04) != 0,
	    (word.System() & 0x02) != 0,
	    (word.System() & 0x01) != 0,
        // Command
	    (word.Command() & 0x20) != 0,
	    (word.Command() & 0x10) != 0,
	    (word.Command() & 0x08) != 0,
	    (word.Command() & 0x04) != 0,
	    (word.Command() & 0x02) != 0,
	    (word.Command() & 0x01) != 0
    };

    log << "Pulse duration: " << pulse_spacer.Duration() << endl;
    log << "Duty cycle duration: " << duty_cycle_spacer.Duration() << endl;

    log << "F:" << TimerFrequency() << endl;

    __int64 last = TimerCounter();
    HANDLE h = port.Handle();
    for (int bit = 0; bit < BITS_PER_WORD; bit++)
    {
        for (int p = 0; p < PULSES_PER_BIT; p++)
        {
            // When it's time for the next pulse...
            pulse_spacer.Next();
            __int64 t = TimerCounter();
            log << "D: " << t - last << endl;
            last = t;

            // Either pulse in the second half-bit of a "1" or in the first half-bit of a "0"
            // to generate an either upward or downward transition
            bool second_half = (PULSES_PER_BIT / 2) <= p;

            duty_cycle_spacer.Begin();
	        port.OutputPin(bits[bit] == second_half);
            duty_cycle_spacer.End();
	        port.OutputPin(false);

            log << "Time to do pulse: " << TimerCounter() - t << endl;
        }
    }

    pulse_spacer.Next();
	port.OutputPin(false);

    Priority(THREAD_PRIORITY_NORMAL);
}

