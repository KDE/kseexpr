#include "ErrorMessages.h"

using namespace SeExpr2;

QString ErrorMessages::message(SeExpr2::ErrorCode code)
{
    switch (code)
    {
        case ErrorCode::None:
            return QString();
        case ErrorCode::ExpectedStringOrFloatAnyD:
            return tr("Expected string or float[d]");
        case ErrorCode::ExpectedFloatAnyD:
            return tr("Expected float[d]");
        case ErrorCode::ExpectedFloatD:
            return tr("Expected float[%1]");
        case ErrorCode::TypeMismatch12:
            return tr("Type mismatch, first: '%1'; second: '%2'");
        case ErrorCode::ExpectedFloatOrFloat3:
            return tr("Expected float or float[3]");
        case ErrorCode::ArgumentTypeMismatch:
            return tr("Expected '%1' for argument, got '%2'");
        case ErrorCode::WrongNumberOfArguments:
            return tr("Wrong number of arguments, should be 1 to 7");
        case ErrorCode::WrongNumberOfArgumentsMultiple3Plus1:
            return tr("Wrong number of arguments, should be multiple of 3 plus 1");
        case ErrorCode::WrongNumberOfArguments1Plus:
            return tr("Wrong number of arguments, should be 1 or more");
        case ErrorCode::FirstArgumentNotString:
            return tr("First argument must be a string");
        case ErrorCode::IncompleteFormatSpecifier:
            return tr("Incomplete format specifier");
        case ErrorCode::UndeclaredVariable:
            return tr("No variable named '%1'");
        case ErrorCode::UndeclaredFunction:
            return tr("Function '%1' has no definition");
        case ErrorCode::BadAssignmentOperator:
            return tr("Assignment operation has incorrect type '%1'");
        case ErrorCode::ConditionalTypesNotCompatible:
            return tr("Types of conditional are not compatible");
        case ErrorCode::InconsistentDefinition:
            return tr("Variable '%1' defined in conditionals inconsistently");
        case ErrorCode::FunctionTooFewArguments:
            return tr("Too few arguments for function '%1'");
        case ErrorCode::FunctionTooManyArguments:
            return tr("Too many arguments for function '%1'");
        case ErrorCode::ExpressionIncompatibleTypes:
            return tr("Expression generated type '%1', incompatible with desired type '%2'");
        case ErrorCode::SyntaxError:
            return tr("Syntax error near '%1'");
        case ErrorCode::UnexpectedEndOfExpression:
            return tr("Unexpected end of expression near '%1'");
        case ErrorCode::UnexpectedEndOfFormatString:
            return tr("Unexpected end of format string");
        case ErrorCode::InvalidFormatString:
            return tr("Invalid format string, only %v or %f is allowed");
        case ErrorCode::WrongNumberOfArgumentsForFormatString:
            return tr("Wrong number of arguments for format string");
        case ErrorCode::Unknown:
        default:
            return tr("Unknown error (message = %1)");
    }
}
