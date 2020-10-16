// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
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
#include <inttypes.h>
#endif  // defined(_WIN32)...

// general includes
#include <stdio.h>
#include <math.h>
#include <assert.h>

// missing functions on Windows
#if defined(WINDOWS) && defined(_MSC_VER)
#define SEEXPR2_snprintf sprintf_s
#define SEEXPR2_strtok_r strtok_s
typedef __int64 FilePos;
#define SEEXPR2_fseeko _fseeki64
#define SEEXPR2_ftello _ftelli64

inline double log2(double x) { return log(x) * 1.4426950408889634; }

typedef unsigned int uint32_t;
#ifndef M_E
#define M_E (2.7182818284590452354)
#endif
#ifndef M_PI
#define M_PI (3.141592653589793238)
#endif
#if !defined(UINT32_MAX)
#define UINT32_MAX (0xffffffff)
#endif
#if !defined(UINT32_MAX)
#define UINT32_MIN (0)
#endif
#else
#define SEEXPR2_snprintf snprintf
#define SEEXPR2_strtok_r strtok_r
#define SEEXPR2_fseeko fseeko
#define SEEXPR2_ftello ftello
typedef off_t FilePos;
#endif

#endif  // Platform_h
