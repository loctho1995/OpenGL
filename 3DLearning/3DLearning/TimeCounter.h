#pragma once

#include <Windows.h>

class TimeCounter
{
public:
    void StartCounter();
    float GetCouter();
    static TimeCounter* GetInstance();
    ~TimeCounter();

private:
    TimeCounter();
    LARGE_INTEGER mStartTime, mEndTime, mDelta, mClockRate;
    static TimeCounter *mInstance;
};

