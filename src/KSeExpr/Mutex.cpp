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

#include "Mutex.h"

using namespace SeExprInternal2;

/*
 * Mutex/SpinLock classes
 */
_Mutex::_Mutex()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    _mutex = CreateMutex(NULL, FALSE, NULL);
#else
    pthread_mutex_init(&_mutex, 0);
#endif
}

_Mutex::~_Mutex()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    CloseHandle(_mutex);
#else
    pthread_mutex_destroy(&_mutex);
#endif
}

void _Mutex::lock()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    WaitForSingleObject(_mutex, INFINITE);
#else
    pthread_mutex_lock(&_mutex);
#endif
}

void _Mutex::unlock()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    ReleaseMutex(_mutex);
#else
    pthread_mutex_unlock(&_mutex);
#endif
}

_SpinLock::_SpinLock()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    _spinlock = new CRITICAL_SECTION;
    InitializeCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(_spinlock));
#elif __APPLE__
    _spinlock = OS_UNFAIR_LOCK_INIT;
#else
    pthread_spin_init(&_spinlock, PTHREAD_PROCESS_PRIVATE);
#endif
}

_SpinLock::~_SpinLock() {
#if defined(WINDOWS) && defined(_MSC_VER)
    DeleteCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(_spinlock));
    delete _spinlock;
    _spinlock = nullptr;
#elif __APPLE__
#else
    pthread_spin_destroy(&_spinlock);
#endif
}

void _SpinLock::lock()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    EnterCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(_spinlock));
#elif __APPLE__
    os_unfair_lock_lock(&_spinlock);
#else
    pthread_spin_lock(&_spinlock);
#endif
}

void _SpinLock::unlock()
{
#if defined(WINDOWS) && defined(_MSC_VER)
    LeaveCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(_spinlock));
#elif __APPLE__
    os_unfair_lock_unlock(&_spinlock);
#else
    pthread_spin_unlock(&_spinlock);
#endif
}
