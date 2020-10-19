// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <vector>
#include <iostream>
#include <cmath>
#include <typeinfo>
#include <KSeExpr/ExprNode.h>
#include <KSeExpr/ExprWalker.h>
#include <KSeExpr/Expression.h>
using namespace KSeExpr;

class Expr : public Expression {
  public:
    void walk() {}

    struct Var : public ExprVarRef {
        Var() : ExprVarRef(ExprType().Varying().FP(3)) {}
        double val;
        void eval(double* result) {
            for (int k = 0; k < 3; k++) result[k] = val;
        }
        void eval(const char** result) { assert(false); }
    };
    mutable Var X;

    struct VarStr : public ExprVarRef {
        VarStr() : ExprVarRef(ExprType().Varying().String()) {}
        void eval(double* result) { assert(false); }

        void eval(const char** resultStr) { resultStr[0] = "testo"; }
    };
    mutable VarStr s;

    ExprVarRef* resolveVar(const std::string& name) const {
        std::cerr << "trying to resolve " << name << std::endl;
        if (name == "X") return &X;
        if (name == "s") return &s;
        return 0;
    }
};

int main(int argc, char* argv[]) {
    std::cerr << "fun fun" << std::endl;
    Expr expr;
    expr.setExpr(argv[1]);
    if (!expr.isValid()) {
        std::cerr << "parse error " << expr.parseError() << std::endl;
    } else {
        // pre eval
        std::cerr << "pre eval interp" << std::endl;
        expr.debugPrintInterpreter();
        std::cerr << "starting eval interp" << std::endl;

        // expr._interpreter->print();
        double sum = 0;
        //        for(int i=0;i<200000000;i++){
        for (int i = 0; i < 5; i++) {
            std::cerr << "eval iter " << i << " ";
            expr.X.val = (double)i;
            const double* d = expr.evalFP();
            for (int k = 0; k < expr.returnType().dim(); k++) std::cerr << d[k] << " ";
            std::cerr << std::endl;
            sum += d[0];
        }
        std::cerr << "sum " << sum << std::endl;
        expr.debugPrintInterpreter();
    }

    return 0;
}
