// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#include "Utils.h"
#include <cerrno>

#if defined(KSeExpr_HAVE_CHARCONV_WITH_DOUBLES)
#include <charconv>

double_t KSeExpr::Utils::atof(const char *num)
{
    double_t v;
    auto [p, ec] = std::from_chars(num, num + std::strlen(num), v);
    if (ec == std::errc()) {
        return v;
    } else {
        return HUGE_VAL;
    }
}

double_t KSeExpr::Utils::atof(const std::string &num)
{
    double_t v;
    auto [p, ec] = std::from_chars(val.data(), val.data() + val.size(), v);
    if (ec == std::errc()) {
        return v;
    } else {
        return HUGE_VAL;
    }
}

int32_t KSeExpr::Utils::strtol(const std::string &num)
{
    int32_t v;
    auto [p, ec] = std::from_chars(val.data(), val.data() + val.size(), v);
    if (ec == std::errc()) {
        return v;
    } else if (ec == std::errc::result_out_of_range) {
        throw std::out_of_range {"KSeExpr::Utils::strtol: out of range"};
    } else {
        throw std::invalid_argument {"KSeExpr::Utils::strtol: impossible to parse the given number"};
    }
}

#else
/**
 * Locale-independent atof() - amyspark
 * Prevents SeExpr missing mantissas if LC_ALL is not "C" (like Qt)
 */
double_t KSeExpr::Utils::atof(const char *num)
{
    if (!num) {
        return 0;
    }

    double_t sign = 1;
    double_t int_part = 0.0;
    double_t frac_part = 0.0;
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
            return HUGE_VAL;
        }
        ++num;
    }

    if (has_frac) {
        double_t frac_exp = 0.1; // NOLINT readability-magic-numbers

        while (*num != '\0') {
            if (*num >= '0' && *num <= '9') {
                frac_part += frac_exp * (*num - '0');
                frac_exp *= 0.1; // NOLINT readability-magic-numbers
            } else if (*num == 'e') {
                has_exp = true;
                ++num;
                break;
            } else {
                return HUGE_VAL;
            }
            ++num;
        }
    }

    // parsing exponent part
    double_t exp_part = 1.0;
    if (*num != '\0' && has_exp) {
        int exp_sign = 1;
        if (*num == '-') {
            exp_sign = -1;
            ++num;
        } else if (*num == '+') {
            ++num;
        }

        int e = 0;
        while (*num != '\0') {
            if (*num >= '0' && *num <= '9') {
                e = e * 10 + *num - '0'; // NOLINT readability-magic-numbers
            } else {
                return HUGE_VAL;
            }

            ++num;
        }

        exp_part = pow(exp_sign * e, 10); // NOLINT readability-magic-numbers
    }

    return sign * (int_part + frac_part) * exp_part;
}

double_t KSeExpr::Utils::atof(const std::string &num)
{
    return Utils::atof(num.data());
}

int32_t KSeExpr::Utils::strtol(const std::string &num)
{
    char *ptr {nullptr};
    // integer numbers only use dots
    const auto result {std::strtol(num.c_str(), &ptr, 10)};
    if (ptr == num.c_str())
        throw std::invalid_argument {"KSeExpr::Utils::atoi: impossible to parse the given number"};
    else if (ptr != num.end().base())
        throw std::invalid_argument {"KSeExpr::Utils::atoi: the string had invalid extra characters"};
    else if (errno == ERANGE)
        throw std::out_of_range {"KSeExpr::Utils::atoi: out of range"};
    return result;
}

#endif // defined(HAVE_CHARCONV_WITH_DOUBLES)

// Dynamically dispatchable functions.
// These have to be in a namespace, otherwise GCC chokes.
// See https://stackoverflow.com/questions/19785010/gcc-function-multiversioning-and-namespaces

namespace KSeExpr
{
namespace Utils
{
KSeExpr_DEFAULT double_t round(double_t val)
{
    return std::round(val);
}

KSeExpr_DEFAULT double_t floor(double_t val)
{
    return std::floor(val);
}

#if defined(KSeExpr_HAVE_DYNAMIC_DISPATCH)
KSeExpr_SSE41 double_t round(double_t val)
{
    return _mm_cvtsd_f64(_mm_round_sd(_mm_set_sd(0.0), _mm_set_sd(val), _MM_FROUND_TO_NEAREST_INT));
}

KSeExpr_SSE41 double_t floor(double_t val)
{
    return _mm_cvtsd_f64(_mm_floor_sd(_mm_set_sd(0.0), _mm_set_sd(val)));
}
#endif
} // namespace Utils
} // namespace KSeExpr
