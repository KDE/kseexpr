// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Expression.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "ExprWalker.h"
#include "ExprPatterns.h"
#include "ExprFunc.h"
#include "ControlSpec.h"

using namespace KSeExpr;
/**
   @file assignmentPatterns.cpp
*/
//! Simple expression class to list out variable uses
class PatternExpr : public Expression {
  public:
    struct DummyFuncX : ExprFuncX {
        DummyFuncX() : ExprFuncX(false) {};

        ExprType prep(ExprFuncNode* node, ExprType wanted, ExprVarEnv& env) {
            bool valid = true;
            for (int i = 0; i < node->numChildren(); i++) {
                if (!node->isStrArg(i)) valid &= node->child(i)->prep(false, env).isValid();
            }
            return wanted;
        }

        virtual bool isScalar() const {
            return true;
        };
        virtual ExprType retType() const {
            return ExprType().FP(1).Varying();
        };

        void eval(const ExprFuncNode* node, Vec3d& result) const { result = Vec3d(); }
    } dummyFuncX;
    mutable ExprFunc dummyFunc;

    //! Constructor that takes the expression to parse
    PatternExpr(const std::string& expr)
        : Expression(expr), dummyFunc(dummyFuncX, 0, 16), _examiner(), _walker(&_examiner) {};

    //! Empty constructor
    PatternExpr() : Expression(), dummyFunc(dummyFuncX, 0, 16), _examiner(), _walker(&_examiner) {};

    inline void walk() {
        _walker.walk(_parseTree);
    };
    void specs() {
        if (isValid()) {
            walk();
            printSpecs(_examiner);
        };
    };

  private:
    KSeExpr::SpecExaminer _examiner;
    KSeExpr::ConstWalker _walker;

    template <typename Examiner>
    void printSpecs(Examiner examiner) {
        if (isValid()) {
            for (int i = 0; i < examiner.length(); ++i) std::cout << examiner.spec(i)->toString() << std::endl;
        };
    };

    //! resolve function that only supports one external variable 'x'
    ExprVarRef* resolveVar(const std::string& name) const {
        return 0;
    };

    ExprFunc* resolveFunc(const std::string& name) const { return &dummyFunc; }
};

void quit(const std::string& str) {
    if (str == "quit" || str == "q") exit(0);
};

int main(int argc, char* argv[]) {
    PatternExpr expr;
    std::string str;

    std::cout << "KSeExpr Basic Pattern Matcher:";

    while (true) {
        std::cout << std::endl << "> ";
        // std::cin >> str;
        getline(std::cin, str);

        if (std::cin.eof()) {
            std::cout << std::endl;
            str = "q";
        };

        quit(str);
        expr.setExpr(str);

        if (!expr.isValid()) {
            std::cerr << "Expression failed: " << expr.parseError() << std::endl;
        } else {
            std::cout << "Expression value:" << std::endl;
            std::cout << "   " << expr.evalFP() << std::endl;
            std::cout << "Expression patterns:" << std::endl;
            expr.specs();
        };
    };

    return 0;
}
