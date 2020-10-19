// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

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
#define KSeExpr_SSE41 [[gnu::target("sse4.1")]]
#else
#define KSeExpr_DEFAULT
#endif

namespace KSeExpr
{
namespace Utils
{
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
} // namespace Utils
} // namespace KSeExpr
