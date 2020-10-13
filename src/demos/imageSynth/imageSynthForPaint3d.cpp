// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
/**
   @file imageSynth.cpp
*/
#include <map>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <png.h>
#include <fstream>

#include <KSeExpr/Expression.h>
#include <KSeExpr/Vec.h>
#include <KSeExpr/Interpreter.h>
#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprFuncX.h>
#include <KSeExpr/Timer.h>

namespace KSeExpr {
class ImageSynthRandFuncX : public ExprFuncSimple {

    struct Data : public ExprFuncNode::Data {
        std::vector<std::pair<int, int> > ranges;
        std::string format;
    };

    virtual ExprType prep(ExprFuncNode* node, bool wantScalar, ExprVarEnvBuilder& envBuilder) const {
        bool valid = true;
        for (int i = 0; i < node->numChildren(); i++)
            valid &= node->checkArg(i, ExprType().FP(1).Varying(), envBuilder);
        return valid ? ExprType().FP(1).Varying() : ExprType().Error();
    }

    virtual ExprFuncNode::Data* evalConstant(const ExprFuncNode* node, ArgHandle args) const { return new Data; }

    virtual void eval(ArgHandle args) {
        if (args.nargs() >= 2) {
            args.outFp = (args.inFp<1>(0)[0] - args.inFp<1>(1)[0]) / 2.0;
        } else
            args.outFp = 0.5;
    }

public:
    ImageSynthRandFuncX()
        : ExprFuncSimple(true)
    {
    } // Thread Safe
    virtual ~ImageSynthRandFuncX()
    {
    }
} imageSynthRand;

// map(string name, [float format-arg], [float u], [float v], [int channel])
class MapFuncX : public ExprFuncSimple {
    struct Data : public ExprFuncNode::Data {
        std::vector<std::pair<int, int> > ranges;
        std::string format;
    };

    virtual ExprType prep(ExprFuncNode* node, bool wantScalar, ExprVarEnvBuilder& envBuilder) const {
        bool valid = true;
        valid &= node->checkArg(0, ExprType().String().Constant(), envBuilder);
        for (int i = 1; i < node->numChildren(); i++)
            valid &= node->checkArg(i, ExprType().FP(1).Varying(), envBuilder);
        return valid ? ExprType().FP(3).Varying() : ExprType().Error();
    }

    virtual ExprFuncNode::Data* evalConstant(const ExprFuncNode* node, ArgHandle args) const { return new Data; }

    virtual void eval(ArgHandle args) {
        double* out = &args.outFp;

        double val = 0.5;
        int num = args.nargs();
        if (num > 2)
            for (int k = 2; k < num; k++) val += args.inFp<1>(k)[0];

        for (int k = 0; k < 3; k++) out[k] = val;
    }

  public:
    MapFuncX() : ExprFuncSimple(true) {}  // Thread Safe
    virtual ~MapFuncX() {}
} map;

// triplanar(string name, [vector scale], [float blend], [vector rotation], [vector translation])
class TriplanarFuncX : public ExprFuncSimple {
    struct Data : public ExprFuncNode::Data {
        std::vector<std::pair<int, int> > ranges;
        std::string format;
    };

    virtual ExprType prep(ExprFuncNode* node, bool wantScalar, ExprVarEnvBuilder& envBuilder) const {
        bool valid = true;
        valid &= node->checkArg(0, ExprType().String().Constant(), envBuilder);
        for (int i = 1; i < node->numChildren(); i++)
            valid &= node->checkArg(i, ExprType().FP(1).Varying(), envBuilder);
        return valid ? ExprType().FP(3).Varying() : ExprType().Error();
    }

    virtual ExprFuncNode::Data* evalConstant(const ExprFuncNode* node, ArgHandle args) const { return new Data; }

    virtual void eval(ArgHandle args) {
        double* out = &args.outFp;

        double val = 0.5;
        int num = args.nargs();
        if (num > 1)
            for (int k = 1; k < num; k++) val += (args.inFp<3>(k)[0] + args.inFp<3>(k)[1] + args.inFp<3>(k)[2]);

        for (int k = 0; k < 3; k++) out[k] = val;
    }

  public:
    TriplanarFuncX() : ExprFuncSimple(true) {}  // Thread Safe
    virtual ~TriplanarFuncX() {}
} triplanar;
}

static const char* rand_docstring = "rand\n";
static const char* map_docstring = "map\n";
static const char* triplanar_docstring = "triplanar\n";

using namespace KSeExpr;

//! Simple image synthesizer expression class to support our function grapher
class ImageSynthExpr : public Expression {
  public:
    //! Constructor that takes the expression to parse
    ImageSynthExpr(const std::string& expr) : Expression(expr, ExprType().FP(3)) {}

