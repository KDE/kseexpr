// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0

/*
* @file BasicExpression.h
* @brief A basic expression context for the expression previewer
* @author  aselle
*/

#ifndef BasicExpression_h
#define BasicExpression_h

#include <map>
#include <KSeExpr/Expression.h>
#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprNode.h>

class BasicExpression : public KSeExpr::Expression {
  public:
    struct ScalarRef : public KSeExpr::ExprVarRef {
        double value;
        ScalarRef() : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying()), value(0.0) {}
        void eval(double* result) { result[0] = value; }
        void eval(const char** result) { assert(false); }
    };

    struct VectorRef : public KSeExpr::ExprVarRef {
        KSeExpr::Vec3d value;
        VectorRef() : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(3).Varying()), value(0.0) {}
        void eval(double* result) {
            for (int k = 0; k < 3; k++) result[k] = value[k];
        };
        void eval(const char** result) {
            assert(false);
        };
    };

    struct DummyFuncX : KSeExpr::ExprFuncSimple {
        DummyFuncX() : KSeExpr::ExprFuncSimple(true) {}
        virtual ~DummyFuncX() {}

        virtual KSeExpr::ExprType prep(KSeExpr::ExprFuncNode* node,
                                       bool scalarWanted,
                                       KSeExpr::ExprVarEnvBuilder& envBuilder) const {
            bool valid = true;
            int nargs = node->numChildren();
            for (int i = 0; i < nargs; i++)
                valid &= node->checkArg(i, KSeExpr::ExprType().FP(3).Constant(), envBuilder);
            return valid ? KSeExpr::ExprType().FP(3).Varying() : KSeExpr::ExprType().Error();
        }

        virtual KSeExpr::ExprFuncNode::Data* evalConstant(const KSeExpr::ExprFuncNode* node, ArgHandle args) const {
            return new KSeExpr::ExprFuncNode::Data();
        }

        virtual void eval(ArgHandle args) {
            double* out = &args.outFp;
            for (int i = 0; i < 3; i++) out[i] = 0.0;
        }
    } dummyFuncX;
    mutable KSeExpr::ExprFunc dummyFunc;

    mutable ScalarRef u;
    mutable ScalarRef v;
    mutable VectorRef P;

    typedef std::map<std::string, VectorRef*> VARMAP;
    mutable VARMAP varmap;
    typedef std::map<std::string, bool> FUNCMAP;
    mutable FUNCMAP funcmap;

    BasicExpression(const std::string& expr, const KSeExpr::ExprType& type = KSeExpr::ExprType().FP(3));
    virtual ~BasicExpression();

    KSeExpr::ExprVarRef* resolveVar(const std::string& name) const;
    KSeExpr::ExprFunc* resolveFunc(const std::string& name) const;
    void setExpr(const std::string& str);
    void clearVars();
};

#endif
