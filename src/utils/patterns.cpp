// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprFuncX.h>
#include <KSeExpr/ExprPatterns.h>
#include <KSeExpr/ExprWalker.h>
#include <KSeExpr/Expression.h>
#include <KSeExprUI/ControlSpec.h>

using namespace KSeExpr;

//! Simple expression class to list out variable uses
class PatternExpr : public Expression
{
public:
    struct DummyFuncX : ExprFuncSimple {
        DummyFuncX()
            : ExprFuncSimple(false) {};

        ExprType prep(ExprFuncNode *node, bool, ExprVarEnvBuilder &envBuilder) const override
        {
            bool valid = true;
            for (int i = 0; i < node->numChildren(); i++) {
                if (!node->isStrArg(i))
                    valid &= node->child(i)->prep(false, envBuilder).isValid();
            }
            return valid ? ExprType().FP(1).Varying() : ExprType().Error();
        }

        ExprFuncNode::Data *evalConstant(const ExprFuncNode *, ArgHandle) const override
        {
            return nullptr;
        }

        void eval(ArgHandle args) override
        {
            double *out = &args.outFp;
            for (int k = 0; k < 3; k++)
                out[k] = 0.0;
        }
    } dummyFuncX;
    mutable ExprFunc dummyFunc;

    //! Constructor that takes the expression to parse
    PatternExpr(const std::string &expr)
        : Expression(expr)
        , dummyFunc(dummyFuncX, 0, 16)
        , _examiner()
        , _walker(&_examiner) {};

    //! Empty constructor
    PatternExpr()
        : Expression()
        , dummyFunc(dummyFuncX, 0, 16)
        , _examiner()
        , _walker(&_examiner) {};

    inline void walk()
    {
        _walker.walk(_parseTree);
    };
    void specs()
    {
        if (isValid()) {
            walk();
            printSpecs(_examiner);
        };
    };

private:
    KSeExpr::SpecExaminer _examiner;
    KSeExpr::ConstWalker _walker;

    template<typename Examiner> void printSpecs(Examiner examiner)
    {
        if (isValid()) {
            for (int i = 0; i < examiner.length(); ++i)
                std::cout << examiner.spec(i)->toString() << std::endl;
        };
    }

    //! resolve function that only supports one external variable 'x'
    ExprVarRef *resolveVar(const std::string &) const override
    {
        return nullptr;
    }

    ExprFunc *resolveFunc(const std::string &) const override
    {
        return &dummyFunc;
    }
};

void quit(const std::string &str)
{
    if (str == "quit" || str == "q")
        exit(0);
}

int main(int, char *[])
{
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
