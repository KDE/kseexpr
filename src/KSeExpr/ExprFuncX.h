// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ExprType.h"
#include "Vec.h"
#include "ExprNode.h"

namespace KSeExpr {
class ExprFuncNode;
class Interpreter;
class ExprVarEnv;
using Statistics = std::map<std::string, double>;

//! Extension function spec, used for complicated argument custom functions.
/** Provides the ability to handle all argument type checking and processing manually.
    Derive from this class and then make your own ExprFunc that takes this object.
    This is necessary if you need string arguments or you have variable numbers of
    arguments.  See ExprBuiltins.h for some examples */
class ExprFuncX {
  public:
    //! Create an ExprFuncX. If the functions and type checking you implement
    //! is thread safe your derived class should call this with true. If not,
    //! then false.  If you mark a function as thread unsafe,  and it is used
    //! in an expression then bool Expression::isThreadSafe() will return false
    //! and the controlling software should not attempt to run multiple threads
    //! of an expression.
    ExprFuncX(const bool threadSafe) : _threadSafe(threadSafe) {}
    ExprFuncX(const ExprFuncX &) = default;
    ExprFuncX(ExprFuncX &&) = default;
    ExprFuncX& operator=(const ExprFuncX &) = default;
    ExprFuncX& operator=(ExprFuncX &&) = default;

    /** prep the expression by doing all type checking argument checking, etc. */
    virtual ExprType prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnvBuilder& env) const = 0;
    virtual ExprType type() const { return _type; }

    /** evaluate the expression. the given node is where in the parse tree
        the evaluation is for */
    // virtual void eval(const ExprFuncNode* node, Vec3d& result) const = 0;
    //! Build an interpreter to evaluate the expression
    virtual int buildInterpreter(const ExprFuncNode* node, Interpreter* interpreter) const = 0;
    virtual ~ExprFuncX() = default;

    bool isThreadSafe() const { return _threadSafe; }

    /// Return memory usage of a funcX in bytes.
    virtual size_t sizeInBytes() const { return 0; }

    /// Give this function a chance to populate its statistics
    virtual void statistics(Statistics& /*statistics*/) const {}

  protected:
    bool _isScalar{};
    ExprType _type;

  private:
    bool _threadSafe;
};

class ExprFuncSimple : public ExprFuncX {
  public:
    ExprFuncSimple(const bool threadSafe) : ExprFuncX(threadSafe) {}

    class ArgHandle {
      public:
        ArgHandle(int* opData, double* fp, char** c, std::vector<int>&)
            : outFp(fp[opData[2]]), outStr(c[opData[2]]), data(reinterpret_cast<ExprFuncNode::Data*>(c[opData[1]])),
              // TODO: put the value in opData rather than fp
              _nargs((int)fp[opData[3]]),  // TODO: would be good not to have to convert to int!
              opData(opData + 4), fp(fp), c(c) {}

        template <int d>
        Vec<double, d, true> inFp(int i) {
            return Vec<double, d, true>(&fp[opData[i]]);
        }
        char* inStr(int i) { return c[opData[i]]; }
        int nargs() const { return _nargs; }

        /// Return a vector handle which is easier to assign to
        template <int d>
        Vec<double, d, true> outFpHandle() {
            return Vec<double, d, true>(&outFp);
        }

        double& outFp;
        char*& outStr;
        ExprFuncNode::Data* data;

      private:
        int _nargs;
        int* opData;
        double* fp;
        char** c;
        // std::stack<int>& callStack;
    };

    int buildInterpreter(const ExprFuncNode* node, Interpreter* interpreter) const override;

    ExprType prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnvBuilder& envBuilder) const override = 0;
    virtual ExprFuncNode::Data* evalConstant(const ExprFuncNode* node, ArgHandle args) const = 0;
    virtual void eval(ArgHandle args) = 0;

  private:
    static int EvalOp(int* opData, double* fp, char** c, std::vector<int>& callStack);
};

class ExprFuncLocal : public ExprFuncX {
    ExprFuncLocal() : ExprFuncX(true) {}

    /** prep the expression by doing all type checking argument checking, etc. */
    ExprType prep(ExprFuncNode* node, bool scalarWanted, ExprVarEnvBuilder& envBuilder) const override;
    //! Build an interpreter to evaluate the expression
    int buildInterpreter(const ExprFuncNode* node, Interpreter* interpreter) const override;
};
} // namespace KSeExpr
