// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/Expression.h>
#include <KSeExpr/Vec.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define STACK_DEPTH 256

using namespace KSeExpr;

/**
   @file asciiCalculator.cpp
*/
//! Simple expression class to support our function calculator
class CalculatorExpr : public Expression
{
public:
    //! Constructor that takes the expression to parse
    CalculatorExpr(const std::string &expr)
        : Expression(expr)
    {
        for (int i = 0; i < STACK_DEPTH; i++) {
            stack.at(i).val = Vec<double, 3, false>(0.0);
            fail_stack.at(i) = false;
        }
    };

    //! Empty constructor
    CalculatorExpr()
        : Expression() {};

    //! Push current result on stack
    void push()
    {
        if (returnType().isString()) {
            evalStr();
        } else if (returnType().isFP()) {
            const double *val = evalFP();
            int dim = returnType().dim();
            for (int k = 0; k < 3; k++)
                std::cerr << val[k] << " ";
            std::cerr << std::endl;
            if (dim == 1)
                stack.at(_count).val = Vec<double, 3, false>(val[0]);
            else if (dim == 2)
                stack.at(_count).val = Vec<double, 3, false>(val[0], val[1], 0);
            else if (dim == 3)
                stack.at(_count).val = Vec<double, 3, true>(const_cast<double *>(&val[0]));
            else {
                std::cerr << "Return type FP(" << dim << ") ignoring" << std::endl;
            }

            _count++;
        }
    };

    //! Failed attempt; push 0 on stack
    void fail_push()
    {
        fail_stack.at(_count) = true;
        stack.at(_count).val = Vec<double, 3, false>(0.0);
        _count++;
    };

    Vec<double, 3, false> peek()
    {
        return stack.at(_count - 1).val;
    }

    int count() const
    {
        return _count;
    };

private:
    //! Simple variable that just returns its internal value
    struct SimpleVar : public ExprVarRef {
        SimpleVar()
            : ExprVarRef(ExprType().FP(3).Varying())
            , val(0.0)
        {
        }

        Vec<double, 3, false> val; // independent variable

        void eval(double *result) override
        {
            for (int k = 0; k < 3; k++)
                result[k] = val[k];
        }

        void eval(const char **) override
        {
            assert(false);
        }
    };

    //! previous computations
    mutable std::array<SimpleVar, STACK_DEPTH> stack;
    mutable std::array<bool, STACK_DEPTH> fail_stack {};
    mutable int _count {0};

    //! resolve function that only supports one external variable 'x'
    ExprVarRef *resolveVar(const std::string &name) const override
    {
        if (name[0] == '_') {
            int position = std::stoi(name.substr(1, name.size() - 1));
            if (position >= count())
                std::cerr << "Use of unused result line." << std::endl;
            if (fail_stack.at(position))
                std::cerr << "Use of invalid result line." << std::endl;
            return &(stack.at(position));
        };
        addError(ErrorCode::UndeclaredVariable, {name}, 0, 0);
        return nullptr;
    };
};

int main(int, char *[])
{
    std::cout << "KSeExpr Basic Calculator";

    CalculatorExpr expr;
    while (true) {
        std::string str;
        std::cout << std::endl << expr.count() << "> ";
        // std::cin >> str;
        getline(std::cin, str);

        if (std::cin.eof()) {
            std::cout << std::endl;
            str = "q";
        };

        if (str == "quit" || str == "q")
            break;
        expr.setDesiredReturnType(ExprType().FP(3));
        expr.setExpr(str);

        if (!expr.isValid()) {
            expr.fail_push();
            std::cerr << "Expression failed: " << expr.parseError() << std::endl;
        } else {
            expr.push();
            std::cout << "   " << expr.peek();
        }
    }
    ExprFunc::cleanup();
    return 0;
}
