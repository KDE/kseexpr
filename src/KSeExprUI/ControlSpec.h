// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include <KSeExpr/Curve.h>
#include <KSeExpr/ExprNode.h>
#include <KSeExpr/ExprWalker.h>

namespace KSeExpr
{
//! Generic Expression control specification
class ControlSpec
{
public:
    ControlSpec(const ExprNode &node)
        : _start(node.startPos())
        , _end(node.endPos()) {};
    virtual ~ControlSpec() = default;
    ControlSpec &operator=(const ControlSpec &) = default;
    ControlSpec &operator=(ControlSpec &&) = default;
    ControlSpec(const ControlSpec &) = default;
    ControlSpec(ControlSpec &&) = default;

    //! Generates a replacement string based on changes to the spec
    virtual std::string toString() const = 0;

protected:
    //! Name of control
    std::string _name;
    //! Start position of text in original source
    int _start;
    //! End position of text in original source
    int _end;
};

//! Variable equals scalar control specification
class ExprScalarAssignSpec : public ControlSpec
{
public:
    ExprScalarAssignSpec(const ExprAssignNode &node);
    std::string toString() const override;
    inline double value() const
    {
        return _val;
    };
    static const ExprScalarAssignSpec *match(const ExprNode *node);

private:
    //! Range of values
    double _min, _max;
    //! Current Value
    double _val;
};

//! Variable equals vector control specification
class ExprVectorAssignSpec : public ControlSpec
{
public:
    ExprVectorAssignSpec(const ExprAssignNode &node);
    std::string toString() const override;
    inline const Vec3d &value() const
    {
        return _val;
    };
    static const ExprVectorAssignSpec *match(const ExprNode *node);

private:
    //! Range of values
    double _min, _max;
    //! Current Value
    Vec3d _val;
};

//! Curve assignment expression. Assignment of curve to a variable.
template<class T> class ExprCurveAssignSpec : public ControlSpec
{
public:
    ExprCurveAssignSpec(const ExprAssignNode &node);
    std::string toString() const override;
    static const ExprCurveAssignSpec *match(const ExprNode *node);

private:
    //! Lookup subexpression text
    std::string _lookupText;
    //! Control points of curve spline
    std::vector<typename Curve<T>::CV> _vec;
};

class ExprStrSpec : public ControlSpec
{
    enum Type { STRING, FILE, DIRECTORY };

public:
    //! Takes name and type comments and takes ownership of them!
    ExprStrSpec(const ExprStrNode &node, const char *name, Type type)
        : ControlSpec(node)
        , _str(node.str())
        , _type(type)
    {
        _name = name;
    }

    std::string toString() const override;
    static const ExprStrSpec *match(const ExprNode *node);

private:
    std::string _str;
    Type _type;
};

/// Examiner that builds a list of specs potentially used in widgets (for qdgui)
class SpecExaminer : public Examiner<true>
{
public:
    SpecExaminer() = default;
    ~SpecExaminer();
    SpecExaminer &operator=(const SpecExaminer &) = default;
    SpecExaminer &operator=(SpecExaminer &&) = default;
    SpecExaminer(const SpecExaminer &) = default;
    SpecExaminer(SpecExaminer &&) = default;

    bool examine(const ExprNode *examinee) override;
    void post(const ExprNode *) override {};
    void reset() override
    {
        _specList.clear();
    };
    inline int length() const
    {
        return _specList.size();
    };
    inline const ControlSpec *spec(int i) const
    {
        return _specList[i];
    };
    inline std::vector<const ControlSpec *>::const_iterator begin() const;
    inline std::vector<const ControlSpec *>::const_iterator end() const;

private:
    std::vector<const ControlSpec *> _specList;
};
} // namespace KSeExpr
