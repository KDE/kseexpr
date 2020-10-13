// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#ifndef ExprParser_h
#define ExprParser_h

#ifndef MAKEDEPEND
#include <string>
#endif

#include "Expression.h"
#include "ErrorCode.h"
#include "ExprNode.h"

namespace KSeExpr {
bool ExprParse(KSeExpr::ExprNode*& parseTree,
               KSeExpr::ErrorCode& errorCode,
               std::vector<std::string>& errorIds,
               int& errorStart,
               int& errorEnd,
               std::vector<std::pair<int, int> >& _comments,
               const KSeExpr::Expression* expr,
               const char* str,
               bool wantVec = true);
}

#endif
