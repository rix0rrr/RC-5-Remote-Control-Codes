#include "StdAfx.h"
#include "RC5Encoder.h"

RC5Encoder::RC5Encoder(SerialPort& port)
	: port(port), word_spacer(WORD_TIME), halfbit_spacer(HALFBIT_TIME), pulse_spacer(DUTY_CYCLE_TIME)
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

void RC5Encoder::TransmitWord(RC5Word& word, bool toggle)
{
	TransmitBit(true);
	TransmitBit((word.Command() & 0x40) != 0);
	TransmitBit(toggle);

	TransmitBit((word.System() & 0x10) != 0);
	TransmitBit((word.System() & 0x08) != 0);
	TransmitBit((word.System() & 0x04) != 0);
	TransmitBit((word.System() & 0x02) != 0);
	TransmitBit((word.System() & 0x01) != 0);

	TransmitBit((word.Command() & 0x20) != 0);
	TransmitBit((word.Command() & 0x10) != 0);
	TransmitBit((word.Command() & 0x08) != 0);
	TransmitBit((word.Command() & 0x04) != 0);
	TransmitBit((word.Command() & 0x02) != 0);
	TransmitBit((word.Command() & 0x01) != 0);

	port.OutputPin(false);
}

void RC5Encoder::TransmitBit(bool one)
{
	// A 1 is defined as an upwards transition, a 0
	// as a downward transition.
	if (one)
	{
		HalfBitOff();
		HalfBitOn();
	}
	else
	{
		HalfBitOn();
		HalfBitOff();
	}
}

void RC5Encoder::HalfBitOff()
{
	for (int i = 0; i < 32; i++)
		pulse_spacer.Next();
}

void RC5Encoder::HalfBitOn()
{
	for (int i = 0; i < 32; i++)
		Pulse();
}

void RC5Encoder::Pulse()
{
	pulse_spacer.Next();
	port.OutputPin(true);
	pulse_spacer.Next();
	port.OutputPin(false);
}