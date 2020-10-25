// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * @file BasicExpression.h
 * @brief A basic expression context for the expression previewer
 * @author  aselle
 */

#pragma once

#include <KSeExpr/ExprFunc.h>
#include <KSeExpr/ExprNode.h>
#include <KSeExpr/Expression.h>
#include <map>

class BasicExpression : public KSeExpr::Expression
{
public:
    struct ScalarRef : public KSeExpr::ExprVarRef {
        double value {};
        ScalarRef()
            : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(1).Varying())
        {
        }
        void eval(double *result) override
        {
            result[0] = value;
        }
        void eval(const char **) override
        {
            assert(false);
        }
    };

    struct VectorRef : public KSeExpr::ExprVarRef {
        KSeExpr::Vec3d value;
        VectorRef()
            : KSeExpr::ExprVarRef(KSeExpr::ExprType().FP(3).Varying())
            , value(0.0)
        {
        }
        void eval(double *result) override
        {
            for (int k = 0; k < 3; k++)
                result[k] = value[k];
        };
        void eval(const char **) override
        {
            assert(false);
        }
    };

    struct DummyFuncX : KSeExpr::ExprFuncSimple {
        DummyFuncX()
            : KSeExpr::ExprFuncSimple(true)
        {
        }

        ~DummyFuncX() override = default;
        DummyFuncX &operator=(DummyFuncX &&) = default;
        DummyFuncX &operator=(const DummyFuncX &) = default;
        DummyFuncX(DummyFuncX &&) = default;
        DummyFuncX(const DummyFuncX &) = default;

        KSeExpr::ExprType prep(KSeExpr::ExprFuncNode *node, bool, KSeExpr::ExprVarEnvBuilder &envBuilder) const override
        {
            bool valid = true;
            int nargs = node->numChildren();
            for (int i = 0; i < nargs; i++)
                valid &= node->checkArg(i, KSeExpr::ExprType().FP(3).Constant(), envBuilder);
            return valid ? KSeExpr::ExprType().FP(3).Varying() : KSeExpr::ExprType().Error();
        }

        KSeExpr::ExprFuncNode::Data *evalConstant(const KSeExpr::ExprFuncNode *, ArgHandle) const override
        {
            return new KSeExpr::ExprFuncNode::Data();
        }

        void eval(ArgHandle args) override
        {
            double *out = &args.outFp;
            for (int i = 0; i < 3; i++)
                out[i] = 0.0;
        }
    } dummyFuncX;
    mutable KSeExpr::ExprFunc dummyFunc;

    mutable ScalarRef u;
    mutable ScalarRef v;
    mutable VectorRef P;

    using VARMAP = std::map<std::string, VectorRef *>;
    mutable VARMAP varmap;
    using FUNCMAP = std::map<std::string, bool>;
    mutable FUNCMAP funcmap;

    BasicExpression(const std::string &expr, const KSeExpr::ExprType &type = KSeExpr::ExprType().FP(3));
    ~BasicExpression() override;

    BasicExpression &operator=(BasicExpression &&) = delete;
    BasicExpression &operator=(const BasicExpression &) = delete;
    BasicExpression(BasicExpression &&) = delete;
    BasicExpression(const BasicExpression &) = delete;

    KSeExpr::ExprVarRef *resolveVar(const std::string &name) const override;
    KSeExpr::ExprFunc *resolveFunc(const std::string &name) const override;
    void setExpr(const std::string &str);
    void clearVars();
};
