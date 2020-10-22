// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ExprEnv.h"
#include "ExprType.h"
#include "Expression.h"

namespace KSeExpr
{
void ExprVarEnv::resetAndSetParent(ExprVarEnv *parent)
{
    _parent = parent;
}

ExprLocalVar *ExprVarEnv::find(const std::string &name)
{
    auto iter = _map.find(name);
    if (iter != _map.end())
        return iter->second.get();
    else if (_parent)
        return _parent->find(name);
    else
        return nullptr;
}

ExprLocalFunctionNode *ExprVarEnv::findFunction(const std::string &name)
{
    auto iter = _functions.find(name);
    if (iter != _functions.end())
        return iter->second;
    else if (_parent)
        return _parent->findFunction(name);
    else
        return nullptr;
}

ExprLocalVar const *ExprVarEnv::lookup(const std::string &name) const
{
    auto iter = _map.find(name);
    if (iter != _map.end())
        return iter->second.get();
    else if (_parent)
        return _parent->lookup(name);
    return nullptr;
}

void ExprVarEnv::addFunction(const std::string &name, ExprLocalFunctionNode *prototype)
{
    // go to parent until we are at root (all functions globally declared)
    if (_parent)
        _parent->addFunction(name, prototype);
    else {
        auto iter = _functions.find(name);
        if (iter != _functions.end())
            iter->second = prototype;
        else
            _functions.insert(std::make_pair(name, prototype));
    }
}

void ExprVarEnv::add(const std::string &name, std::unique_ptr<ExprLocalVar> var)
{
    auto iter = _map.find(name);
    if (iter != _map.end()) {
        // throw std::runtime_error("Invalid creation of existing variable in same scope!");
        shadowedVariables.emplace_back(std::move(iter->second));
        iter->second = std::move(var);
    } else
        _map.insert(std::make_pair(name, std::move(var)));
}

size_t ExprVarEnv::mergeBranches(const ExprType &type, ExprVarEnv &env1, ExprVarEnv &env2)
{
    using MakeMap = std::map<std::pair<ExprLocalVar *, ExprLocalVar *>, std::string>;
    MakeMap phisToMake;
    /// For each thing in env1 see if env2 has an entry
    for (auto &ienv : env1._map) {
        const std::string &name = ienv.first;
        ExprLocalVar *var = ienv.second.get();
        if (ExprLocalVar *env2Var = env2.find(name)) {
            phisToMake[std::make_pair(var, env2Var)] = name;
        }
    }
    /// For each thing in env2 see if env1 has an entry
    for (auto &ienv : env2._map) {
        const std::string &name = ienv.first;
        ExprLocalVar *var = ienv.second.get();
        if (ExprLocalVar *env1Var = env1.find(name)) {
            phisToMake[std::make_pair(env1Var, var)] = name;
        }
    }

    std::vector<std::pair<std::string, ExprLocalVarPhi *>> mergedVariablesInThisCall;
    for (auto& it : phisToMake) {
        auto newVar = std::make_unique<ExprLocalVarPhi>(type, it.first.first, it.first.second);
        mergedVariablesInThisCall.emplace_back(it.second, dynamic_cast<ExprLocalVarPhi *>(newVar.get()));
        add(it.second, std::move(newVar));
    }
    _mergedVariables.emplace_back(std::move(mergedVariablesInThisCall));
    return _mergedVariables.size() - 1;
}
} // namespace KSeExpr
