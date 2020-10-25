// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include <vector>

#include "ExprLLVM.h"
#include "ExprType.h"

namespace KSeExpr
{
class ExprVarRef;
class ExprLocalVar;
class ExprNode;
class ExprLocalFunctionNode;
class Interpreter;

//! ExprLocalVar reference, all local variables in seexpr are subclasses of this or this itself
class ExprLocalVar
{
private:
    ExprType _type;
    ExprLocalVar *_phi {nullptr};
    mutable LLVM_VALUE _varPtr {0};

public:
    ExprLocalVar(const ExprType &type)
        : _type(type)
    {
    }

    virtual ~ExprLocalVar() = default;
    ExprLocalVar(ExprLocalVar &) = default;
    ExprLocalVar(ExprLocalVar &&) = default;
    ExprLocalVar &operator=(const ExprLocalVar &) = default;
    ExprLocalVar &operator=(ExprLocalVar &&) = default;

    //! get the primary representative phi node (i.e. the global parent of a dependent phi node)
    const ExprLocalVar *getPhi() const
    {
        return _phi;
    }
    //! returns type of the variable
    ExprType type() const
    {
        return _type;
    }

    //! setter for variable type
    virtual void setType(const ExprType &type)
    {
        _type = type;
    }
    //! sets the representative phi node (like a brute force set unioning operation) phi is the set representative
    virtual void setPhi(ExprLocalVar *phi)
    {
        _phi = phi;
    }

    //! LLVM value that has been allocated
    virtual LLVM_VALUE codegen(LLVM_BUILDER, const std::string &, LLVM_VALUE) LLVM_BODY;

    //! LLVM value that has been pre-done
    virtual LLVM_VALUE varPtr()
    {
        return _varPtr;
    }

    //! Allocates variable for interpreter
    int buildInterpreter(Interpreter *interpreter) const;
};

//! ExprLocalVar join (merge) references. Remembers which variables are possible assigners to this
// This is basically like single assignment form inspired. hence the phi node nomenclature.
class ExprLocalVarPhi : public ExprLocalVar
{
public:
    ExprLocalVarPhi(const ExprType &condLife, ExprLocalVar *thenVar, ExprLocalVar *elseVar)
        : ExprLocalVar(ExprType())
        , _thenVar(thenVar)
        , _elseVar(elseVar)
    {
        // find the compatible common-denominator lifetime
        ExprType firstType = _thenVar->type();
        ExprType secondType = _elseVar->type();
        if (ExprType::valuesCompatible(_thenVar->type(), _elseVar->type())) {
            setType(((firstType.isFP(1) ? secondType : firstType).setLifetime(firstType, secondType)));
        }
        // lifetime should be the minimum (error=0,varying=1,uniform=2,constant=3).
        // i.e. you can only guarantee something is constant if the condition, ifvar, and else var are the same
        setType(type().setLifetime(firstType, secondType, condLife));
    }

    bool valid() const
    {
        return !type().isError();
    }

    void setPhi(ExprLocalVar *phi) override
    {
        ExprLocalVar::setPhi(phi);
        _thenVar->setPhi(phi);
        _elseVar->setPhi(phi);
    }

    ExprNode *_condNode {nullptr};
    ExprLocalVar *_thenVar {nullptr}, *_elseVar {nullptr};
};

//! Variable scope for tracking variable lookup
class ExprVarEnv
{
private:
    using VarDictType = std::map<std::string, std::unique_ptr<ExprLocalVar>>;
    VarDictType _map;
    using FuncDictType = std::map<std::string, ExprLocalFunctionNode *>;
    FuncDictType _functions;

    //! Variables that have been superceded (and thus are inaccessible)
    // i.e. a=3;a=[1,2,3];a=[2];a will  yield 2 entries in shadowedVariables
    std::vector<std::unique_ptr<ExprLocalVar>> shadowedVariables;

    //! Keep track of all merged variables in
    std::vector<std::vector<std::pair<std::string, ExprLocalVarPhi *>>> _mergedVariables;

