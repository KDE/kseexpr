// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0

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
