// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _TypePrinter_h_
#define _TypePrinter_h_

#include <ExprNode.h>
#include <ExprWalker.h>
#include <Expression.h>
#include <typeinfo>

using namespace KSeExpr;

/**
   @file TypePrinter.cpp
*/

class TypePrintExaminer : public KSeExpr::Examiner<true> {
  public:
    virtual bool examine(const ExprNode* examinee);
    virtual void reset() {};
};

bool TypePrintExaminer::examine(const ExprNode* examinee) {
    const ExprNode* curr = examinee;
    int depth = 0;
    char buf[1024];
    while (curr != 0) {
        depth++;
        curr = curr->parent();
    }
    sprintf(buf, "%*s", depth * 2, " ");
    std::cout << buf << "'" << examinee->toString() << "' " << typeid(*examinee).name()
              << " type=" << examinee->type().toString() << std::endl;

    return true;
};

#endif