    //! Parent variable environment has all variablesf rom previou scope (for lookup)
    ExprVarEnv *_parent {nullptr};

protected:
    ExprVarEnv(ExprVarEnv &other);
    ExprVarEnv &operator=(const ExprVarEnv &other);

public:
    // TODO: figure out when anotherOwns is needed
    //! Create a scope with no parent
    ExprVarEnv() = default;

    ~ExprVarEnv() = default;

    ExprVarEnv(ExprVarEnv &&) = default;
    ExprVarEnv& operator=(ExprVarEnv&&) = default;

    //! Resets the scope (deletes all variables) and sets parent
    void resetAndSetParent(ExprVarEnv *parent);
    //! Find a function by name (recursive to parents)
    ExprLocalFunctionNode *findFunction(const std::string &name);
    //! Find a variable name by name (recursive to parents)
    ExprLocalVar *find(const std::string &name);
    //! Find a const variable reference name by name (recursive to parents)
    ExprLocalVar const *lookup(const std::string &name) const;
    //! Add a function
    void addFunction(const std::string &name, ExprLocalFunctionNode *prototype);
    //! Add a variable refernece
    void add(const std::string &name, std::unique_ptr<ExprLocalVar> var);
    //! Add all variables into scope by name, but modify their lifetimes to the given type's lifetime
    //    void add(ExprVarEnv & env,const ExprType & modifyingType);
    //! Checks if each branch shares the same items and the same types!
    // static bool branchesMatch(const ExprVarEnv & env1, const ExprVarEnv & env2);
    size_t mergeBranches(const ExprType &type, ExprVarEnv &env1, ExprVarEnv &env2);
    // Code generate merges.
    LLVM_VALUE codegenMerges(LLVM_BUILDER, int) LLVM_BODY; // NOLINT
    // Query merges
    std::vector<std::pair<std::string, ExprLocalVarPhi *>> &merge(size_t index)
    {
        return _mergedVariables[index];
    }
};

//! Variable scope builder is used by the type checking and code gen to track visiblity of variables and changing of
// scopes
// It is inspired by IRBuilder's notion of a basic block insertion point
class ExprVarEnvBuilder
{
public:
    //! Creates an empty builder with one current scope entry
    ExprVarEnvBuilder()
    {
        reset();
    }
    //! Reset to factory state (one empty environment that is current)
    void reset()
    {
        std::unique_ptr<ExprVarEnv> newEnv(new ExprVarEnv);
        _currentEnv = newEnv.get();
        all.emplace_back(std::move(newEnv));
    }
    //! Return the current variable scope
    ExprVarEnv *current()
    {
        return _currentEnv;
    }
    //! Set a new current variable scope
    void setCurrent(ExprVarEnv *env)
    {
        _currentEnv = env;
    }
    //! Create a descendant scope from the provided parent, does not clobber current
    ExprVarEnv *createDescendant(ExprVarEnv *parent)
    {
        std::unique_ptr<ExprVarEnv> newEnv(new ExprVarEnv);
        newEnv->resetAndSetParent(parent);
        all.emplace_back(std::move(newEnv));
        return all.back().get();
    }

private:
    //! All owned symbol tables
    std::vector<std::unique_ptr<ExprVarEnv>> all;
    //! The current symbol table (should be a pointer owned by all)
    ExprVarEnv *_currentEnv {nullptr};
};

//! Evaluation result.
struct ExprEvalResult {
    ExprEvalResult() = default;
    ExprEvalResult(int n, double *fp)
        : n(n)
        , fp(fp)
    {
    }
    ExprEvalResult(const char **c)
        : n(1)
        , str(c)
    {
    }
    ExprEvalResult(int n, double *fp, const char **c)
        : n(n)
        , fp(fp)
        , str(c)
    {
    }

    int n {0};
    double *fp {nullptr};
    const char **str {nullptr};
};
} // namespace KSeExpr
