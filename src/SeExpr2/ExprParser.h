/*
 Copyright Disney Enterprises, Inc.  All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License
 and the following modification to it: Section 6 Trademarks.
 deleted and replaced with:

 6. Trademarks. This License does not grant permission to use the
 trade names, trademarks, service marks, or product names of the
 Licensor and its affiliates, except as required for reproducing
 the content of the NOTICE file.

 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
*/
#ifndef ExprParser_h
#define ExprParser_h

#ifndef MAKEDEPEND
#include <string>
#endif

#include "Expression.h"
#include "ErrorCode.h"
#include "ExprNode.h"

namespace SeExpr2 {
bool ExprParse(SeExpr2::ExprNode*& parseTree,
               SeExpr2::ErrorCode& errorCode,
               std::vector<std::string>& errorIds,
               int& errorStart,
               int& errorEnd,
               std::vector<std::pair<int, int> >& _comments,
               const SeExpr2::Expression* expr,
               const char* str,
               bool wantVec = true);
}

#endif
