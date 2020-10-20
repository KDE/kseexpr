// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <string>

#include "ExprBuiltins.h"
#include "ExprFunc.h"
#include "ExprNode.h"
#include "Expression.h"

namespace
{
// FuncTable - table of pre-defined functions
class FuncTable
{
public:
    void define(const char *name, const KSeExpr::ExprFunc &f, const char *docString = nullptr)
    {
        if (docString)
            funcmap[name] = FuncMapItem(std::string(docString), f);
        else
            funcmap[name] = FuncMapItem(name, f);
    }

    const KSeExpr::ExprFunc *lookup(const std::string &name)
    {
        const auto &i = funcmap.find(name);
        if (i == funcmap.end())
            return nullptr;
        else
            return &i->second.second;
    }

    void getFunctionNames(std::vector<std::string> &names)
    {
        for (const auto &i : funcmap)
            names.push_back(i.first);
    }

    std::string getDocString(const char *functionName)
    {
        const auto &i = funcmap.find(functionName);
        if (i == funcmap.end())
            return "";
        else
            return i->second.first;
    }

    size_t sizeInBytes() const
    {
        size_t totalSize = 0;
        for (const auto &it : funcmap) {
            totalSize += it.first.size() + sizeof(FuncMapItem);
            const KSeExpr::ExprFunc &function = it.second.second;
            if (const KSeExpr::ExprFuncX *funcx = function.funcx()) {
                totalSize += funcx->sizeInBytes();
            }
        }
        return totalSize;
    }

    KSeExpr::Statistics statistics() const
    {
        KSeExpr::Statistics statisticsDump;
        size_t totalSize = 0;
        for (const auto &it : funcmap) {
            totalSize += it.first.size() + sizeof(FuncMapItem);
            const KSeExpr::ExprFunc &function = it.second.second;
            if (const KSeExpr::ExprFuncX *funcx = function.funcx()) {
                funcx->statistics(statisticsDump);
            }
        }
        return statisticsDump;
    }

private:
    using FuncMapItem = std::pair<std::string, KSeExpr::ExprFunc>;
    using FuncMap = std::map<std::string, FuncMapItem>;
    FuncMap funcmap;
};

std::unique_ptr<FuncTable> Functions;
} // namespace

// ExprType ExprFuncX::prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnv & env) const
//{
//    /* call base node prep by default:
//       this passes wantVec to all the children and sets isVec true if any
//       child is a vec */
//    /* TODO: check that this is correct behavior */
//    return node->ExprNode::prep(scalarWanted, env);
//}

namespace KSeExpr
{
static std::mutex mutex;

void ExprFunc::init()
{
    std::lock_guard<std::mutex> locker(mutex);
    initInternal();
}

void ExprFunc::cleanup()
{
    std::lock_guard<std::mutex> locker(mutex);
    Functions = nullptr;
}

const ExprFunc *ExprFunc::lookup(const std::string &name)
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    const ExprFunc *ret = Functions->lookup(name);
    return ret;
}

inline static void defineInternal(const char *name, const ExprFunc &f)
{
    // THIS FUNCTION IS NOT THREAD SAFE, it assumes you have a mutex from callee
    // ALSO YOU MUST BE VERY CAREFUL NOT TO CALL ANYTHING THAT TRIES TO REACQUIRE MUTEX!
    Functions->define(name, f);
}

inline static void defineInternal3(const char *name, const ExprFunc &f, const char *docString)
{
    // THIS FUNCTION IS NOT THREAD SAFE, it assumes you have a mutex from callee
    // ALSO YOU MUST BE VERY CAREFUL NOT TO CALL ANYTHING THAT TRIES TO REACQUIRE MUTEX!
    Functions->define(name, f, docString);
}

void ExprFunc::initInternal()
{
    // THIS FUNCTION IS NOT THREAD SAFE, it assumes you have a mutex from callee
    // ALSO YOU MUST BE VERY CAREFUL NOT TO CALL ANYTHING THAT TRIES TO REACQUIRE MUTEX!

    if (Functions)
        return;
    Functions = std::make_unique<FuncTable>();
    KSeExpr::defineBuiltins(defineInternal, defineInternal3);
}

void ExprFunc::define(const char *name, const ExprFunc &f)
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    defineInternal(name, f);
}

void ExprFunc::define(const char *name, const ExprFunc &f, const char *docString)
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    defineInternal3(name, f, docString);
}

void ExprFunc::getFunctionNames(std::vector<std::string> &names)
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    Functions->getFunctionNames(names);
}

std::string ExprFunc::getDocString(const char *functionName)
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    std::string ret = Functions->getDocString(functionName);
    return ret;
}

size_t ExprFunc::sizeInBytes()
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    return Functions->sizeInBytes();
}

KSeExpr::Statistics ExprFunc::statistics()
{
    std::lock_guard<std::mutex> locker(mutex);
    if (!Functions)
        initInternal();
    return Functions->statistics();
}
} // namespace KSeExpr
