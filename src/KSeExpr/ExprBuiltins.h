// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0

#ifndef ExprBuiltins_h
#define ExprBuiltins_h

#include "ExprFunc.h"
#include "Platform.h"

/// Translation macros - for UI extraction
/// These macros *need* a context for lupdate to not complain
#ifndef QT_TRANSLATE_NOOP
#define QT_TRANSLATE_NOOP(scope, x) x
#endif
#ifndef QT_TRANSLATE_NOOP_UTF8
#define QT_TRANSLATE_NOOP_UTF8(scope, x) x
#endif

namespace KSeExpr {

// trig
inline double deg(double angle) { return angle * (180 / M_PI); }
inline double rad(double angle) { return angle * (M_PI / 180); }
inline double cosd(double x) { return cos(rad(x)); }
inline double sind(double x) { return sin(rad(x)); }
inline double tand(double x) { return tan(rad(x)); }
inline double acosd(double x) { return deg(acos(x)); }
inline double asind(double x) { return deg(asin(x)); }
inline double atand(double x) { return deg(atan(x)); }
inline double atan2d(double y, double x) { return deg(atan2(y, x)); }

// clamping
inline double clamp(double x, double lo, double hi) { return x < lo ? lo : x > hi ? hi : x; }
inline double round(double x) { return x < 0 ? ceil(x - 0.5) : floor(x + 0.5); }
inline double max(double x, double y) { return x > y ? x : y; }
inline double min(double x, double y) { return x < y ? x : y; }

// blending / remapping
inline double invert(double x) { return 1 - x; }
double compress(double x, double lo, double hi);
double expand(double x, double lo, double hi);
double fit(double x, double a1, double b1, double a2, double b2);
double gamma(double x, double g);
double bias(double x, double b);
double contrast(double x, double c);
double boxstep(double x, double a);
double linearstep(double x, double a, double b);
double smoothstep(double x, double a, double b);
double gaussstep(double x, double a, double b);
double remap(double x, double s, double r, double f, double interp);
double mix(double x, double y, double alpha);
Vec3d hsi(int n, const Vec3d* args);
Vec3d midhsi(int n, const Vec3d* args);
Vec3d rgbtohsl(const Vec3d& rgb);
Vec3d hsltorgb(const Vec3d& hsl);

// noise
double hash(int n, double* args);
double noise(int n, const Vec3d* args);
double snoise(const Vec3d& p);
Vec3d cnoise(const Vec3d& p);
Vec3d vnoise(const Vec3d& p);
double turbulence(int n, const Vec3d* args);
Vec3d vturbulence(int n, const Vec3d* args);
Vec3d cturbulence(int n, const Vec3d* args);
double fbm(int n, const Vec3d* args);
Vec3d vfbm(int n, const Vec3d* args);
Vec3d cfbm(int n, const Vec3d* args);
double cellnoise(const Vec3d& p);
Vec3d ccellnoise(const Vec3d& p);
double pnoise(const Vec3d& p, const Vec3d& period);

// vectors
double dist(const Vec3d& a, const Vec3d& b);
double length(const Vec3d& v);
double hypot(double x, double y);
double dot(const Vec3d& a, const Vec3d& b);
Vec3d norm(const Vec3d& a);
Vec3d cross(const Vec3d& a, const Vec3d& b);
double angle(const Vec3d& a, const Vec3d& b);
Vec3d ortho(const Vec3d& a, const Vec3d& b);
Vec3d up(const Vec3d& vec, const Vec3d& upvec);

// variations
double cycle(double index, double loRange, double hiRange);
double pick(int n, double* params);
double choose(int n, double* params);
double wchoose(int n, double* params);
double spline(int n, double* params);

// add builtins to expression function table
void defineBuiltins(ExprFunc::Define define, ExprFunc::Define3 define3);
}

#endif
