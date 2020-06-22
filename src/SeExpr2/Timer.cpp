/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
* Copyright (C) 2020 L. E. Segovia <amy@amyspark.me>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License
* and the following modification to it: Section 6 Trademarks.
* deleted and replaced with:
*
* 6. Trademarks. This License does not grant permission to use the
* trade names, trademarks, service marks, or product names of the
* Licensor and its affiliates, except as required for reproducing
* the content of the NOTICE file.
*
* You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "Timer.h"

#if defined(WINDOWS) && defined(_MSC_VER)
#define _CRT_NONSTDC_NO_DEPRECATE 1
#define _CRT_SECURE_NO_DEPRECATE 1
#define NOMINMAX 1

// windows - defined for both Win32 and Win64
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

using namespace SeExpr2;

Timer::Timer()
    : started(false)
{
#if defined(WINDOWS) && defined(_MSC_VER)
    // get the timer frequency
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    ticksPerSeconds = frequency.QuadPart;
#endif
}

void Timer::start()
{
    started = true;
#if defined(WINDOWS) && defined(_MSC_VER)
    startTime = this->time();
#elif __APPLE__
    gettimeofday(&startTime, 0);
#else
    clock_gettime(CLOCK_MONOTONIC, &startTime);
#endif
}

#if defined(WINDOWS) && defined(_MSC_VER)
Time Timer::time() {
    LARGE_INTEGER perfCounter;
    QueryPerformanceCounter(&perfCounter);
    return perfCounter.QuadPart;
}
#endif

long Timer::elapsedTime() {
    assert(started);
#if defined(WINDOWS) && defined(_MSC_VER)
    stopTime = this->time();
    return static_cast<long>(((stopTime - startTime) * 1000000) / ticksPerSeconds);
#elif __APPLE__
    gettimeofday(&stopTime, 0);
    long seconds = stopTime.tv_sec - startTime.tv_sec;
    long useconds = stopTime.tv_usec - startTime.tv_usec;
    long elapsedTime = ((seconds)*1000 + useconds / 1000.0) + 0.5;
#else
        clock_gettime(CLOCK_MONOTONIC, &stopTime);
        long seconds = stopTime.tv_sec - startTime.tv_sec;
        long nseconds = stopTime.tv_nsec - startTime.tv_nsec;
        long elapsedTime = ((seconds)*1000 + nseconds / 1000000.0) + 0.5;
#endif
    return elapsedTime;
}

PrintTiming::PrintTiming(const std::string& s)
    : _s(s)
{
    _timer.start();
}

PrintTiming::~PrintTiming()
{
    std::cout << _s.c_str() << " (" << _timer.elapsedTime() << " ms)" << std::endl;
}
