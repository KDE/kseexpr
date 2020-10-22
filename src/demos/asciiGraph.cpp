// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KSeExpr/Expression.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace KSeExpr;

/**
   @file asciiGraph.cpp
*/
//! Simple expression class to support our function grapher
class GrapherExpr : public Expression
{
public:
    //! Constructor that takes the expression to parse
    GrapherExpr(const std::string &expr)
        : Expression(expr)
    {
    }

    //! set the independent variable
    void setX(double x_input)
    {
        x.val = x_input;
    }

private:
    //! Simple variable that just returns its internal value
    struct SimpleVar : public ExprVarRef {
        SimpleVar()
            : ExprVarRef(ExprType().FP(1).Varying())
        {
        }

        double val{0.0}; // independent variable
        void eval(double *result) override
        {
            result[0] = val;
        }

        void eval(const char **) override
        {
            assert(false);
        }
    };

    //! independent variable
    mutable SimpleVar x;

    //! resolve function that only supports one external variable 'x'
    ExprVarRef *resolveVar(const std::string &name) const override
    {
        if (name == "x")
            return &x;
        return nullptr;
    }
};

int main(int argc, char *argv[])
{
    std::string exprStr =
        "\
                         $val=.5*PI*x;\
                         7*sin(val)/val \
                         ";
    if (argc == 2) {
        exprStr = argv[1];
    }
    GrapherExpr expr(exprStr);

    if (!expr.isValid()) {
        std::cerr << "expression failed " << expr.parseError() << std::endl;
        exit(1);
    } else if (!expr.returnType().isFP(1)) {
        std::cerr << "Expected expression of type " << ExprType().FP(1).Varying().toString() << " got " << expr.returnType().toString() << std::endl;
        exit(1);
    }

    double xmin = -10; // NOLINT readability-magic-numbers
    double xmax = 10;  // NOLINT readability-magic-numbers
    double ymin = -10; // NOLINT readability-magic-numbers
    double ymax = 10;  // NOLINT readability-magic-numbers
    int w = 60;        // NOLINT readability-magic-numbers
    int h = 30;        // NOLINT readability-magic-numbers
    auto buffer = std::vector<char>(w * h, ' ');

    // draw x axis
    int j_zero = static_cast<int>((-ymin) / (ymax - ymin) * h);
    if (j_zero >= 0 && j_zero < h) {
        for (int i = 0; i < w; i++) {
            buffer[i + j_zero * w] = '-';
        }
    }
    // draw y axis
    int i_zero = static_cast<int>((-xmin) / (xmax - xmin) * w);
    if (i_zero >= 0 && i_zero < w) {
        for (int j = 0; j < h; j++) {
            buffer[i_zero + j * w] = '|';
        }
    }

    // evaluate the graph
    const int samplesPerPixel = 10;
    const double one_over_samples_per_pixel = 1. / samplesPerPixel;
    for (int i = 0; i < w; i++) {
        for (int sample = 0; sample < samplesPerPixel; sample++) {
            // transform from device to logical coordinatex
            double dx = double(sample) * one_over_samples_per_pixel;
            double x = double(dx + i) / double(w) * (xmax - xmin) + xmin;
            // prep the expression engine for evaluation
            expr.setX(x);
            const double *val = expr.evalFP();
            // evaluate and pull scalar value - currently does not work
            // TODO: fix eval and then use actual call
            // Vec3d val=0.0;//expr.evaluate();
            double y = val[0];
            // transform from logical to device coordinate
            int j = static_cast<int>((y - ymin) / (ymax - ymin) * h);
            // store to the buffer
            if (j >= 0 && j < h)
                buffer[i + j * w] = '#';
        }
    }

    // draw the graph from the buffer
    for (int j = h - 1; j >= 0; j--) {
        for (int i = 0; i < w; i++) {
            std::cout << buffer[i + j * w];
        }
        std::cout << std::endl;
    }

    return 0;
}
