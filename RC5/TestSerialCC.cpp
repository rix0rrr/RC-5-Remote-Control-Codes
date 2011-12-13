// TestSerialCC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>

#define COM1 0x3F8
#define MCR 0x04
#define DTR 0x01

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hSerial = CreateFileA("COM2",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (hSerial==INVALID_HANDLE_VALUE)
    {
        printf("Error opening serial port.\n");
        return 1;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(hSerial, &dcb)) {
        printf("Error getting state.\n");
        return 1;
    }

    dcb.fParity      = false;
    dcb.fOutxCtsFlow = false;
    dcb.fOutxDsrFlow = false;
    dcb.fDtrControl  = DTR_CONTROL_DISABLE;
    dcb.fOutX        = false;
    dcb.fRtsControl  = RTS_CONTROL_DISABLE;
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;
    if (!SetCommState(hSerial, &dcb))
    {
        printf("Error setting state.\n");
        return 1;
    }

    printf("Setting DTR.\n");
    if (!EscapeCommFunction(hSerial, SETDTR))
    {
        printf("Error setting DTR.\n");
        return 1;
    }
    printf("DTR has been set, you should now see IR.\n");
    getch();

    CloseHandle(hSerial);
}
