// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Expression.h"

namespace KSeExpr
{
class DExpression;
class GlobalVal;
class Expressions;

using VariableHandle = std::set<GlobalVal *>::iterator;
using VariableSetHandle = std::set<GlobalVal *>::iterator;
using ExprHandle = std::set<DExpression *>::iterator;
using ExprEvalHandle = std::pair<ExprHandle, std::vector<DExpression *> >;

class DExpression : public Expression
{
    Expressions &dContext;

public:
    DExpression(const std::string &varName, Expressions &context, const std::string &e, const ExprType &type = ExprType().FP(3), EvaluationStrategy be = defaultEvaluationStrategy);

    mutable std::set<DExpression *> operandExprs;
    mutable std::set<GlobalVal *> operandVars;

    GlobalVal *val;
    const std::string &name() const;
    ExprVarRef *resolveVar(const std::string &name) const override;
    void eval();
};

class Expressions
{
    std::set<DExpression *> exprToEval;
    std::set<DExpression *> exprEvaled;

public:
    std::set<DExpression *> AllExprs;
    std::set<GlobalVal *> AllExternalVars;

    // Expressions(int numberOfEvals=1);
    Expressions() = default;
    ~Expressions();
    Expressions(const Expressions&) = default;
    Expressions& operator=(const Expressions &) = default;
    Expressions(Expressions &&) = default;
    Expressions &operator=(Expressions &&) = default;

    VariableHandle addExternalVariable(const std::string &variableName, ExprType seTy);
    ExprHandle addExpression(const std::string &varName, ExprType seTy, const std::string &expr);

    VariableSetHandle getLoopVarSetHandle(VariableHandle vh);
    void setLoopVariable(VariableSetHandle handle, double *values, unsigned dim);
    void setLoopVariable(VariableSetHandle handle, double value)
    {
        setLoopVariable(handle, &value, 1);
    }
    void setLoopVariable(VariableSetHandle handle, const char *values);

    void setVariable(VariableHandle handle, double *values, unsigned dim);
    void setVariable(VariableHandle handle, double value)
    {
        setVariable(handle, &value, 1);
    }
    void setVariable(VariableHandle handle, const char *values);

    bool isValid() const;
    void getErrors(std::vector<std::string> &errors) const;
    // bool isVariableUsed(VariableHandle variableHandle) const;

    ExprEvalHandle getExprEvalHandle(ExprHandle eh);
    const std::vector<double> &evalFP(ExprEvalHandle eeh);
    const char *evalStr(ExprEvalHandle eeh);

    void resetEval()
    {
        exprToEval.clear();
        exprEvaled.clear();
    }

    void reset()
    {
        resetEval();
        AllExprs.clear();
        AllExternalVars.clear();
    }
};
} // namespace KSeExpr
