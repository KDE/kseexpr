// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprNode.h>
#include <KSeExpr/Expression.h>
#include <KSeExpr/TypePrinter.h>

#include "TypeBuilder.h"

using namespace KSeExpr;

/**
   @file typeTests.h
*/
//! Simple expression class to check all final types of tests
class TypeTesterExpr : public TypeBuilderExpr {
  public:
    using FindResultOne = ExprType (*)(const ExprType &);
    using FindResultTwo = ExprType (*)(const ExprType &, const ExprType &);
    using FindResultThree = ExprType (*)(const ExprType &, const ExprType &, const ExprType &);

    TypePrintExaminer _examiner;
    KSeExpr::ConstWalker _walker;

    TypeTesterExpr() : TypeBuilderExpr(), _walker(&_examiner) {};

    TypeTesterExpr(const std::string &e) : TypeBuilderExpr(e), _walker(&_examiner) {};

    ExprVarRef *resolveVar(const std::string &name) const override {
        return TypeBuilderExpr::resolveVar(name);
    };

    ExprFunc *resolveFunc(const std::string &name) const override {
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
