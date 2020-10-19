// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>
#include <iostream>

#include "Timer.h"

namespace KSeExpr {
    class PerformanceTimer {
    public:
        PerformanceTimer(std::string l)
            : label(std::move(l))
        {
            clock.start();
        }

        ~PerformanceTimer() {
            std::cout << label << " (" << clock.elapsedTime() << " ms)" << std::endl;
        }

        PerformanceTimer(PerformanceTimer&&) = default;
        PerformanceTimer& operator=(PerformanceTimer &&) = default;
        PerformanceTimer(PerformanceTimer&) = default;
        PerformanceTimer& operator=(const PerformanceTimer&) = default;

    private:
        Timer clock;
        std::string label;
    };
} // namespace KSeExpr
