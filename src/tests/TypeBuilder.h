// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TYPEBUILDER_H
#define TYPEBUILDER_H

#include <KSeExpr/Expression.h>
#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprNode.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>


using namespace KSeExpr;

/**
   @file TypeBuilder.h
*/
//! Simple expression class to build all intermediate types
class TypeBuilderExpr : public Expression {
  public:
    struct DummyFuncX : ExprFuncX {
        DummyFuncX() : ExprFuncX(false) {};

        virtual bool isScalar() const {
            return true;
        };
        virtual ExprType retType() const {
            return ExprType().FP(1).Varying();
        };
        // TODO: fix -- just a no-op function to get code to compile.
        ExprType prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnvBuilder & env) const override {
            return ExprType().None();
        };

        // TODO: fix -- just a no-op function to get code to compile.
        int buildInterpreter(const ExprFuncNode* node, Interpreter* interpreter) const override {
            return 0;
        };

        void eval(const ExprFuncNode* node, Vec3d& result) const { result = Vec3d(); }
    } dummyFuncX;
    mutable ExprFunc dummyFunc;
    mutable ExprFunc func;

    class ExprNothingVarRef : public ExprVarRef {
      public:
        ExprNothingVarRef(const ExprType& type) : ExprVarRef::ExprVarRef(type) {};

        void eval(const ExprVarNode* node, Vec3d& result) {
            result = 0.0;
        };

        // TODO: fix -- just a no-op function to get code to compile.
        void eval(double* result) override {};

        // TODO: fix -- just a no-op function to get code to compile.
        void eval(const char** resultStr) override {};
    };

    //! Empty constructor
    TypeBuilderExpr()
        : Expression::Expression(), dummyFunc(dummyFuncX, 0, 16), func(dummyFuncX, 3, 3),
          F1(ExprType().FP(1).Varying()), F2(ExprType().FP(2).Varying()), F3(ExprType().FP(3).Varying()),
          ST(ExprType().String().Varying()), SE(ExprType().Error().Varying()), LC(ExprType().FP(1).Constant()),
          LU(ExprType().FP(1).Uniform()), LV(ExprType().FP(1).Varying()), LE(ExprType().FP(1).Error()),
          v(ExprType().FP(1).Varying()), x(ExprType().FP(1).Varying()), y(ExprType().FP(1).Varying()),
          z(ExprType().FP(1).Varying()) {};

    TypeBuilderExpr(const std::string& e)
        : Expression::Expression(e), dummyFunc(dummyFuncX, 0, 16), func(dummyFuncX, 3, 3),
          F1(ExprType().FP(1).Varying()), F2(ExprType().FP(2).Varying()), F3(ExprType().FP(3).Varying()),
          ST(ExprType().String().Varying()), SE(ExprType().Error().Varying()), LC(ExprType().FP(1).Constant()),
          LU(ExprType().FP(1).Uniform()), LV(ExprType().FP(1).Varying()), LE(ExprType().FP(1).Error()),
          v(ExprType().FP(1).Varying()), x(ExprType().FP(1).Varying()), y(ExprType().FP(1).Varying()),
          z(ExprType().FP(1).Varying()) {};

    //! resolve function
    ExprVarRef* resolveVar(const std::string& name) const override {
        if (name == "F1") return &F1;
        if (name == "F2") return &F2;
        if (name == "F3") return &F3;
        if (name == "ST") return &ST;
        if (name == "SE") return &SE;
        if (name == "LC") return &LC;
        if (name == "LU") return &LU;
        if (name == "LV") return &LV;
        if (name == "LE") return &LE;
        if (name == "v") return &v;
        if (name == "x") return &x;
        if (name == "y") return &y;
        if (name == "z") return &z;

        /*else*/
        return nullptr;
    };

    ExprFunc* resolveFunc(const std::string& name) const override {
        if (name == "func")
            return &func;
        else
            return nullptr;
        /* else             return &dummyFunc; */
    };

    void setVar(const std::string& to, const std::string& from) {
        if (to == "v") v = ExprNothingVarRef(resolveVar(from)->type());
        if (to == "x") x = ExprNothingVarRef(resolveVar(from)->type());
        if (to == "y") y = ExprNothingVarRef(resolveVar(from)->type());
        if (to == "z") z = ExprNothingVarRef(resolveVar(from)->type());
        // TODO: rem ove
        // std::cerr<<"v is "<<v.type().toString()<<" from "<<from<<" is
        // "<<(resolveVar(from)->type().toString())<<std::endl;
    };

  private:
    // Various types
    mutable ExprNothingVarRef F1;
    mutable ExprNothingVarRef F2;
    mutable ExprNothingVarRef F3;
    mutable ExprNothingVarRef ST;
    mutable ExprNothingVarRef SE;
    mutable ExprNothingVarRef LC;
    mutable ExprNothingVarRef LU;
    mutable ExprNothingVarRef LV;
    mutable ExprNothingVarRef LE;

    // Use variables
    mutable ExprNothingVarRef v;
    mutable ExprNothingVarRef x;
    mutable ExprNothingVarRef y;
    mutable ExprNothingVarRef z;
};

#endif  // TYPEBUILDER_H
