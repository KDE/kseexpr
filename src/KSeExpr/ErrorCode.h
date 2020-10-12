/*
 * Copyright (C) 2020 L. E. Segovia <amy@amyspark.me>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License
 * and the following modification to it: Section 6 Trademarks.
 * deleted and replaced with:
 *
 * 6. Trademarks. This License does not grant permission to use the
 * trade names, trademarks, service marks, or product names of the
 * Licensor and its affiliates, except as required for reproducing
 * the content of the NOTICE file.
 *
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef ErrorCode_h
#define ErrorCode_h

namespace KSeExpr {
enum ErrorCode {
    //! OK
    None = 0,

    //! "Expected String or Float[d]"
    ExpectedStringOrFloatAnyD,
    //! "Expected Float[d]"
    ExpectedFloatAnyD,
    //! "Expected Float[" << d << "]"
    ExpectedFloatD,
    //! "Type mismatch. First: " << first << " Second: " << second
    TypeMismatch12,
    //! "Expected float or FP[3]"
    ExpectedFloatOrFloat3,
    //! "Expected "<< type << " for argument, got " << childType
    ArgumentTypeMismatch,
    //! "Wrong number of arguments, should be 1 to 7"
    WrongNumberOfArguments,
    //! "Wrong number of arguments, should be multiple of 3 plus 1"
    WrongNumberOfArgumentsMultiple3Plus1,
    //! "Wrong number of arguments, should be >= 1"
    WrongNumberOfArguments1Plus,
    //! "First argument must be a string."
    FirstArgumentNotString,
    //! "incomplete format specifier"
    IncompleteFormatSpecifier,
    //! name + " fail resolveVar"
    //! "No variable named '" << name() << "'"
    UndeclaredVariable,
    //! "Function " << _name << " has no definition"
    UndeclaredFunction,
    //! "Assignment operation has bad type: " << _type
    BadAssignmentOperator,
    //! "Types of conditional are not compatible",
    ConditionalTypesNotCompatible,
    //! "Variable " << name() << " defined in conditionals inconsistently."
    InconsistentDefinition,
    //! "Too few args for function " << _name
    FunctionTooFewArguments,
    //! "Too many args for function " << _name
    FunctionTooManyArguments,

    //! "Expression generated type " << _parseTree->type() << " incompatible with desired type " << _desiredReturnType
    ExpressionIncompatibleTypes,

    //! Syntax error
    SyntaxError,
    //! Unexpected end of expression
    UnexpectedEndOfExpression,
    //! Unexpected end of format string
    UnexpectedEndOfFormatString,
    //! Invalid format string, only %v or %f is allowed
    InvalidFormatString,
    //! Wrong number of arguments for format string
    WrongNumberOfArgumentsForFormatString,

    //! Unknown error (message = %1)
    Unknown,
};
}

#endif
