// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

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
using LLVM_VALUE = llvm::Value *;
using LLVM_BUILDER = llvm::IRBuilder<> &;
#define LLVM_BASE const
#define LLVM_BODY const override
#else
using LLVM_VALUE = double;
using LLVM_BUILDER = double;
#define LLVM_BASE const { return 0; }
#define LLVM_BODY const override { return 0; }
#endif
