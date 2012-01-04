#pragma once

#include "stdafx.h"

/**
 * Return the frequency of the available timer in Hz
 */
inline static __int64 TimerFrequency()
{
    static bool initialized = false;
    static LARGE_INTEGER f;

    if (!initialized)
    {
        if (!QueryPerformanceFrequency(&f)) throw Win32Exception();
        initialized = true;
    }

    return f.QuadPart;
}

/**
 * Return the current value of the timer
 */
inline static __int64 TimerCounter()
{
    LARGE_INTEGER f;
    if (!QueryPerformanceCounter(&f)) throw Win32Exception();
    return f.QuadPart;
}