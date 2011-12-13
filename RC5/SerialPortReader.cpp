#include "StdAfx.h"
#include "SerialPortReader.h"

#include <assert.h>

using namespace std;

SerialPortReader::SerialPortReader(TCHAR* port, bool invert, RC5Decoder& decoder)
    : port(port), decoder(decoder), invert(invert), open(false)
{
}

SerialPortReader::~SerialPortReader()
{
    if (open) Close();
}

void SerialPortReader::Open()
{
    this->handle = CreateFile(port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
    if (this->handle == INVALID_HANDLE_VALUE) throw new Win32Exception();
    this->open = true; // From this point on the handle must be closed

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(this->handle, &dcb)) throw new Win32Exception();

    dcb.fParity         = false;
    dcb.fOutxCtsFlow    = false;
    dcb.fOutxDsrFlow    = true;
    dcb.fDtrControl     = DTR_CONTROL_DISABLE; // Important
    dcb.fOutX           = false;
    dcb.fRtsControl     = RTS_CONTROL_ENABLE; // Important
    dcb.fDsrSensitivity = true;

    if (!SetCommState(this->handle, &dcb)) throw new Win32Exception();

    // Prepare for toggling based on DCD
    if (!SetCommMask(this->handle, EV_RLSD)) throw new Win32Exception();
}

void SerialPortReader::Close()
{
    if (!this->open) return;

    CloseHandle(this->handle);
    this->open = false;
}

void SerialPortReader::Run()
{
    if (!this->open) throw new logic_error("Open SerialPortReader before calling Run()");

    bool previous = GetDCD();
    while (true)
    {
        WaitForDCDChange();
        bool next = GetDCD();
        assert(next != previous);

        decoder.Transition(invert ? !next : next);
        previous = next;
    }
}

bool SerialPortReader::GetDCD()
{
    DWORD dcd;
    if (!GetCommModemStatus(this->handle, &dcd)) throw new Win32Exception();
    return dcd != 0;
}

void SerialPortReader::WaitForDCDChange()
{
    DWORD evt;
    if (!WaitCommEvent(this->handle, &evt, NULL)) throw new Win32Exception();
}