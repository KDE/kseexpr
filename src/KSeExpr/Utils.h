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
#include <stdexcept>
#include <string>

#include "ExprConfig.h"

#if defined(KSeExpr_HAVE_DYNAMIC_DISPATCH)
// Enable SSE4.1 functions only with automagic dynamic dispatch available.
#include <smmintrin.h>
#define KSeExpr_DEFAULT [[gnu::target("default")]]
#define KSeExpr_SSE41   [[gnu::target("sse4.1")]]
#else
#define KSeExpr_DEFAULT
#endif

namespace KSeExpr {
    namespace Utils {
        double_t atof(const char *num);
        double_t atof(const std::string &num);
        int32_t strtol(const char *num);
        int32_t strtol(const std::string &num);
        KSeExpr_DEFAULT double_t round(double_t val);
        KSeExpr_DEFAULT double_t floor(double_t val);

#if defined(KSeExpr_HAVE_DYNAMIC_DISPATCH)
        KSeExpr_SSE41 double_t round(double_t val);
        KSeExpr_SSE41 double_t floor(double_t val);
#endif
    } //namespace Utils
}  // namespace KSeExpr
