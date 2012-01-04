#pragma once

#include "stdafx.h"
#include "SerialPort.h"
#include "RC5Word.h"
#include "Spacer.h"
#include <fstream>

#define CARRIER_HZ 36000

class RC5Encoder
{
private:
	SerialPort& port;
	Spacer word_spacer;
	Spacer pulse_spacer;
    Spacer duty_cycle_spacer;
	bool toggle_bit;
    ofstream log;

	void TransmitWord(RC5Word& word, bool toggle);
public:
	RC5Encoder(SerialPort& port, bool initial_toggle_bit=false);
	~RC5Encoder();

	/**
	 * Transmit the given code word
	 */
	void Transmit(RC5Word& word, int repeat=1);

	bool ToggleBit() { return toggle_bit; }
};