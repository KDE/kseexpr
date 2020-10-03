/*
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

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <string>

namespace SeExpr2
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
                return s.substr(begin, end);
            return s; // empty string
        }
    } // namespace Utils
} // namespace SeExpr2
