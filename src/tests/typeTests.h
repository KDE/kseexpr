// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TYPETESTS_H
#define TYPETESTS_H

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <Expression.h>
#include <ExprNode.h>
#include <ExprFunc.h>

#include "TypeBuilder.h"
#include "TypePrinter.h"

using namespace KSeExpr;

/**
   @file typeTests.h
*/
//! Simple expression class to check all final types of tests
class TypeTesterExpr : public TypeBuilderExpr {
  public:
    typedef ExprType (*FindResultOne)(const ExprType &);
    typedef ExprType (*FindResultTwo)(const ExprType &, const ExprType &);
    typedef ExprType (*FindResultThree)(const ExprType &, const ExprType &, const ExprType &);

    TypePrintExaminer _examiner;
    KSeExpr::ConstWalker _walker;

    TypeTesterExpr() : TypeBuilderExpr(), _walker(&_examiner) {};

    TypeTesterExpr(const std::string &e) : TypeBuilderExpr(e), _walker(&_examiner) {};

    virtual ExprVarRef *resolveVar(const std::string &name) const {
        return TypeBuilderExpr::resolveVar(name);
    };

    ExprFunc *resolveFunc(const std::string &name) const {
        return TypeBuilderExpr::resolveFunc(name);
    };

    void doTest(const std::string &testStr, ExprType expectedResult, ExprType actualResult);

    void testOneVar(const std::string &testStr,
                    // SingleWholeTypeIterator::ProcType proc);
                    ExprType (*proc)(const ExprType &));

    void testTwoVars(const std::string &testStr,
                     // DoubleWholeTypeIterator::ProcType proc);
                     ExprType (*proc)(const ExprType &, const ExprType &));
};

#endif  // TYPETESTS_H
