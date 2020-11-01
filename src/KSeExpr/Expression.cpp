// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stack>
#include <typeinfo>

#include "Evaluator.h"
#include "ExprConfig.h"
#include "ExprEnv.h"
#include "ExprFunc.h"
#include "ExprNode.h"
#include "ExprParser.h"
#include "ExprType.h"
#include "ExprWalker.h"
#include "Expression.h"
#include "TypePrinter.h"

namespace KSeExpr {

// Get debugging flag from environment
bool Expression::debugging = getenv("SE_EXPR_DEBUG") != nullptr;
// Choose the defeault strategy based on what we've compiled with (SEEXPR_ENABLE_LLVM)
// And the environment variables SE_EXPR_DEBUG
static Expression::EvaluationStrategy chooseDefaultEvaluationStrategy() {
    if (Expression::debugging) {
        std::cerr << "KSeExpr Debug Mode Enabled " <<
#if defined(_MSC_VER)
            _MSC_FULL_VER
#else
            __VERSION__
#endif
            << std::endl;
    }
#if defined(SEEXPR_ENABLE_LLVM)
    if (char* env = getenv("SE_EXPR_EVAL")) {
        if (Expression::debugging) std::cerr << "Overriding SeExpr Evaluation Default to be " << env << std::endl;
        return !strcmp(env, "LLVM") ? Expression::UseLLVM : !strcmp(env, "INTERPRETER") ? Expression::UseInterpreter
                                                                                        : Expression::UseInterpreter;
    } else
        return Expression::UseLLVM;
#else
    return Expression::UseInterpreter;
#endif
}
Expression::EvaluationStrategy Expression::defaultEvaluationStrategy = chooseDefaultEvaluationStrategy();

Expression::Expression(Expression::EvaluationStrategy evaluationStrategy)
    : _wantVec(true), _expression(""), _evaluationStrategy(evaluationStrategy), _context(&Context::global()),
      _desiredReturnType(ExprType().FP(3).Varying()), _parseTree(nullptr), _isValid(false), _parsed(false), _prepped(false),
      _interpreter(nullptr), _llvmEvaluator(new LLVMEvaluator()) {
    ExprFunc::init();
}

Expression::Expression(const std::string& e,
                       const ExprType& type,
                       EvaluationStrategy evaluationStrategy,
                       const Context& context)
    : _wantVec(true), _expression(e), _evaluationStrategy(evaluationStrategy), _context(&context),
      _desiredReturnType(type), _parseTree(nullptr), _isValid(false), _parsed(false), _prepped(false), _interpreter(nullptr),
      _llvmEvaluator(new LLVMEvaluator()) {
    ExprFunc::init();
}

Expression::~Expression() {
    reset();
    delete _llvmEvaluator;
}

void Expression::debugPrintInterpreter() const {
    if (_interpreter) {
        _interpreter->print();
        std::cerr << "return slot " << _returnSlot << std::endl;
    }
}

void Expression::debugPrintLLVM() const { _llvmEvaluator->debugPrint(); }

void Expression::debugPrintParseTree() const {
    if (_parseTree) {
        // print the parse tree
        std::cerr << "Parse tree desired type " << _desiredReturnType.toString() << " actual "
                  << _parseTree->type().toString() << std::endl;
        TypePrintExaminer _examiner;
        KSeExpr::ConstWalker _walker(&_examiner);
        _walker.walk(_parseTree);
    }
}

void Expression::reset() {
    delete _llvmEvaluator;
    _llvmEvaluator = new LLVMEvaluator();
    delete _parseTree;
    _parseTree = nullptr;
    if (_evaluationStrategy == UseInterpreter) {
        delete _interpreter;
        _interpreter = nullptr;
    }
    _isValid = 0;
    _parsed = 0;
    _prepped = 0;
    _parseErrorCode = ErrorCode::None;
    _parseErrorIds.clear();
    _vars.clear();
    _funcs.clear();
    //_localVars.clear();
    _errors.clear();
    _envBuilder.reset();
    _threadUnsafeFunctionCalls.clear();
    _comments.clear();
}

void Expression::setContext(const Context& context) {
    reset();
    _context = &context;
}

void Expression::setDesiredReturnType(const ExprType& type) {
    reset();
    _desiredReturnType = type;
}

void Expression::setVarBlockCreator(const VarBlockCreator* creator) {
    reset();
    _varBlockCreator = creator;
}

void Expression::setExpr(const std::string& e) {
    if (_expression != "") reset();
    _expression = e;
}

bool Expression::syntaxOK() const {
    parseIfNeeded();
    return _isValid;
}

bool Expression::isConstant() const {
    parseIfNeeded();
    return returnType().isLifetimeConstant();
}

bool Expression::usesVar(const std::string& name) const {
    parseIfNeeded();
    return _vars.find(name) != _vars.end();
}

bool Expression::usesFunc(const std::string& name) const {
    parseIfNeeded();
    return _funcs.find(name) != _funcs.end();
}

void Expression::parse() const {
    if (_parsed) return;
    _parsed = true;
    int tempStartPos, tempEndPos;
    ExprParse(_parseTree, _parseErrorCode, _parseErrorIds, tempStartPos, tempEndPos, _comments, this, _expression.c_str(), _wantVec);
    if (!_parseTree) {
        addError(_parseErrorCode, _parseErrorIds, tempStartPos, tempEndPos);
    }
}

void Expression::prep() const {
    if (_prepped) return;
#ifdef SEEXPR_PERFORMANCE
    PerformanceTimer timer("[ PREP     ] v2 prep time: ");
#endif
    _prepped = true;
    parseIfNeeded();

    bool error = false;
    std::string _parseError;

    if (!_parseTree) {
        // parse error
        error = true;
    } else if (!_parseTree->prep(_desiredReturnType.isFP(1), _envBuilder).isValid()) {
        // prep error
        error = true;
    } else if (!ExprType::valuesCompatible(_parseTree->type(), _desiredReturnType)) {
        // incompatible type error
        error = true;
        _parseTree->addError(ErrorCode::ExpressionIncompatibleTypes, { _parseTree->type().toString(), _desiredReturnType.toString() });
    } else {
        _isValid = true;

        if (_evaluationStrategy == UseInterpreter) {
            if (debugging) {
                debugPrintParseTree();
                std::cerr << "Eval strategy is interpreter" << std::endl;
            }
            assert(!_interpreter);
            _interpreter = new Interpreter;
            _returnSlot = _parseTree->buildInterpreter(_interpreter);
            if (_desiredReturnType.isFP()) {
                int dimWanted = _desiredReturnType.dim();
                int dimHave = _parseTree->type().dim();
                if (dimWanted > dimHave) {
                    _interpreter->addOp(getTemplatizedOp<Promote>(dimWanted));
                    int finalOp = _interpreter->allocFP(dimWanted);
                    _interpreter->addOperand(_returnSlot);
                    _interpreter->addOperand(finalOp);
                    _returnSlot = finalOp;
                    _interpreter->endOp();
                }
            }
            if (debugging) _interpreter->print();
        } else {  // useLLVM
            if (debugging) {
                std::cerr << "Eval strategy is llvm" << std::endl;
                debugPrintParseTree();
            }
            if (!_llvmEvaluator->prepLLVM(_parseTree, _desiredReturnType)) {
                error = true;
            }
        }

        // TODO: need promote
        _returnType = _parseTree->type();
    }

    if (error) {
        _isValid = false;
        _returnType = ExprType().Error();

        // build line lookup table
        std::vector<int> lines;
        const char* start = _expression.c_str();
        const char* p = _expression.c_str();
        while (*p != 0) {
            if (*p == '\n') lines.push_back(static_cast<int>(p - start));
            p++;
        }
        lines.push_back(static_cast<int>(p - start));

        std::stringstream sstream;
        for (unsigned int i = 0; i < _errors.size(); i++) {
            int* bound = std::lower_bound(&*lines.begin(), &*lines.end(), _errors[i].startPos);
            int line = static_cast<int>(bound - &*lines.begin() + 1);
            int lineStart = line == 1 ? 0 : lines[line - 1];
            int col = _errors[i].startPos - lineStart;
            sstream << "  Line " << line << " Col " << col << " - " << _errors[i].error << std::endl;
        }
        _parseError = std::string(sstream.str());
    }

    if (debugging) {
        std::cerr << "ending with isValid " << _isValid << std::endl;
        std::cerr << "parse error \n" << _parseError << std::endl;
    }
}

bool Expression::isVec() const {
    prepIfNeeded();
    return _isValid ? _parseTree->isVec() : _wantVec;
}

const ExprType& Expression::returnType() const {
    prepIfNeeded();
    return _returnType;
}

const double* Expression::evalFP(VarBlock* varBlock) const {
    prepIfNeeded();
    if (_isValid) {
        if (_evaluationStrategy == UseInterpreter) {
            _interpreter->eval(varBlock);
            return (varBlock && varBlock->threadSafe) ? &(varBlock->d[_returnSlot]) : &_interpreter->d[_returnSlot];
        } else {  // useLLVM
            return _llvmEvaluator->evalFP(varBlock);
        }
    }
    static double noCrash[16] = {};
    return noCrash;
}

void Expression::evalMultiple(VarBlock* varBlock, int outputVarBlockOffset, size_t rangeStart, size_t rangeEnd) const {
    prepIfNeeded();
    if (_isValid) {
        if (_evaluationStrategy == UseInterpreter) {
            // TODO: need strings to work
            int dim = _desiredReturnType.dim();
            // double* iHack=reinterpret_cast<double**>(varBlock->data())[outputVarBlockOffset];
            double* destBase = reinterpret_cast<double**>(varBlock->data())[outputVarBlockOffset];
            for (size_t i = rangeStart; i < rangeEnd; i++) {
                varBlock->indirectIndex = static_cast<int>(i);
                const double* f = evalFP(varBlock);
                for (int k = 0; k < dim; k++) {
                    destBase[dim * i + k] = f[k];
                }
            }
        } else {  // useLLVM
            _llvmEvaluator->evalMultiple(varBlock, outputVarBlockOffset, rangeStart, rangeEnd);
        }
    }
}

const char* Expression::evalStr(VarBlock* varBlock) const {
    prepIfNeeded();
    if (_isValid) {
        if (_evaluationStrategy == UseInterpreter) {
            _interpreter->eval(varBlock);
            return (varBlock && varBlock->threadSafe) ? varBlock->s[_returnSlot] : _interpreter->s[_returnSlot];
        } else {  // useLLVM
            return _llvmEvaluator->evalStr(varBlock);
        }
    }
    return nullptr;
}

}  // end namespace KSeExpr/
