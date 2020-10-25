// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * @file BasicExpression.cpp
 * @brief A basic expression context for the expression previewer
 * @author  aselle
 */

#include "BasicExpression.h"

BasicExpression::BasicExpression(const std::string &expr, const KSeExpr::ExprType &type)
    : Expression(expr, type)
    , dummyFunc(dummyFuncX, 0, 16)
{
}

BasicExpression::~BasicExpression()
{
    clearVars();
}

template<class T_MAP> void deleteAndClear(T_MAP &map)
{
    for (const auto& i: map)
        delete i.second;
    map.clear();
}

void BasicExpression::clearVars()
{
    deleteAndClear(varmap);
    funcmap.clear();
}

void BasicExpression::setExpr(const std::string &str)
{
    clearVars();
    Expression::setExpr(str);
}

KSeExpr::ExprVarRef *BasicExpression::resolveVar(const std::string &name) const
{
    if (name == "u")
        return &u;
    else if (name == "v")
        return &v;
    else if (name == "P")
        return &P;
    else {
        // make a variable to resolve any unknown
        auto i = varmap.find(name);
        if (i != varmap.end())
            return i->second;
        else {
            varmap[name] = new VectorRef();
            return varmap[name];
        }
    }
}

KSeExpr::ExprFunc *BasicExpression::resolveFunc(const std::string &name) const
{
    // check if it is builtin so we get proper behavior
    if (KSeExpr::ExprFunc::lookup(name))
        return nullptr;

    funcmap[name] = true;
    return &dummyFunc;
}
