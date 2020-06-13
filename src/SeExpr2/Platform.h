/*
 Copyright Disney Enterprises, Inc.  All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License
 and the following modification to it: Section 6 Trademarks.
 deleted and replaced with:

 6. Trademarks. This License does not grant permission to use the
 trade names, trademarks, service marks, or product names of the
 Licensor and its affiliates, except as required for reproducing
 the content of the NOTICE file.

 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
*/
/*
  The function crack_atof is adapted from the version at
  https://github.com/shaovoon/floatbench/blob/b13794c63d824b95998dac18b8cf3d05e7d84e57/BenchmarkFloatConv/BenchmarkFloatConv/BenchmarkFloatConv.cpp#L88-L171

  MIT License

  Copyright (c) 2016 shaovoon

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef Platform_h
#define Platform_h

/** @file Platform.h
    @brief Platform-specific classes, functions, and includes.
*/
#include <iostream>

#ifdef __APPLE__
#include <Availability.h>
#include <libgen.h>
#include <os/lock.h>
#endif

// platform-specific includes
#if defined(_WIN32) || defined(_WINDOWS) || defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#ifndef WINDOWS
#define WINDOWS
#endif
#ifdef _MSC_VER
#define _CRT_NONSTDC_NO_DEPRECATE 1
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#if !defined(NOMINMAX)
#define NOMINMAX 1
#endif
#endif

// note: because there are some conflicts preventing the use of
// windows.h and COFF.h (one of LLVM include files) in the same
// compilation unit (https://groups.google.com/forum/#!topic/llvm-dev/6n5Q0pFdaSA)
// do NOT include windows.h here. The Windows implementation is
// done on the Platform.cpp file, using opaque types.

#if defined(WINDOWS) && defined(_MSC_VER)

#include <malloc.h>
#include <io.h>
#include <tchar.h>
#include <process.h>

#else

// linux/unix/posix/mingw
#include <stdlib.h>
#if defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>
// Fix generated ExprParser not finding alloca - amyspark
#include <unistd.h>
#else
#include <alloca.h>
#endif
#include <string.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/time.h>
// OS for spinlock
#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#include <sys/types.h>
#endif
#endif  // defined(_WIN32)...

// general includes
#include <stdio.h>
#include <math.h>
#include <assert.h>

// missing functions on Windows
#if defined(WINDOWS) && defined(_MSC_VER)
#define snprintf sprintf_s
#define strtok_r strtok_s
typedef __int64 FilePos;
#define fseeko _fseeki64
#define ftello _ftelli64

inline double log2(double x) { return log(x) * 1.4426950408889634; }

typedef unsigned int uint32_t;
#define M_E (2.7182818284590452354)
#define M_PI (3.141592653589793238)
#if !defined(UINT32_MAX)
#define UINT32_MAX (0xffffffff)
#endif
#if !defined(UINT32_MAX)
#define UINT32_MIN (0)
#endif
#else
typedef off_t FilePos;
#endif

// Locale-independent atof() - amyspark
// Prevents SeExpr missing mantissas if LC_ALL is not "C" (like Qt)
inline double crack_atof(const char* num) {
  if (!num) {
    return 0;
  }

  int sign = 1;
  double int_part = 0.0;
  double frac_part = 0.0;
  bool has_frac = false;
  bool has_exp = false;

  // +/- sign
  if (*num == '-') {
    ++num;
    sign = -1;
  } else if (*num == '+') {
    ++num;
  }

  while (*num != '\0') {
    if (*num >= '0' && *num <= '9') {
      int_part = int_part * 10 + (*num - '0');
    } else if (*num == '.') {
      has_frac = true;
      ++num;
      break;
    } else if (*num == 'e') {
      has_exp = true;
      ++num;
      break;
    } else {
      return sign * int_part;
    }
    ++num;
  }

  if (has_frac) {
    double frac_exp = 0.1;

    while (*num != '\0') {
      if (*num >= '0' && *num <= '9') {
        frac_part += frac_exp * (*num - '0');
        frac_exp *= 0.1;
      } else if (*num == 'e') {
        has_exp = true;
        ++num;
        break;
      } else {
        return sign * (int_part + frac_part);
      }
      ++num;
  }
  }

  // parsing exponent part
  double exp_part = 1.0;
  if (*num != '\0' && has_exp) {
    int exp_sign = 1;
    if (*num == '-') {
      exp_sign = -1;
      ++num;
    } else if (*num == '+') {
        ++num;
    }

    int e = 0;
    while (*num != '\0' && *num >= '0' && *num <= '9') {
      e = e * 10 + *num - '0';
    ++num;
    }

    exp_part = pow(exp_sign * e, 10);
  }

  return sign * (int_part + frac_part) * exp_part;
}

