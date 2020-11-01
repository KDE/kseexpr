// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>
#include <set>

#include "ExprMultiExpr.h"

namespace KSeExpr
{
class GlobalVal : public ExprVarRef
{
public:
    GlobalVal(const std::string &varName, const KSeExpr::ExprType &et)
        : ExprVarRef(et)
        , varName(varName)
    {
    }
    std::set<DExpression *> users;
    std::string varName;
};

struct GlobalFP : public GlobalVal {
    GlobalFP(const std::string &varName, int dim)
        : GlobalVal(varName, ExprType().FP(dim).Varying())
    {
        val.assign(dim, 0);
    }

    std::vector<double> val;
    void eval(double *result) override
    {
        for (int i = 0; i < type().dim(); i++)
            result[i] = val[i];
    }
    void eval(const char **) override
    {
        assert(false);
    }
    bool isVec()
    {
        return type().dim() > 1;
    }
};

struct GlobalStr : public GlobalVal {
    GlobalStr(const std::string &varName)
        : GlobalVal(varName, ExprType().String().Varying())
    {
    }

    const char *val{nullptr};
    void eval(double *) override
    {
        assert(false);
    }
    void eval(const char **result) override
    {
        *result = val;
    }
    bool isVec()
    {
        return false;
    }
};
} // namespace KSeExpr

namespace
{
std::set<KSeExpr::DExpression *> getAffectedExpr(KSeExpr::GlobalVal *gv)
{
    std::set<KSeExpr::DExpression *> ret;

    std::set<KSeExpr::DExpression *> workList = gv->users;
    while (!workList.empty()) {
        KSeExpr::DExpression *de = *workList.begin();
        workList.erase(de);
        ret.insert(de);
        workList.insert(de->val->users.begin(), de->val->users.end());
    }

    return ret;
}

std::set<KSeExpr::DExpression *> getTransitiveOperandExpr(KSeExpr::DExpression *expr)
{
    std::set<KSeExpr::DExpression *> ret;

    std::set<KSeExpr::DExpression *> workList;
    workList.insert(expr);
    while (!workList.empty()) {
        KSeExpr::DExpression *de = *workList.begin();
        workList.erase(de);
        ret.insert(de);
        workList.insert(de->operandExprs.begin(), de->operandExprs.end());
    }

    return ret;
}

std::set<KSeExpr::DExpression *> tmpOperandExprs;
std::set<KSeExpr::GlobalVal *> tmpOperandVars;
} // namespace