    //! Simple variable that just returns its internal value
    struct Var : public ExprVarRef {
        Var(const double val) : ExprVarRef(ExprType().FP(1).Varying()), val(val) {}

        Var() : ExprVarRef(ExprType().FP(1).Varying()), val(0.0) {}

        double val;  // independent variable
        void eval(double* result) { result[0] = val; }

        void eval(const char** result) { assert(false); }
    };

    struct VecVar : public ExprVarRef {
        VecVar() : ExprVarRef(ExprType().FP(3).Varying()), val(0.0) {}

        Vec<double, 3, false> val;  // independent variable

        void eval(double* result) {
            for (int k = 0; k < 3; k++) result[k] = val[k];
        }

        void eval(const char** reuslt) {}
    };

    //! variable map
    mutable std::map<std::string, Var> vars;
    mutable std::map<std::string, VecVar> vecvars;

    //! resolve function that only supports one external variable 'x'
    ExprVarRef* resolveVar(const std::string& name) const {
        {
            std::map<std::string, Var>::iterator i = vars.find(name);
            if (i != vars.end()) return &i->second;
        }
        {
            std::map<std::string, VecVar>::iterator i = vecvars.find(name);
            if (i != vecvars.end()) return &i->second;
        }
        return 0;
    }
};

double clamp(double x) { return std::max(0., std::min(255., x)); }

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <image file> <width> <height> <exprFile>" << std::endl;
        return 1;
    }

    // Disney: this function overrides the existing rand() -- amyspark
    ExprFunc::define("rand", ExprFunc(KSeExpr::imageSynthRand, 0, 3), rand_docstring);
    ExprFunc::define("map", ExprFunc(KSeExpr::map, 1, 4), map_docstring);
    ExprFunc::define("triplanar", ExprFunc(KSeExpr::triplanar, 1, 5), triplanar_docstring);

    // parse arguments
    const char* imageFile = argv[1];
    const char* exprFile = argv[4];
    int width = atoi(argv[2]), height = atoi(argv[3]);
    if (width < 0 || height < 0) {
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

    expr.vars["faceId"] = ImageSynthExpr::Var(0.);
    expr.vecvars["P"] = ImageSynthExpr::VecVar();
    expr.vecvars["Cs"] = ImageSynthExpr::VecVar();
    expr.vecvars["Ci"] = ImageSynthExpr::VecVar();

    // check if expression is valid
    bool valid = expr.isValid();
    if (!valid) {
        std::cerr << "Invalid expression " << std::endl;
        std::cerr << expr.parseError() << std::endl;
        return 1;
    }
    //    if(!expr.returnType().isFP(3)){
    //        std::cerr<<"Expected color FP[3] got type "<<expr.returnType().toString()<<std::endl;
    //        return 1;
    //    }

    // evaluate expression
    std::cerr << "Evaluating expresion...from " << exprFile << std::endl;
    unsigned char* image = new unsigned char[width * height * 4];
    double one_over_width = 1. / width, one_over_height = 1. / height;
    double& u = expr.vars["u"].val;
    double& v = expr.vars["v"].val;

    double& faceId = expr.vars["faceId"].val;
    Vec<double, 3, false>& P = expr.vecvars["P"].val;
    Vec<double, 3, false>& Cs = expr.vecvars["Cs"].val;
    Vec<double, 3, false>& Ci = expr.vecvars["Ci"].val;

    unsigned char* pixel = image;

    {
        PrintTiming timer("eval time: ");
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                u = one_over_width * (col + .5);
                v = one_over_height * (row + .5);

                faceId = floor(u * 5);
                P[0] = u * 10;
                P[1] = v * 10;
                P[2] = 0.5 * 10;
                Cs[0] = 0.;
                Cs[1] = 0.4;
                Cs[2] = 0.6;
                Ci[0] = 0.;
                Ci[1] = 0.4;
                Ci[2] = 0.6;

                const double* result = expr.evalFP();

                //            expr._interpreter->print();
                pixel[0] = clamp(result[0] * 256.);
                pixel[1] = clamp(result[1] * 256.);
                pixel[2] = clamp(result[2] * 256.);
                pixel[3] = 255;
                pixel += 4;
            }
        }
    }

    // write image as png
    std::cerr << "Writing image..." << imageFile << std::endl;
    FILE* fp = fopen(imageFile, "wb");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    png_structp png_ptr;
    png_infop info_ptr;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    int color_type = PNG_COLOR_TYPE_RGBA;
    png_set_IHDR(png_ptr,
                 info_ptr,
                 width,
                 height,
                 8,
                 color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    const unsigned char* ptrs[height];
    for (int i = 0; i < height; i++) {
        ptrs[i] = &image[width * i * 4];
    }
    png_set_rows(png_ptr, info_ptr, (png_byte**)ptrs);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

    fclose(fp);
}
