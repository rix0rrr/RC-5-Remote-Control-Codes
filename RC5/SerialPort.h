#pragma once

#include "RC5Decoder.h"

class SerialPort
{
private:
    TCHAR* port;
    IRDecoder& decoder;
    bool open;
    bool invertedRead;
    HANDLE handle;

    void WaitForDCDChange();
    bool GetDCD();
public:
    SerialPort(TCHAR* port, bool invertedRead, IRDecoder& decoder);
    ~SerialPort();

	/**
	 * Open the port
	 */
    void Open();

	/**
	 * Close the port (called automatically as part of destructor if required)
	 */
    void Close();

	/**
	 * Start an endless read loop that will pass received transitions
	 * on the DCD line to the RC5 decoder
	 */
    void ReadLoop();

	/**
	 * Make the DTR output pin on the RS232 port high or low, turning the IR LED on or off
	 */
	void OutputPin(bool high);
    
    HANDLE Handle() { return handle; }
};