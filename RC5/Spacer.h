#pragma once

#include "stdafx.h"
#include "time.h"

#define S 1
#define MS 1E-3
#define US 1E-6
#define NS 1E-9

/**
 * Class used for timing purposes
 *
 * Call Begin() before an operation and End() after it. Will
 * wait additional time (if necessary) to make the operation take
 * exactly the amount of seconds that the Spacer
 * has been initialized with.
 */
class Spacer
{
private:
	__int64 duration;
	__int64 expectedEnd;
	__int64 schedulingQuantum;

public:
	Spacer(double duration) : duration((__int64)(duration * TimerFrequency())) { }
	~Spacer() { }

	void Begin()
	{
		expectedEnd = TimerCounter() + duration;
	}

	void End() 
	{
		// This may look bad but we need sub-ms resolution, and
		// Windows usually only guarantees us resolution on the 
		// order of 10ms, so there's no way to do better.
	
		while (TimerCounter() <= expectedEnd) /* Nothing */; 
	}

	void Next()
	{
		End();
		Begin();
	}
};

