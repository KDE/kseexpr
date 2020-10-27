// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <type_traits>


//#############################################################################
// Template Metaprogramming Helpers
namespace KSeExpr
{
//! Static conditional type true case
template<bool c, class T1, class T2> struct static_if {
    using TYPE = T1;
};
//! Static conditional type false case
template<class T1, class T2> struct static_if<false, T1, T2> {
    using TYPE = T2;
};

//! Vec class, generic dimension vector class
//! can also point to data if the template argument ref is true
template<class T, size_t d, bool ref = false> class Vec
{
    //! internal data (either an explicit arary or a pointer to raw data)
    typename static_if<ref, T *, std::array<T, d>>::TYPE x;

public:
    using T_VEC_VALUE = Vec<T, d, false>;
    using T_VEC_REF = Vec<T, d, true>;

    //! Initialize vector value using raw memory
    template<class T2> static Vec<T, d, false> copy(T2 *raw)
    {
        static_assert(!ref, "Cannot be used with vector references");
        Vec<T, d, false> ret;
        for (size_t k = 0; k < d; k++)
            ret[k] = static_cast<T>(raw[k]);
        return ret;
    }

    //! Initialize vector to be reference to plain raw data
    explicit Vec(T *raw)
        : x(raw)
    {
        static_assert(ref, "Constructor cannot be used with vector values");
    }

    //! Empty constructor (this is invalid for a reference type)
    Vec()
    {
        static_assert(!ref, "Constructor cannot be used with vector references");
    }

    //! Convenience constant vector initialization (valid for any d)
    Vec(T v0)
    {
        static_assert(!ref, "Constructor cannot be used with vector references");
        for (size_t k = 0; k < d; k++)
            x[k] = v0;
    }

    //! Convenience 2 vector initialization (only for d==2)
    Vec(T v1, T v2)
    {
        static_assert(!ref, "Constructor cannot be used with vector references");
        static_assert(d == 2, "Invalid constructor for dimension");
        x[0] = v1;
        x[1] = v2;
    }

    //! Convenience 3 vector initialization (only for d==3)
    Vec(T v1, T v2, T v3)
    {
        static_assert(!ref, "Constructor cannot be used with vector references");
        static_assert(d == 3, "Invalid constructor for dimension");
        x[0] = v1;
        x[1] = v2;
        x[2] = v3;
    }

    //! Convenience 4 vector initialization (only for d==4)
    Vec(T v1, T v2, T v3, T v4)
    {
        static_assert(!ref, "Constructor cannot be used with vector references");
        static_assert(d == 4, "Invalid constructor for dimension");
        x[0] = v1;
        x[1] = v2;
        x[2] = v3;
        x[3] = v4;
    }
    // Changed this to default. This is safe! for reference case it makes another reference
    // for value it copies
    //! Copy construct. Only valid if we are not going to be a reference data!
    // Vec(const Vec&)

    //! Copy construct. Only valid if we are not going to be reference data!
    template<class T2, bool refother> Vec(const Vec<T2, d, refother> &other)
    {
        static_assert(!ref, "Cannot be used with vector references");
        *this = other;
    }

    template<class T2, bool refother> Vec &operator=(const Vec<T2, d, refother> &other)
    {
        for (size_t k = 0; k < d; k++)
            x[k] = other[k];
        return *this;
    }

    template<class Tother, bool refother> bool operator==(const Vec<Tother, d, refother> &other) const
    {
        for (size_t k = 0; k < d; k++)
            if (x[k] != other[k])
                return false;
        return true;
    }

    template<class Tother, bool refother> bool operator!=(const Vec<Tother, d, refother> &other) const
    {
        return !(*this != other);
    }

    // non-const element access
    T &operator[](const int i)
    {
        return x[i];
    }

    // const element access
    const T &operator[](const int i) const
    {
        return x[i];
    }

    //! Square of euclidean (2) norm
    T length2() const
    {
        std::array<T, d> data;
        for (size_t k = 0; k < d; k++)
            data[k] = x[k] * x[k];
        return std::accumulate(data.cbegin(), data.cend(), static_cast<T>(0));
    }

    //! Euclidean (2) norm
    T length() const
    {
        return sqrt(length2());
    }

    //! Normalize in place and return the 2-norm before normalization
    T normalize()
    {
        T l = length2();
        if (l) {
            l = sqrt(l);
            *this /= l;
        } else {
            *this = T_VEC_VALUE((T)0);
            x[0] = 1;
        }
        return l;
    }

    //! Return a copy of the vector that is normalized
    Vec<T, d, false> normalized() const
    {
        Vec<T, d, false> other(*this);
        other.normalize();
        return other;
    }

    Vec &operator/=(const T val)
    {
        T one_over_val = T(1) / val;
        for (size_t k = 0; k < d; k++)
            x[k] *= one_over_val;
        return *this;
    }

    Vec &operator*=(const T val)
    {
        for (size_t k = 0; k < d; k++)
            x[k] *= val;
        return *this;
    }

    template<bool refother> Vec &operator+=(const Vec<T, d, refother> &other)
    {
        for (size_t k = 0; k < d; k++)
            x[k] += other[k];
        return *this;
    }

    template<bool refother> Vec &operator-=(const Vec<T, d, refother> &other)
    {
        for (size_t k = 0; k < d; k++)
            x[k] -= other[k];
        return *this;
    }

    template<bool refother> Vec &operator*=(const Vec<T, d, refother> &other)
    {
        for (size_t k = 0; k < d; k++)
            x[k] *= other[k];
        return *this;
    }

    template<bool refother> Vec &operator/=(const Vec<T, d, refother> &other)
    {
        for (size_t k = 0; k < d; k++)
            x[k] /= other[k];
        return *this;
    }

    T_VEC_VALUE operator-() const
    {
        T_VEC_VALUE val(*this);
        for (size_t k = 0; k < d; k++)
            val[k] = -val[k];
        return val;
    }

    template<bool refother> bool operator==(const Vec<T, d, refother> &other) const
    {
        bool equal = true;
        for (size_t k = 0; k < d; k++)
            equal &= (x[k] == other[k]);
        return equal;
    }

    template<bool refother> bool operator!=(const Vec<T, d, refother> &other) const
    {
        return !(*this == other);
    }

    T_VEC_VALUE operator*(T s) const
    {
        T_VEC_VALUE val(*this);
        val *= s;
        return val;
    }

    T_VEC_VALUE operator/(T s) const
    {
        T_VEC_VALUE val(*this);
        val /= s;
        return val;
    }

    template<bool refother> T_VEC_VALUE operator+(const Vec<T, d, refother> &other) const
    {
        T_VEC_VALUE val(*this);
        val += other;
        return val;
    }

    template<bool refother> T_VEC_VALUE operator-(const Vec<T, d, refother> &other) const
    {
        T_VEC_VALUE val(*this);
        val -= other;
        return val;
    }

    template<bool refother> T_VEC_VALUE operator*(const Vec<T, d, refother> &other) const
    {
        T_VEC_VALUE val(*this);
        val *= other;
        return val;
    }

    template<bool refother> T_VEC_VALUE operator/(const Vec<T, d, refother> &other) const
    {
        T_VEC_VALUE val(*this);
        val /= other;
        return val;
    }

    friend T_VEC_VALUE operator*(T s, const Vec &v)
    {
        return v * s;
    }

    /** Inner product. */
    template<bool refother> T dot(const Vec<T, d, refother> &o) const
    {
        std::array<T, d> data;
        for (size_t k = 0; k < d; k++)
            data[k] = x[k] * o[k];
        return std::accumulate(data.cbegin(), data.cend(), static_cast<T>(0));
    }

    /** Cross product. */
    template<bool refother> T_VEC_VALUE cross(const Vec<T, 3, refother> &o) const
    {
        static_assert(d == 3, "Invalid constructor for dimension");        return T_VEC_VALUE(x[1] * o[2] - x[2] * o[1], x[2] * o[0] - x[0] * o[2], x[0] * o[1] - x[1] * o[0]);
    }

    /** Return a vector orthogonal to the current vector. */
    T_VEC_VALUE orthogonal() const
    {
        static_assert(d == 3, "Invalid constructor for dimension");
        return T_VEC_VALUE(x[1] + x[2], x[2] - x[0], -x[0] - x[1]);
    }

    /**
     * Returns the angle in radians between the current vector and the
     * passed in vector.
     */
    template<bool refother> T angle(const Vec<T, 3, refother> &o) const
    {
        static_assert(d == 3, "Invalid constructor for dimension");
        T l = length() * o.length();
        if (l == 0)
            return 0;
        return acos(dot(o) / l);
    }

    /**
     * Returns the vector rotated by the angle given in radians about
     * the given axis. (Axis must be normalized)
     */
    template<bool refother> T_VEC_VALUE rotateBy(const Vec<T, 3, refother> &axis, T angle) const
    {
        static_assert(d == 3, "Invalid constructor for dimension");
        double c = cos(angle);
        double s = sin(angle);
        return c * (*this) + (1 - c) * dot(axis) * axis - s * cross(axis);
    }
};

//! Output stream
template<class T, size_t d, bool r> std::ostream &operator<<(std::ostream &out, const Vec<T, d, r> &val)
{
    if (d > 0)
        out << "(" << val[0];
    for (size_t k = 1; k < d; k++)
        out << "," << val[k];
    out << ")";
    return out;
}

using Vec1d = Vec<double, 1, false>;
using Vec2d = Vec<double, 2, false>;
using Vec3d = Vec<double, 3, false>;
using Vec4d = Vec<double, 4, false>;
using Vec1f = Vec<float, 1, false>;
using Vec2f = Vec<float, 2, false>;
using Vec3f = Vec<float, 3, false>;
using Vec4f = Vec<float, 4, false>;
using Vec1dRef = Vec<double, 1, true>;
using Vec2dRef = Vec<double, 2, true>;
using Vec3dRef = Vec<double, 3, true>;
using Vec4dRef = Vec<double, 4, true>;
using Vec1fRef = Vec<float, 1, true>;
using Vec2fRef = Vec<float, 2, true>;
using Vec3fRef = Vec<float, 3, true>;
using Vec4fRef = Vec<float, 4, true>;
using Vec1dConstRef = Vec<const double, 1, true>;
using Vec2dConstRef = Vec<const double, 2, true>;
using Vec3dConstRef = Vec<const double, 3, true>;
using Vec4dConstRef = Vec<const double, 4, true>;
using Vec1fConstRef = Vec<const float, 1, true>;
using Vec2fConstRef = Vec<const float, 2, true>;
using Vec3fConstRef = Vec<const float, 3, true>;
using Vec4fConstRef = Vec<const float, 4, true>;
} // namespace KSeExpr
