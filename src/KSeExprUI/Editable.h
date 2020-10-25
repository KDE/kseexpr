// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * @file Editable.h
 * @author Andrew Selle, L. E. Segovia
 */

#pragma once

#include <KSeExpr/Curve.h>
#include <KSeExpr/Vec.h>
#include <cstring>
#include <sstream>

inline void printVal(std::stringstream &stream, double v)
{
    stream << v;
}
inline void printVal(std::stringstream &stream, const KSeExpr::Vec3d &v)
{
    stream << "[" << v[0] << "," << v[1] << "," << v[2] << "]";
}

template<typename T> void UNUSED(T)
{
}

class Editable
{
public:
    std::string name;
    int startPos, endPos;

    Editable(const std::string &name, int startPos, int endPos);
    Editable &operator=(const Editable &) = default;
    Editable &operator=(Editable &&) = default;
    Editable(const Editable &) = default;
    Editable(Editable &&) = default;

    void updatePositions(const Editable &other);

    virtual ~Editable() = default; // must have this to ensure destruction

    /// parses a comment. if false is returned then delete the control from the editable
    virtual bool parseComment(const std::string &comment) = 0;
    virtual std::string str() const;
    virtual void appendString(std::stringstream &stream) const = 0;
    virtual bool controlsMatch(const Editable &) const = 0;
};

class NumberEditable : public Editable
{
public:
    double v;
    double min, max;
    bool isInt;
    NumberEditable(const std::string &name, int startPos, int endPos, double val);

    bool parseComment(const std::string &comment) override;
    std::string str() const override;
    void appendString(std::stringstream &stream) const override;
    bool controlsMatch(const Editable &other) const override;
};

class VectorEditable : public Editable
{
public:
    KSeExpr::Vec3d v;
    double min, max;
    bool isColor;
    VectorEditable(const std::string &name, int startPos, int endPos, const KSeExpr::Vec3d &val);

    bool parseComment(const std::string &comment) override;
    std::string str() const override;
    void appendString(std::stringstream &stream) const override;
    bool controlsMatch(const Editable &other) const override;
};

class StringEditable : public Editable
{
public:
    std::string v;
    std::string type;
    StringEditable(int startPos, int endPos, const std::string &val);

    bool parseComment(const std::string &comment) override;
    void appendString(std::stringstream &stream) const override;
    std::string str() const override;
    bool controlsMatch(const Editable &other) const override;
};

template<class TVAL> struct GenericCurveEditable : public Editable {
    using Curve = typename KSeExpr::Curve<TVAL>;
    using CV = typename KSeExpr::Curve<TVAL>::CV;
    using InterpType = typename Curve::InterpType;

    std::vector<CV> cvs;
    GenericCurveEditable(const std::string &name, int startPos, int endPos)
        : Editable(name, startPos, endPos)
    {
    }

    void add(double x, const TVAL &y, int interp)
    {
        cvs.push_back(CV(x, y, InterpType(interp)));
    }

    bool parseComment(const std::string & /*comment*/) override
    {
        return true;
    }
    std::string str() const override
    {
        std::stringstream s;
        s << name << " ccurve";
        return s.str();
    }

public:
    void appendString(std::stringstream &stream) const override
    {
        for (const auto &cv : cvs) {
            stream << "," << cv._pos << ",";
            printVal(stream, cv._val);
            stream << "," << cv._interp;
        }
    }

    bool controlsMatch(const Editable &other) const override
    {
        if (const auto *o = dynamic_cast<const GenericCurveEditable *>(&other)) {
            // TODO: fix  this
            //            return cvs==o->cvs && name==o->name;
            UNUSED(o);
            return false;
        } else
            return false;
    }
};
using ColorCurveEditable = GenericCurveEditable<KSeExpr::Vec3d>;
using CurveEditable = GenericCurveEditable<double>;

class ColorSwatchEditable : public Editable
{
public:
    std::vector<KSeExpr::Vec3d> colors;
    std::string labelType;

    ColorSwatchEditable(const std::string &name, int startPos, int endPos);

    bool parseComment(const std::string &comment) override;
    std::string str() const override;
    void appendString(std::stringstream &stream) const override;
    bool controlsMatch(const Editable &other) const override;

    void add(const KSeExpr::Vec3d &value);
    void change(int index, const KSeExpr::Vec3d &value);
    void remove(int index);
    void print();
};
