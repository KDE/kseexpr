// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: LicenseRef-Apache-2.0

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
