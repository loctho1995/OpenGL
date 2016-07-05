#include "TimeCounter.h"


TimeCounter* TimeCounter::mInstance = NULL;

TimeCounter::TimeCounter()
{

}

TimeCounter::~TimeCounter()
{
}

TimeCounter* TimeCounter::GetInstance()
{
    if (!mInstance)
        mInstance = new TimeCounter();

    return mInstance;
}

void TimeCounter::StartCounter()
{
    if (!QueryPerformanceFrequency(&mClockRate))
    {
        return;
    }

    QueryPerformanceCounter(&mStartTime);

}

float TimeCounter::GetCouter()
{
    QueryPerformanceCounter(&mEndTime);
    mDelta.QuadPart = mEndTime.QuadPart - mStartTime.QuadPart;

    return ((float)mDelta.QuadPart / mClockRate.QuadPart);
}