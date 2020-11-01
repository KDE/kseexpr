// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
   @file imageSynth.cpp
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <png.h>
#include <vector>

#include <KSeExpr/Expression.h>
#include <KSeExpr/Interpreter.h>
#include <KSeExpr/PerformanceTimer.h>

namespace KSeExpr
{
//! Simple image synthesizer expression class to support our function grapher
class ImageSynthExpr : public Expression
{
public:
    //! Constructor that takes the expression to parse
    ImageSynthExpr(const std::string &expr)
        : Expression(expr)
    {
    }

    //! Simple variable that just returns its internal value
    struct Var : public ExprVarRef {
        Var(const double val)
            : ExprVarRef(ExprType().FP(1).Varying())
            , val(val)
        {
        }

        Var()
            : ExprVarRef(ExprType().FP(1).Varying())
        {
        }

        double val {0.0}; // independent variable
        void eval(double *result) override
        {
            result[0] = val;
        }

        void eval(const char **) override
        {
            assert(false);
        }
    };
    //! variable map
    mutable std::map<std::string, Var> vars;

    //! resolve function that only supports one external variable 'x'
    ExprVarRef *resolveVar(const std::string &name) const override
    {
        auto i = vars.find(name);
        if (i != vars.end())
            return &i->second;
        return nullptr;
    }
};
} // namespace KSeExpr

constexpr double clamp(double x)
{
    return std::max(0., std::min(255., x)); // NOLINT readability-magic-numbers
}

using namespace KSeExpr;

int main(int argc, char *argv[])
{
    if (argc != 5) { // NOLINT readability-magic-numbers
        std::cerr << "Usage: " << argv[0] << " <image file> <width> <height> <exprFile>" << std::endl;
        return 1;
    }

    // parse arguments
    const char *imageFile = argv[1];
    const char *exprFile = argv[4];
    size_t width = std::strtoul(argv[2], nullptr, 10);  // NOLINT readability-magic-numbers
    size_t height = std::strtoul(argv[3], nullptr, 10); // NOLINT readability-magic-numbers
    if (!width || !height) {
        std::cerr << "invalid width/height" << std::endl;
        return 1;
    }

    std::ifstream istream(exprFile);
    if (!istream) {
        std::cerr << "Cannot read file " << exprFile << std::endl;
        return 1;
    }
    std::string exprStr((std::istreambuf_iterator<char>(istream)), std::istreambuf_iterator<char>());
    ImageSynthExpr expr(exprStr);

    // make variables
    expr.vars["u"] = ImageSynthExpr::Var(0.);
    expr.vars["v"] = ImageSynthExpr::Var(0.);
    expr.vars["w"] = ImageSynthExpr::Var(width);
    expr.vars["h"] = ImageSynthExpr::Var(height);

    // check if expression is valid
    bool valid = expr.isValid();
    if (!valid) {
        std::cerr << "Invalid expression " << std::endl;
        std::cerr << expr.parseError() << std::endl;
        return 1;
    }
    if (!expr.returnType().isFP(3)) {
        std::cerr << "Expected color FP[3] got type " << expr.returnType().toString() << std::endl;
        return 1;
    }

    // evaluate expression
    std::cerr << "Evaluating expresion...from " << exprFile << std::endl;
    std::vector<unsigned char> image(width * height * 4);

    {
        PerformanceTimer evalTime("eval time");
        double one_over_width = 1. / width;
        double one_over_height = 1. / height;
        double &u = expr.vars["u"].val;
        double &v = expr.vars["v"].val;
        for (size_t row {}; row < height; row++) {
            for (size_t col {}; col < width; col++) {
                auto i = (row * width + col) * 4;
                u = one_over_width * (col + .5);  // NOLINT readability-magic-numbers
                v = one_over_height * (row + .5); // NOLINT readability-magic-numbers

                const double *result = expr.evalFP();

                // expr._interpreter->print();
                image[i] = clamp(result[0] * 256.);     // NOLINT readability-magic-numbers
                image[i + 1] = clamp(result[1] * 256.); // NOLINT readability-magic-numbers
                image[i + 2] = clamp(result[2] * 256.); // NOLINT readability-magic-numbers
                image[i + 3] = 255;                     // NOLINT readability-magic-numbers
            }
        }
    } // timer

    // write image as png
    std::cerr << "Writing image..." << imageFile << std::endl;
    std::unique_ptr<std::FILE, decltype(&std::fclose)> fp {fopen(imageFile, "wb"), &std::fclose};
    if (!fp) {
        perror("fopen");
        return 1;
    }
    png_structp png_ptr {png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)};
    png_infop info_ptr {png_create_info_struct(png_ptr)};
    png_init_io(png_ptr, fp.get());
    png_set_IHDR(png_ptr,
                 info_ptr,
                 width,
                 height,
                 8, // NOLINT readability-magic-numbers
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    std::vector<png_byte *> ptrs(height);
    for (size_t i {}; i < height; i++) {
        ptrs[i] = &image[width * i * 4];
    }
    png_set_rows(png_ptr, info_ptr, ptrs.data());
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
}
