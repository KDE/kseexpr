// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <cstdio>
#include <typeinfo>

#include <KSeExpr/ExprNode.h>
#include <KSeExpr/ExprWalker.h>
#include <KSeExpr/Expression.h>

namespace KSeExpr
{
class TypePrintExaminer : public KSeExpr::Examiner<true> {
  public:
    bool examine(const ExprNode* examinee) override;
    void reset() override {};
    void post(const ExprNode *) override {};
};

bool TypePrintExaminer::examine(const ExprNode* examinee) {
    const ExprNode* curr = examinee;
    int depth = 0;
    std::array<char, 1024> buf{};
    while (curr != nullptr) {
        depth++;
        curr = curr->parent();
    }
    sprintf(buf.data(), "%*s", depth * 2, " ");
    std::cout << buf.data() << "'" << examinee->toString() << "' " << typeid(*examinee).name()
              << " type=" << examinee->type().toString() << std::endl;

    return true;
}
} // namespace KSeExpr
