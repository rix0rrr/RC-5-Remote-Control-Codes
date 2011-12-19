#include "StdAfx.h"
#include "SerialPort.h"

#include <assert.h>

using namespace std;

SerialPort::SerialPort(TCHAR* port, bool invertedRead, IRDecoder& decoder)
    : port(port), decoder(decoder), invertedRead(invertedRead), open(false)
{
}

SerialPort::~SerialPort()
{
    if (open) Close();
}

void SerialPort::Open()
{
    this->handle = CreateFile(port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
    if (this->handle == INVALID_HANDLE_VALUE)
	{
		throw Win32Exception(std::string("Error opening COM port ") + port);
	}
    this->open = true; // From this point on the handle must be closed

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(this->handle, &dcb)) throw Win32Exception();

    dcb.fParity         = false;
    dcb.fOutxCtsFlow    = false;
    dcb.fOutxDsrFlow    = true;
    dcb.fDtrControl     = DTR_CONTROL_DISABLE; // Important
    dcb.fOutX           = false;
    dcb.fRtsControl     = RTS_CONTROL_ENABLE; // Important
    dcb.fDsrSensitivity = true;

    if (!SetCommState(this->handle, &dcb)) throw Win32Exception();

    // Prepare for toggling based on DCD
    if (!SetCommMask(this->handle, EV_RLSD)) throw Win32Exception();
}

void SerialPort::Close()
{
    if (!this->open) return;

    CloseHandle(this->handle);
    this->open = false;
}

void SerialPort::ReadLoop()
{
    if (!this->open) throw logic_error("Open SerialPort before calling Run()");

    bool previous = GetDCD();
    while (true)
    {
        WaitForDCDChange();
        bool next = GetDCD();
        assert(next != previous);

        decoder.Transition(invertedRead ? !next : next);
        previous = next;
    }
}

bool SerialPort::GetDCD()
{
    DWORD dcd;
    if (!GetCommModemStatus(this->handle, &dcd)) throw Win32Exception();
    return dcd != 0;
}

void SerialPort::WaitForDCDChange()
{
    DWORD evt;
    if (!WaitCommEvent(this->handle, &evt, NULL)) throw Win32Exception();
}

void SerialPort::OutputPin(bool high)
{
    if (!EscapeCommFunction(this->handle, high ? SETDTR : CLRDTR)) throw Win32Exception();
}