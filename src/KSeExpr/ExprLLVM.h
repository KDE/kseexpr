// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
#pragma once

#include "ExprConfig.h"

#if defined(SEEXPR_ENABLE_LLVM)
#ifndef Q_MOC_RUN /* automoc chokes in Qt classes using llvm/ir/intrinsics.h indirectly*/
#include <llvm/IR/IRBuilder.h>
#endif
namespace llvm {
class Value;
class Type;
class Module;
class Function;
}
typedef llvm::Value* LLVM_VALUE;
typedef llvm::IRBuilder<>& LLVM_BUILDER;
#define LLVM_BODY const
#else
typedef double LLVM_VALUE;
typedef double LLVM_BUILDER;
#define LLVM_BODY \
    { return 0; }
#endif
