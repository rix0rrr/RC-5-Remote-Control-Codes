#pragma once

#include "RC5Decoder.h"

class SerialPortReader
{
private:
    TCHAR* port;
    RC5Decoder& decoder;
    bool open;
    bool invert;
    HANDLE handle;

    void WaitForDCDChange();
    bool GetDCD();
public:
    SerialPortReader(TCHAR* port, bool invert, RC5Decoder& decoder);
    ~SerialPortReader();

    void Open();
    void Close();

    void Run();
};

