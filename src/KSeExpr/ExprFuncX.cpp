// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#include "ExprFunc.h"
#include "ExprFuncX.h"
#include "Interpreter.h"
#include "ExprNode.h"
#include <cstdio>

namespace KSeExpr {
int ExprFuncSimple::EvalOp(int *opData, double *fp, char **c, std::vector<int> &callStack) {
    ExprFuncSimple *simple = reinterpret_cast<ExprFuncSimple *>(c[opData[0]]);
    //    ExprFuncNode::Data* simpleData=reinterpret_cast<ExprFuncNode::Data*>(c[opData[1]]);
    ArgHandle args(opData, fp, c, callStack);
    simple->eval(args);
    return 1;
}

int ExprFuncSimple::buildInterpreter(const ExprFuncNode *node, Interpreter *interpreter) const {
    std::vector<int> operands;
    for (int c = 0; c < node->numChildren(); c++) {
        int operand = node->child(c)->buildInterpreter(interpreter);
#if 0
        // debug
        std::cerr<<"we are "<<node->promote(c)<<" "<<c<<std::endl;
#endif
        if (node->promote(c) != 0) {
            interpreter->addOp(getTemplatizedOp<Promote>(node->promote(c)));
            int promotedOperand = interpreter->allocFP(node->promote(c));
            interpreter->addOperand(operand);
            interpreter->addOperand(promotedOperand);
            operand = promotedOperand;
            interpreter->endOp();
        }
        operands.push_back(operand);
    }
    int outoperand = -1;
    int nargsData = interpreter->allocFP(1);
    interpreter->d[nargsData] = node->numChildren();
    if (node->type().isFP())
        outoperand = interpreter->allocFP(node->type().dim());
    else if (node->type().isString())
        outoperand = interpreter->allocPtr();
    else
        assert(false);

    interpreter->addOp(EvalOp);
    int ptrLoc = interpreter->allocPtr();
    int ptrDataLoc = interpreter->allocPtr();
    interpreter->s[ptrLoc] = (char *)this;
    interpreter->addOperand(ptrLoc);
    interpreter->addOperand(ptrDataLoc);
    interpreter->addOperand(outoperand);
    interpreter->addOperand(nargsData);
    for (size_t c = 0; c < operands.size(); c++) {
        interpreter->addOperand(operands[c]);
    }
    interpreter->endOp(false);  // do not eval because the function may not be evaluatable!

    // call into interpreter eval
    int pc = interpreter->nextPC() - 1;
    int *opCurr = (&interpreter->opData[0]) + interpreter->ops[pc].second;

    ArgHandle args(opCurr, &interpreter->d[0], &interpreter->s[0], interpreter->callStack);
    ExprFuncNode::Data* data = evalConstant(node, args);
    node->setData(data);
    interpreter->s[ptrDataLoc] = reinterpret_cast<char *>(data);

    return outoperand;
}
}

extern "C" {
//            allocate int[4+number of args];
//            allocate char*[2];
//            allocate double[1+ sizeof(ret) + sizeof(args)]
//
//            int[0]= c , 0
//            int[1]= c , 1
//            int[2]= f,  0
//            int[3]= f,  8
//
//            int[4]= f, 8
//            int[5]= f, 9
//
//
//                    double[0] = 0
//                    double[1] = 0
//                    double[2] = 0
//                    double[3] = 0
// opData indexes either into f or into c.
// opdata[0] points to ExprFuncSimple instance
// opdata[1] points to the data generated by evalConstant
// opdata[2] points to return value
// opdata[3] points to number of args
// opdata[4] points to beginning of arguments in
void KSeExprLLVMEvalCustomFunction(int *opDataArg,
                                   double *fpArg,
                                   char **strArg,
                                   void **funcdata,
                                   const KSeExpr::ExprFuncNode *node) {
    const KSeExpr::ExprFunc *func = node->func();
    KSeExpr::ExprFuncX *funcX = const_cast<KSeExpr::ExprFuncX *>(func->funcx());
    KSeExpr::ExprFuncSimple *funcSimple = static_cast<KSeExpr::ExprFuncSimple *>(funcX);

    strArg[0] = reinterpret_cast<char *>(funcSimple);

    std::vector<int> callStack;
    KSeExpr::ExprFuncSimple::ArgHandle handle(opDataArg, fpArg, strArg, callStack);
    if (!*funcdata) {
        handle.data = funcSimple->evalConstant(node, handle);
        *funcdata = reinterpret_cast<void *>(handle.data);
        node->setData(handle.data);
    } else {
        handle.data = reinterpret_cast<KSeExpr::ExprFuncNode::Data *>(*funcdata);
    }

    funcSimple->eval(handle);
    // for (int i = 0; i < retSize; ++i) result[i] = fp[1 + i];
}
}
