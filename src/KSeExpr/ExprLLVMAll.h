// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ExprConfig.h"
#include "ExprLLVM.h"

#if defined(SEEXPR_ENABLE_LLVM)
#ifndef Q_MOC_RUN /* automoc chokes in Qt classes using llvm/ir/intrinsics.h indirectly*/
#include <llvm/ADT/ArrayRef.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/InitializePasses.h>
#include <llvm/LinkAllPasses.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <memory>

#if LLVM_VERSION_MAJOR >= 13
#define IN_BOUNDS_GEP(Builder, firstArg, idx) Builder.CreateInBoundsGEP(firstArg->getType()->getScalarType()->getPointerElementType(), firstArg, idx)
#define CREATE_LOAD(Builder, indexVar) Builder.CreateLoad(indexVar->getType()->getPointerElementType(), indexVar)
#define CREATE_LOAD_WITH_ID(Builder, indexVar, id) Builder.CreateLoad(indexVar->getType()->getPointerElementType(), indexVar, id)
#define CREATE_CONST_GEP1_32(Builder, a, b) Builder.CreateConstGEP1_32(a->getType()->getPointerElementType(), a, b)
#else
#define IN_BOUNDS_GEP(Builder, firstArg, idx) Builder.CreateInBoundsGEP(firstArg, idx)
#define CREATE_LOAD(Builder, indexVar) Builder.CreateLoad(indexVar)
#define CREATE_LOAD_WITH_ID(Builder, indexVar, id) Builder.CreateLoad(indexVar, id)
#define CREATE_CONST_GEP1_32(Builder, a, b) Builder.CreateConstGEP1_32(a, b)
#endif // LLVM_VERSION_MAJOR >= 13
#endif // Q_MOC_RUN
#endif
