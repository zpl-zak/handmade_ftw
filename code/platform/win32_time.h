// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(WIN_TIME_H)

global_variable r64 GlobalTimePassed;
global_variable r64 GlobalSecondsPerTick;
global_variable s64 GlobalTimeCount;

internal void
TimeInit(void)
{
    LARGE_INTEGER Freq;
    QueryPerformanceFrequency(&Freq);
    
    GlobalSecondsPerTick = 1.0 / (r64)Freq.QuadPart;
    
    LARGE_INTEGER Counter;
    QueryPerformanceCounter(&Counter);
    
    GlobalTimeCount = Counter.QuadPart;
}

internal r64
TimeGet(void)
{
    LARGE_INTEGER Counter;
    QueryPerformanceCounter(&Counter);
    
     s64 Interval = Counter.QuadPart - GlobalTimeCount;
    GlobalTimeCount = Counter.QuadPart;
    
    r64 PassedSeconds = (r64)Interval * GlobalSecondsPerTick;
    GlobalTimePassed += PassedSeconds;
    
    return GlobalTimePassed;
}

#define WIN_TIME_H
#endif