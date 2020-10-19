// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cassert>
#include <chrono>

namespace KSeExpr
{
class Timer
{
    using Time = std::time_t;

public:
    Timer() = default;
    void start()
    {
        started = true;
        startTime = std::chrono::steady_clock::now();
    }

    void stop()
    {
        started = false;
    }

    std::chrono::steady_clock::rep elapsedTime()
    {
        if (!started) start();
        stopTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
    }

private:
    std::chrono::steady_clock::time_point startTime, stopTime;
    bool started {false};
};

} // namespace KSeExpr
