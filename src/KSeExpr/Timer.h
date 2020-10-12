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

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#include <sys/types.h>
#endif

#include "Platform.h"

#if defined(WINDOWS) && defined(_MSC_VER)
typedef __int64 Time;
#elif __APPLE__
typedef struct timeval Time;
#else
typedef timespec Time;
#endif

namespace KSeExpr {
class Timer {
private:
#if defined(WINDOWS) && defined(_MSC_VER)
    Time time();
    Time ticksPerSeconds;
#endif
    Time startTime, stopTime;
    bool started;

public:
    Timer();
    void start();
    long elapsedTime();
};

class PrintTiming {
  public:
    PrintTiming(const std::string& s);
    ~PrintTiming();

  private:
    Timer _timer;
    const std::string _s;
};

}  // namespace KSeExpr
