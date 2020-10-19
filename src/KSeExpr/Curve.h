// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#pragma once

#include <cassert>
#include <cfloat>
#include <vector>

#include "Vec.h"

namespace KSeExpr
{
//! Interpolation curve class for double->double and double->Vec3D
/**   Interpolation curve class for mapping from double -> double or double -> Vec3D
      Subject to some interpolation points.

      Each segment is interpolated according to the interpolation type specified on the
      left control point. Interpolation types supported are members of InterpType
      below.

      Valid instantiation types for this are double, or Vec3D
*/
template<class T> class Curve
{
    mutable int cacheCV;

public:
    //! Supported interpolation types
    enum InterpType { kNone = 0, kLinear, kSmooth, kSpline, kMonotoneSpline };
    struct CV {
        CV(double pos, const T &val, InterpType type)
            : _pos(pos)
            , _val(val)
            , _interp(type)
        {
        }

        double _pos;
        T _val, _deriv;
        InterpType _interp;
    };

private:
    std::vector<CV> _cvData;
    bool prepared;

public:
    Curve();

    //! Adds a point to the curve
    void addPoint(double position, const T &val, InterpType type);

    //! Prepares points for evaluation (sorts and computes boundaries, clamps extrema)
    void preparePoints();

    //! Evaluates curve and returns full value
    T getValue(double param) const;

    //! Evaluates curve for a sub-component of the interpolation values
    //! must call preparePoints() before this is ok to call
    double getChannelValue(double param, int channel) const;

    //! Returns the control point that is less than the parameter, unless there is no
    //! point, in which case it returns the right point or nothing
    CV getLowerBoundCV(double param) const;

    //! Returns whether the given interpolation type is supported
    static bool interpTypeValid(InterpType interp);

    //! CV Parameter ordering (cv1._pos < cv2._pos)
    static bool cvLessThan(const CV &cv1, const CV &cv2);

private:
    //! Performs hermite derivative clamping in canonical space
    void clampCurveSegment(const T &delta, T &d1, T &d2);

    //! Returns a component of the given value
    static double comp(const T &val, int i);
};
} // namespace KSeExpr