namespace SeExpr2 {
#if !(defined(WINDOWS) && defined(_MSC_VER))

class Timer {
#ifdef __APPLE__
    typedef struct timeval Time;
#else
    typedef timespec Time;
#endif
    Time startTime, stopTime;
    bool started;

  public:
    Timer() : started(false) {}

    void start() {
        started = true;
#ifdef __APPLE__
        gettimeofday(&startTime, 0);
#else
        clock_gettime(CLOCK_MONOTONIC, &startTime);
#endif
    }

    long elapsedTime() {
        assert(started);
#ifdef __APPLE__
        gettimeofday(&stopTime, 0);
        long seconds = stopTime.tv_sec - startTime.tv_sec;
        long useconds = stopTime.tv_usec - startTime.tv_usec;
        long elapsedTime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
#else
        clock_gettime(CLOCK_MONOTONIC, &stopTime);
        long seconds = stopTime.tv_sec - startTime.tv_sec;
        long nseconds = stopTime.tv_nsec - startTime.tv_nsec;
        long elapsedTime = ((seconds) * 1000 + nseconds / 1000000.0) + 0.5;
#endif
        return elapsedTime;
    }
};
#else  // Windows
class Timer {
    __int64 time();
    __int64 ticksPerSeconds;
    __int64 startTime, stopTime;
    bool started;

  public:
    Timer();
    void start();
    long elapsedTime();
};
#endif

class PrintTiming {
  public:
    PrintTiming(const std::string& s) : _s(s) { _timer.start(); }

    ~PrintTiming() { std::cout << _s.c_str() << " (" << _timer.elapsedTime() << " ms)" << std::endl; }


  private:
    Timer _timer;
    const std::string _s;
};
}

namespace SeExprInternal2 {

/*
 * Mutex/SpinLock classes
 */

#if defined(WINDOWS) && defined(_MSC_VER)

class _Mutex {
  public:
    _Mutex();
    ~_Mutex();
    void lock();
    void unlock();

  private:
    void* _mutex;
};

class _SpinLock {
  public:
    _SpinLock();
    ~_SpinLock();
    void lock();
    void unlock();

  private:
    void* _spinlock;
};

#else
// assume linux/unix/posix
class _Mutex {
  public:
    _Mutex() { pthread_mutex_init(&_mutex, 0); }
    ~_Mutex() { pthread_mutex_destroy(&_mutex); }
    void lock() { pthread_mutex_lock(&_mutex); }
    void unlock() { pthread_mutex_unlock(&_mutex); }

  private:
    pthread_mutex_t _mutex;
};

#ifdef __APPLE__
class _SpinLock {
  public:
    _SpinLock() { _spinlock = OS_UNFAIR_LOCK_INIT; }
    ~_SpinLock() {}
    void lock() { os_unfair_lock_lock(&_spinlock); }
    void unlock() { os_unfair_lock_unlock(&_spinlock); }

  private:
    os_unfair_lock _spinlock;
};
#else
class _SpinLock {
  public:
    _SpinLock() { pthread_spin_init(&_spinlock, PTHREAD_PROCESS_PRIVATE); }
    ~_SpinLock() { pthread_spin_destroy(&_spinlock); }
    void lock() { pthread_spin_lock(&_spinlock); }
    void unlock() { pthread_spin_unlock(&_spinlock); }

  private:
    pthread_spinlock_t _spinlock;
};
#endif  // __APPLE__
#endif
}

#endif  // Platform_h
