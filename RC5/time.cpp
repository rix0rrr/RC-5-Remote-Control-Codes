#include "stdafx.h"
#include "time.h"

__int64 TimerFrequency()
{
    LARGE_INTEGER f;
    if (!QueryPerformanceFrequency(&f)) throw Win32Exception();
    return f.QuadPart;
}

__int64 TimerCounter()
{
    LARGE_INTEGER f;
    if (!QueryPerformanceCounter(&f)) throw Win32Exception();
    return f.QuadPart;
}