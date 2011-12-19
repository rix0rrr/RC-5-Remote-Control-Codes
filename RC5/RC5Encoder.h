#pragma once

#include "stdafx.h"
#include "SerialPort.h"
#include "RC5Word.h"
#include "Spacer.h"

#define CARRIER_HZ 36000

#define WORD_TIME (113778 * US)
#define HALFBIT_TIME (889 * US)
#define PULSE_TIME (1 / CARRIER_HZ)
#define DUTY_CYCLE_TIME (0.5 * PULSE_TIME)

class RC5Encoder
{
private:
	SerialPort& port;
	Spacer word_spacer;
	Spacer halfbit_spacer;
	Spacer pulse_spacer;
	bool toggle_bit;

	void TransmitWord(RC5Word& word, bool toggle);
	void TransmitBit(bool one);
	
	void HalfBitOff();
	void HalfBitOn();

	void Pulse();
public:
	RC5Encoder(SerialPort& port, bool initial_toggle_bit=false);
	~RC5Encoder();

	/**
	 * Transmit the given code word
	 */
	void Transmit(RC5Word& word, int repeat=1);

	bool ToggleBit() { return toggle_bit; }
};