// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <string>

namespace KSeExpr
{
    namespace Utils
    {
        bool parseRangeComment(const std::string &comment, double_t &from, double_t &to);
        bool parseRangeComment(const std::string &comment, float_t &from, float_t &to);
        bool parseRangeComment(const std::string &comment, int32_t &from, int32_t &to);
        bool parseTypeNameComment(const std::string &comment, std::string &type, std::string &name);
        bool parseLabelComment(const std::string &comment, std::string &label);

        inline std::string trim(const std::string &s)
        {
            const auto begin {s.find_first_not_of(' ')};
            const auto end {s.find_last_not_of(' ')};
            if (begin != std::string::npos && end != std::string::npos)
                return s.substr(begin, end + 1);
            return s; // empty string
        }
    } // namespace Utils
} // namespace KSeExpr