namespace KSeExpr
{
DExpression::DExpression(const std::string &varName, Expressions &context, const std::string &e, const ExprType &type, EvaluationStrategy be)
    : Expression(e, type, be)
    , dContext(context)
{
    if (type.isFP())
        val = new GlobalFP(varName, type.dim());
    else if (type.isString())
        val = new GlobalStr(varName);
    else
        assert(false);

    operandExprs = dContext.AllExprs;
    operandVars = dContext.AllExternalVars;
    prepIfNeeded();
    operandExprs = tmpOperandExprs;
    operandVars = tmpOperandVars;
}

const std::string &DExpression::name() const
{
    return val->varName;
}

ExprVarRef *DExpression::resolveVar(const std::string &name) const
{
    // first time resolve var from all exprs & vars
    // then resolve var from used exprs & vars
    for (auto *operandExpr : operandExprs) {
        if (operandExpr->name() == name) {
            tmpOperandExprs.insert(operandExpr);
            operandExpr->val->users.insert(const_cast<DExpression *>(this));
            return operandExpr->val;
        }
    }

    for (auto *operandVar : operandVars) {
        if (operandVar->varName == name) {
            tmpOperandVars.insert(operandVar);
            operandVar->users.insert(const_cast<DExpression *>(this));
            return operandVar;
        }
    }

    addError(ErrorCode::UndeclaredVariable, {name}, 0, 0);
    return nullptr;
}

void DExpression::eval()
{
    if (_desiredReturnType.isFP()) {
        const double *ret = evalFP();
        auto *fpVal = dynamic_cast<GlobalFP *>(val);
        fpVal->val.assign(ret, ret + fpVal->val.size());
        return;
    }

    assert(_desiredReturnType.isString());
    auto *strVal = dynamic_cast<GlobalStr *>(val);
    strVal->val = evalStr();
}

Expressions::~Expressions()
{
    for (auto *AllExpr : AllExprs)
        delete AllExpr;

    for (auto *AllExternalVar : AllExternalVars)
        delete AllExternalVar;
}

VariableHandle Expressions::addExternalVariable(const std::string &variableName, ExprType seTy)
{
    std::pair<std::set<GlobalVal *>::iterator, bool> ret;

    if (seTy.isFP())
        ret = AllExternalVars.insert(new GlobalFP(variableName, seTy.dim()));
    else if (seTy.isString())
        ret = AllExternalVars.insert(new GlobalStr(variableName));
    else
        assert(false);

    return ret.first;
}

ExprHandle Expressions::addExpression(const std::string &varName, ExprType seTy, const std::string &expr)
{
    std::pair<std::set<DExpression *>::iterator, bool> ret;
    ret = AllExprs.insert(new DExpression(varName, *this, expr, seTy));
    return ret.first;
}

VariableSetHandle Expressions::getLoopVarSetHandle(VariableHandle vh)
{
    GlobalVal *thisvar = *vh;
    auto initSize = static_cast<unsigned>(thisvar->users.size());
    if (!initSize)
        return AllExternalVars.end();

    std::set<DExpression *> ret = getAffectedExpr(thisvar);
    exprToEval.insert(ret.begin(), ret.end());
    // std::cout << "exprToEval size is " << exprToEval.size() << std::endl;
    return vh;
}

void Expressions::setLoopVariable(VariableSetHandle handle, double *values, unsigned dim)
{
    if (handle == AllExternalVars.end())
        return;

    auto *thisvar = dynamic_cast<GlobalFP *>(*handle);
    assert(thisvar && "set value to variable with incompatible types.");

    assert(dim == thisvar->val.size());
    for (unsigned i = 0; i < dim; ++i)
        thisvar->val[i] = values[i];
}

void Expressions::setLoopVariable(VariableSetHandle handle, const char *values)
{
    if (handle == AllExternalVars.end())
        return;

    auto *thisvar = dynamic_cast<GlobalStr *>(*handle);
    assert(thisvar && "set value to variable with incompatible types.");
    thisvar->val = values;
}

void Expressions::setVariable(VariableHandle handle, double *values, unsigned dim)
{
    auto *thisvar = dynamic_cast<GlobalFP *>(*handle);
    assert(thisvar && "set value to variable with incompatible types.");

    assert(dim == thisvar->val.size());
    for (unsigned i = 0; i < dim; ++i)
        thisvar->val[i] = values[i];

    // eval loop invariant now.
    std::set<DExpression *> ret = getAffectedExpr(thisvar);
    for (auto *I : ret)
        I->eval();
}

void Expressions::setVariable(VariableHandle handle, const char *values)
{
    auto *thisvar = dynamic_cast<GlobalStr *>(*handle);
    assert(thisvar && "set value to variable with incompatible types.");
    thisvar->val = values;

    // eval loop invariant now.
    std::set<DExpression *> ret = getAffectedExpr(thisvar);
    for (auto *I : ret)
        I->eval();
}

bool Expressions::isValid() const
{
    bool ret = true;
    for (auto *AllExpr : AllExprs)
        ret &= AllExpr->isValid();
    return ret;
}

ExprEvalHandle Expressions::getExprEvalHandle(ExprHandle eh)
{
    // std::cout << "exprToEval size is " << exprToEval.size() << std::endl;

    DExpression *de = *eh;
    std::set<DExpression *> all = getTransitiveOperandExpr(de);
    // std::cout << "all size is " << all.size() << std::endl;

    std::vector<DExpression *>::iterator it;

    std::vector<DExpression *> ret1(all.size());
    it = std::set_intersection(all.begin(), all.end(), exprToEval.begin(), exprToEval.end(), ret1.begin());
    ret1.resize(it - ret1.begin());

    std::vector<DExpression *> ret2(ret1.size());
    it = std::set_difference(ret1.begin(), ret1.end(), exprEvaled.begin(), exprEvaled.end(), ret2.begin());
    ret2.resize(it - ret2.begin());

    exprEvaled.insert(ret2.begin(), ret2.end());

    // std::cout << "ret2 size is " << ret2.size() << std::endl;
    return std::make_pair(eh, ret2);
}

const std::vector<double> &Expressions::evalFP(ExprEvalHandle eeh)
{
    // std::cout << "eeh.second.size() is " << eeh.second.size() << std::endl;
    for (auto & I : eeh.second)
        I->eval();

    auto *thisvar = dynamic_cast<GlobalFP *>((*eeh.first)->val);

    //    std::cout << thisvar->val[0] << ","
    //            << thisvar->val[1] << ","
    //            << thisvar->val[2] << std::endl;
    return thisvar->val;
}

const char *Expressions::evalStr(ExprEvalHandle eeh)
{
    for (auto & I : eeh.second)
        I->eval();

    auto *thisvar = dynamic_cast<GlobalStr *>((*eeh.first)->val);
    return thisvar->val;
}
} // namespace KSeExpr
