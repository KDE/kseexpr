/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with SeExprYY_ or SeExpr_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with SeExpr or SeExprYY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define SeExprYYBISON 1

/* Bison version.  */
#define SeExprYYBISON_VERSION "3.6.4"

/* Skeleton name.  */
#define SeExprYYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define SeExprYYPURE 0

/* Push parsers.  */
#define SeExprYYPUSH 0

/* Pull parsers.  */
#define SeExprYYPULL 1


/* Substitute the variable and function names.  */
#define SeExprparse         SeExprparse
#define SeExprlex           SeExprlex
#define SeExprerror         SeExprerror
#define SeExprdebug         SeExprdebug
#define SeExprnerrs         SeExprnerrs
#define SeExprlval          SeExprlval
#define SeExprchar          SeExprchar
#define SeExprlloc          SeExprlloc

/* First part of user prologue.  */
#line 6 "@@PATH@@/src/KSeExpr/ExprParser.y"

#ifndef MAKEDEPEND
#include <algorithm>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#endif
#include "ExprType.h"
#include "ExprNode.h"
#include "ExprParser.h"
#include "Expression.h"
#include <mutex>

/******************
 lexer declarations
 ******************/

// declarations of functions and data in ExprParser.l
int SeExprlex();
int SeExprpos();
extern int SeExpr_start;
extern char* SeExprtext;
struct SeExpr_buffer_state;
SeExpr_buffer_state* SeExpr_scan_string(const char *str);
void SeExpr_delete_buffer(SeExpr_buffer_state*);

/*******************
 parser declarations
 *******************/

// forward declaration
static void SeExprerror(const char* msg);

// local data
static const char* ParseStr;    // string being parsed
static KSeExpr::ErrorCode ParseErrorCode;  // error (set from SeExprerror)
static std::string ParseErrorId; // string that failed parsing (set from SeExprerror)
static KSeExpr::ExprNode* ParseResult; // must set result here since SeExprparse can't return it
static const KSeExpr::Expression* Expr;// used for parenting created SeExprOp's

/* The list of nodes being built is remembered locally here.
   Eventually (if there are no syntax errors) ownership of the nodes
   will belong solely to the parse tree and the parent expression.
   However, if there is a syntax error, we must loop through this list
   and free any nodes that were allocated before the error to avoid a
   memory leak. */
static std::vector<KSeExpr::ExprNode*> ParseNodes;
inline KSeExpr::ExprNode* Remember(KSeExpr::ExprNode* n,const int startPos,const int endPos)
    { ParseNodes.push_back(n); n->setPosition(startPos,endPos); return n; }
inline void Forget(KSeExpr::ExprNode* n)
    { ParseNodes.erase(std::find(ParseNodes.begin(), ParseNodes.end(), n)); }
/* These are handy node constructors for 0-3 arguments */
#define NODE(startPos,endPos,name) Remember(new KSeExpr::Expr##name(Expr),startPos,endPos)
#define NODE1(startPos,endPos,name,a) Remember(new KSeExpr::Expr##name(Expr,a),startPos,endPos)
#define NODE2(startPos,endPos,name,a,b) Remember(new KSeExpr::Expr##name(Expr,a,b),startPos,endPos)
#define NODE3(startPos,endPos,name,a,b,c) Remember(new KSeExpr::Expr##name(Expr,a,b,c),startPos,endPos)
#define NODE4(startPos,endPos,name,a,b,c,t) Remember(new KSeExpr::Expr##name(Expr,a,b,c,t),startPos,endPos)

#line 139 "y.tab.c"

# ifndef SeExprYY_CAST
#  ifdef __cplusplus
#   define SeExprYY_CAST(Type, Val) static_cast<Type> (Val)
#   define SeExprYY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define SeExprYY_CAST(Type, Val) ((Type) (Val))
#   define SeExprYY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef SeExprYY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define SeExprYY_NULLPTR nullptr
#   else
#    define SeExprYY_NULLPTR 0
#   endif
#  else
#   define SeExprYY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef SeExprYY_SEEXPR_Y_TAB_H_INCLUDED
# define SeExprYY_SEEXPR_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef SeExprYYDEBUG
# define SeExprYYDEBUG 0
#endif
#if SeExprYYDEBUG
extern int SeExprdebug;
#endif

/* Token kinds.  */
#ifndef SeExprYYTOKENTYPE
# define SeExprYYTOKENTYPE
  enum SeExprtokentype
  {
    SeExprYYEMPTY = -2,
    SeExprYYEOF = 0,                     /* "end of file"  */
    SeExprYYerror = 256,                 /* error  */
    SeExprYYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    EXTERN = 260,                  /* EXTERN  */
    DEF = 261,                     /* DEF  */
    FLOATPOINT = 262,              /* FLOATPOINT  */
    STRING = 263,                  /* STRING  */
    NAME = 264,                    /* NAME  */
    VAR = 265,                     /* VAR  */
    STR = 266,                     /* STR  */
    NUMBER = 267,                  /* NUMBER  */
    LIFETIME_CONSTANT = 268,       /* LIFETIME_CONSTANT  */
    LIFETIME_UNIFORM = 269,        /* LIFETIME_UNIFORM  */
    LIFETIME_VARYING = 270,        /* LIFETIME_VARYING  */
    LIFETIME_ERROR = 271,          /* LIFETIME_ERROR  */
    AddEq = 272,                   /* AddEq  */
    SubEq = 273,                   /* SubEq  */
    MultEq = 274,                  /* MultEq  */
    DivEq = 275,                   /* DivEq  */
    ExpEq = 276,                   /* ExpEq  */
    ModEq = 277,                   /* ModEq  */
    ARROW = 278,                   /* ARROW  */
    OR = 279,                      /* OR  */
    AND = 280,                     /* AND  */
    EQ = 281,                      /* EQ  */
    NE = 282,                      /* NE  */
    SEEXPR_LE = 283,               /* SEEXPR_LE  */
    SEEXPR_GE = 284,               /* SEEXPR_GE  */
    UNARY = 285                    /* UNARY  */
  };
  typedef enum SeExprtokentype SeExprtoken_kind_t;
#endif

/* Value type.  */
#if ! defined SeExprYYSTYPE && ! defined SeExprYYSTYPE_IS_DECLARED
union SeExprYYSTYPE
{
#line 66 "@@PATH@@/src/KSeExpr/ExprParser.y"

    KSeExpr::ExprNode* n; /* a node is returned for all non-terminals to
		      build the parse tree from the leaves up. */
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: the string
		      is allocated with strdup() in the lexer and must
		      be freed with free() */
    struct {
        KSeExpr::ExprType::Type     type;
        int                  dim;
        KSeExpr::ExprType::Lifetime lifetime;
    } t;  // return value for types
    KSeExpr::ExprType::Lifetime l; // return value for lifetime qualifiers

#line 234 "y.tab.c"

};
typedef union SeExprYYSTYPE SeExprYYSTYPE;
# define SeExprYYSTYPE_IS_TRIVIAL 1
# define SeExprYYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined SeExprYYLTYPE && ! defined SeExprYYLTYPE_IS_DECLARED
typedef struct SeExprYYLTYPE SeExprYYLTYPE;
struct SeExprYYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define SeExprYYLTYPE_IS_DECLARED 1
# define SeExprYYLTYPE_IS_TRIVIAL 1
#endif


extern SeExprYYSTYPE SeExprlval;
extern SeExprYYLTYPE SeExprlloc;
int SeExprparse (void);

#endif /* !SeExprYY_SEEXPR_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum SeExprsymbol_kind_t
{
  SeExprYYSYMBOL_SeExprYYEMPTY = -2,
  SeExprYYSYMBOL_SeExprYYEOF = 0,                      /* "end of file"  */
  SeExprYYSYMBOL_SeExprYYerror = 1,                    /* error  */
  SeExprYYSYMBOL_SeExprYYUNDEF = 2,                    /* "invalid token"  */
  SeExprYYSYMBOL_IF = 3,                         /* IF  */
  SeExprYYSYMBOL_ELSE = 4,                       /* ELSE  */
  SeExprYYSYMBOL_EXTERN = 5,                     /* EXTERN  */
  SeExprYYSYMBOL_DEF = 6,                        /* DEF  */
  SeExprYYSYMBOL_FLOATPOINT = 7,                 /* FLOATPOINT  */
  SeExprYYSYMBOL_STRING = 8,                     /* STRING  */
  SeExprYYSYMBOL_NAME = 9,                       /* NAME  */
  SeExprYYSYMBOL_VAR = 10,                       /* VAR  */
  SeExprYYSYMBOL_STR = 11,                       /* STR  */
  SeExprYYSYMBOL_NUMBER = 12,                    /* NUMBER  */
  SeExprYYSYMBOL_LIFETIME_CONSTANT = 13,         /* LIFETIME_CONSTANT  */
  SeExprYYSYMBOL_LIFETIME_UNIFORM = 14,          /* LIFETIME_UNIFORM  */
  SeExprYYSYMBOL_LIFETIME_VARYING = 15,          /* LIFETIME_VARYING  */
  SeExprYYSYMBOL_LIFETIME_ERROR = 16,            /* LIFETIME_ERROR  */
  SeExprYYSYMBOL_AddEq = 17,                     /* AddEq  */
  SeExprYYSYMBOL_SubEq = 18,                     /* SubEq  */
  SeExprYYSYMBOL_MultEq = 19,                    /* MultEq  */
  SeExprYYSYMBOL_DivEq = 20,                     /* DivEq  */
  SeExprYYSYMBOL_ExpEq = 21,                     /* ExpEq  */
  SeExprYYSYMBOL_ModEq = 22,                     /* ModEq  */
  SeExprYYSYMBOL_23_ = 23,                       /* '('  */
  SeExprYYSYMBOL_24_ = 24,                       /* ')'  */
  SeExprYYSYMBOL_ARROW = 25,                     /* ARROW  */
  SeExprYYSYMBOL_26_ = 26,                       /* ':'  */
  SeExprYYSYMBOL_27_ = 27,                       /* '?'  */
  SeExprYYSYMBOL_OR = 28,                        /* OR  */
  SeExprYYSYMBOL_AND = 29,                       /* AND  */
  SeExprYYSYMBOL_EQ = 30,                        /* EQ  */
  SeExprYYSYMBOL_NE = 31,                        /* NE  */
  SeExprYYSYMBOL_32_ = 32,                       /* '<'  */
  SeExprYYSYMBOL_33_ = 33,                       /* '>'  */
  SeExprYYSYMBOL_SEEXPR_LE = 34,                 /* SEEXPR_LE  */
  SeExprYYSYMBOL_SEEXPR_GE = 35,                 /* SEEXPR_GE  */
  SeExprYYSYMBOL_36_ = 36,                       /* '+'  */
  SeExprYYSYMBOL_37_ = 37,                       /* '-'  */
  SeExprYYSYMBOL_38_ = 38,                       /* '*'  */
  SeExprYYSYMBOL_39_ = 39,                       /* '/'  */
  SeExprYYSYMBOL_40_ = 40,                       /* '%'  */
  SeExprYYSYMBOL_UNARY = 41,                     /* UNARY  */
  SeExprYYSYMBOL_42_ = 42,                       /* '!'  */
  SeExprYYSYMBOL_43_ = 43,                       /* '~'  */
  SeExprYYSYMBOL_44_ = 44,                       /* '^'  */
  SeExprYYSYMBOL_45_ = 45,                       /* '['  */
  SeExprYYSYMBOL_46_ = 46,                       /* '{'  */
  SeExprYYSYMBOL_47_ = 47,                       /* '}'  */
  SeExprYYSYMBOL_48_ = 48,                       /* ']'  */
  SeExprYYSYMBOL_49_ = 49,                       /* ','  */
  SeExprYYSYMBOL_50_ = 50,                       /* '='  */
  SeExprYYSYMBOL_51_ = 51,                       /* ';'  */
  SeExprYYSYMBOL_SeExprYYACCEPT = 52,                  /* $accept  */
  SeExprYYSYMBOL_module = 53,                    /* module  */
  SeExprYYSYMBOL_declarationList = 54,           /* declarationList  */
  SeExprYYSYMBOL_declaration = 55,               /* declaration  */
  SeExprYYSYMBOL_lifetimeOptional = 56,          /* lifetimeOptional  */
  SeExprYYSYMBOL_typeDeclare = 57,               /* typeDeclare  */
  SeExprYYSYMBOL_typeListOptional = 58,          /* typeListOptional  */
  SeExprYYSYMBOL_typeList = 59,                  /* typeList  */
  SeExprYYSYMBOL_formalTypeListOptional = 60,    /* formalTypeListOptional  */
  SeExprYYSYMBOL_formalTypeList = 61,            /* formalTypeList  */
  SeExprYYSYMBOL_block = 62,                     /* block  */
  SeExprYYSYMBOL_optassigns = 63,                /* optassigns  */
  SeExprYYSYMBOL_assigns = 64,                   /* assigns  */
  SeExprYYSYMBOL_assign = 65,                    /* assign  */
  SeExprYYSYMBOL_ifthenelse = 66,                /* ifthenelse  */
  SeExprYYSYMBOL_optelse = 67,                   /* optelse  */
  SeExprYYSYMBOL_e = 68,                         /* e  */
  SeExprYYSYMBOL_exprlist = 69,                  /* exprlist  */
  SeExprYYSYMBOL_optargs = 70,                   /* optargs  */
  SeExprYYSYMBOL_args = 71,                      /* args  */
  SeExprYYSYMBOL_arg = 72                        /* arg  */
};
typedef enum SeExprsymbol_kind_t SeExprsymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define SeExprYY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ SeExprtype_int8;
#elif defined SeExprYY_STDINT_H
typedef int_least8_t SeExprtype_int8;
#else
typedef signed char SeExprtype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ SeExprtype_int16;
#elif defined SeExprYY_STDINT_H
typedef int_least16_t SeExprtype_int16;
#else
typedef short SeExprtype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ SeExprtype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined SeExprYY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t SeExprtype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char SeExprtype_uint8;
#else
typedef short SeExprtype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ SeExprtype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined SeExprYY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t SeExprtype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short SeExprtype_uint16;
#else
typedef int SeExprtype_uint16;
#endif

#ifndef SeExprYYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define SeExprYYPTRDIFF_T __PTRDIFF_TYPE__
#  define SeExprYYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define SeExprYYPTRDIFF_T ptrdiff_t
#  define SeExprYYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define SeExprYYPTRDIFF_T long
#  define SeExprYYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef SeExprYYSIZE_T
# ifdef __SIZE_TYPE__
#  define SeExprYYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define SeExprYYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define SeExprYYSIZE_T size_t
# else
#  define SeExprYYSIZE_T unsigned
# endif
#endif

#define SeExprYYSIZE_MAXIMUM                                  \
  SeExprYY_CAST (SeExprYYPTRDIFF_T,                                 \
           (SeExprYYPTRDIFF_MAXIMUM < SeExprYY_CAST (SeExprYYSIZE_T, -1)  \
            ? SeExprYYPTRDIFF_MAXIMUM                         \
            : SeExprYY_CAST (SeExprYYSIZE_T, -1)))

#define SeExprYYSIZEOF(X) SeExprYY_CAST (SeExprYYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef SeExprtype_uint8 SeExpr_state_t;

/* State numbers in computations.  */
typedef int SeExpr_state_fast_t;

#ifndef SeExprYY_
# if defined SeExprYYENABLE_NLS && SeExprYYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define SeExprYY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef SeExprYY_
#  define SeExprYY_(Msgid) Msgid
# endif
#endif


#ifndef SeExprYY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define SeExprYY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define SeExprYY_ATTRIBUTE_PURE
# endif
#endif

#ifndef SeExprYY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define SeExprYY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define SeExprYY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define SeExprYYUSE(E) ((void) (E))
#else
# define SeExprYYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about SeExprlval being uninitialized.  */
# define SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define SeExprYY_INITIAL_VALUE(Value) Value
#endif
#ifndef SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef SeExprYY_INITIAL_VALUE
# define SeExprYY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define SeExprYY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define SeExprYY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef SeExprYY_IGNORE_USELESS_CAST_BEGIN
# define SeExprYY_IGNORE_USELESS_CAST_BEGIN
# define SeExprYY_IGNORE_USELESS_CAST_END
#endif


#define SeExprYY_ASSERT(E) ((void) (0 && (E)))

#if !defined SeExproverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef SeExprYYSTACK_USE_ALLOCA
#  if SeExprYYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define SeExprYYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define SeExprYYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define SeExprYYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef SeExprYYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define SeExprYYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef SeExprYYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define SeExprYYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define SeExprYYSTACK_ALLOC SeExprYYMALLOC
#  define SeExprYYSTACK_FREE SeExprYYFREE
#  ifndef SeExprYYSTACK_ALLOC_MAXIMUM
#   define SeExprYYSTACK_ALLOC_MAXIMUM SeExprYYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined SeExprYYMALLOC || defined malloc) \
             && (defined SeExprYYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef SeExprYYMALLOC
#   define SeExprYYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (SeExprYYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef SeExprYYFREE
#   define SeExprYYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined SeExproverflow */

#if (! defined SeExproverflow \
     && (! defined __cplusplus \
         || (defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL \
             && defined SeExprYYSTYPE_IS_TRIVIAL && SeExprYYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union SeExpralloc
{
  SeExpr_state_t SeExprss_alloc;
  SeExprYYSTYPE SeExprvs_alloc;
  SeExprYYLTYPE SeExprls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define SeExprYYSTACK_GAP_MAXIMUM (SeExprYYSIZEOF (union SeExpralloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define SeExprYYSTACK_BYTES(N) \
     ((N) * (SeExprYYSIZEOF (SeExpr_state_t) + SeExprYYSIZEOF (SeExprYYSTYPE) \
             + SeExprYYSIZEOF (SeExprYYLTYPE)) \
      + 2 * SeExprYYSTACK_GAP_MAXIMUM)

# define SeExprYYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables SeExprYYSIZE and SeExprYYSTACKSIZE give the old and new number of
   elements in the stack, and SeExprYYPTR gives the new location of the
   stack.  Advance SeExprYYPTR to a properly aligned location for the next
   stack.  */
# define SeExprYYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        SeExprYYPTRDIFF_T SeExprnewbytes;                                         \
        SeExprYYCOPY (&SeExprptr->Stack_alloc, Stack, SeExprsize);                    \
        Stack = &SeExprptr->Stack_alloc;                                    \
        SeExprnewbytes = SeExprstacksize * SeExprYYSIZEOF (*Stack) + SeExprYYSTACK_GAP_MAXIMUM; \
        SeExprptr += SeExprnewbytes / SeExprYYSIZEOF (*SeExprptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined SeExprYYCOPY_NEEDED && SeExprYYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef SeExprYYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define SeExprYYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, SeExprYY_CAST (SeExprYYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define SeExprYYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          SeExprYYPTRDIFF_T SeExpri;                      \
          for (SeExpri = 0; SeExpri < (Count); SeExpri++)   \
            (Dst)[SeExpri] = (Src)[SeExpri];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !SeExprYYCOPY_NEEDED */

/* SeExprYYFINAL -- State number of the termination state.  */
#define SeExprYYFINAL  52
/* SeExprYYLAST -- Last index in SeExprYYTABLE.  */
#define SeExprYYLAST   698

/* SeExprYYNTOKENS -- Number of terminals.  */
#define SeExprYYNTOKENS  52
/* SeExprYYNNTS -- Number of nonterminals.  */
#define SeExprYYNNTS  21
/* SeExprYYNRULES -- Number of rules.  */
#define SeExprYYNRULES  84
/* SeExprYYNSTATES -- Number of states.  */
#define SeExprYYNSTATES  187

#define SeExprYYMAXUTOK   285


/* SeExprYYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by SeExprlex, with out-of-bounds checking.  */
#define SeExprYYTRANSLATE(SeExprYYX)                                \
  (0 <= (SeExprYYX) && (SeExprYYX) <= SeExprYYMAXUTOK                     \
   ? SeExprYY_CAST (SeExprsymbol_kind_t, SeExprtranslate[SeExprYYX])        \
   : SeExprYYSYMBOL_SeExprYYUNDEF)

/* SeExprYYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by SeExprlex.  */
static const SeExprtype_int8 SeExprtranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,     2,     2,    40,     2,     2,
      23,    24,    38,    36,    49,    37,     2,    39,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,    51,
      32,    50,    33,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    48,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,    43,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    25,    28,
      29,    30,    31,    34,    35,    41
};

#if SeExprYYDEBUG
  /* SeExprYYRLINE[SeExprYYN] -- Source line where rule number SeExprYYN was defined.  */
static const SeExprtype_int16 SeExprrline[] =
{
       0,   121,   121,   123,   128,   129,   135,   143,   151,   161,
     162,   163,   164,   165,   169,   172,   177,   183,   184,   188,
     192,   199,   200,   204,   209,   218,   219,   224,   225,   229,
     230,   234,   235,   236,   239,   242,   245,   248,   251,   254,
     255,   258,   261,   264,   267,   270,   276,   281,   282,   283,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   314,   320,   321,   322,   323,   327,   328,
     334,   335,   340,   341,   345
};
#endif

/** Accessing symbol of state STATE.  */
#define SeExprYY_ACCESSING_SYMBOL(State) SeExprYY_CAST (SeExprsymbol_kind_t, SeExprstos[State])

#if SeExprYYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   SeExprYYSYMBOL.  No bounds checking.  */
static const char *SeExprsymbol_name (SeExprsymbol_kind_t SeExprsymbol) SeExprYY_ATTRIBUTE_UNUSED;

/* SeExprYYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at SeExprYYNTOKENS, nonterminals.  */
static const char *const SeExprtname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "EXTERN",
  "DEF", "FLOATPOINT", "STRING", "NAME", "VAR", "STR", "NUMBER",
  "LIFETIME_CONSTANT", "LIFETIME_UNIFORM", "LIFETIME_VARYING",
  "LIFETIME_ERROR", "AddEq", "SubEq", "MultEq", "DivEq", "ExpEq", "ModEq",
  "'('", "')'", "ARROW", "':'", "'?'", "OR", "AND", "EQ", "NE", "'<'",
  "'>'", "SEEXPR_LE", "SEEXPR_GE", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UNARY", "'!'", "'~'", "'^'", "'['", "'{'", "'}'", "']'", "','", "'='",
  "';'", "$accept", "module", "declarationList", "declaration",
  "lifetimeOptional", "typeDeclare", "typeListOptional", "typeList",
  "formalTypeListOptional", "formalTypeList", "block", "optassigns",
  "assigns", "assign", "ifthenelse", "optelse", "e", "exprlist", "optargs",
  "args", "arg", SeExprYY_NULLPTR
};

static const char *
SeExprsymbol_name (SeExprsymbol_kind_t SeExprsymbol)
{
  return SeExprtname[SeExprsymbol];
}
#endif

#ifdef SeExprYYPRINT
/* SeExprYYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const SeExprtype_int16 SeExprtoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    40,    41,   278,    58,    63,   279,   280,
     281,   282,    60,    62,   283,   284,    43,    45,    42,    47,
      37,   285,    33,   126,    94,    91,   123,   125,    93,    44,
      61,    59
};
#endif

#define SeExprYYPACT_NINF (-76)

#define SeExprpact_value_is_default(Yyn) \
  ((Yyn) == SeExprYYPACT_NINF)

#define SeExprYYTABLE_NINF (-1)

#define SeExprtable_value_is_error(Yyn) \
  0

  /* SeExprYYPACT[STATE-NUM] -- Index in SeExprYYTABLE of the portion describing
     STATE-NUM.  */
static const SeExprtype_int16 SeExprpact[] =
{
      67,     0,     4,    76,    24,    31,   -76,   -76,   115,   115,
     115,   115,   115,   115,    40,    67,   -76,   -76,    77,   -76,
     -76,   587,   115,    83,    78,    62,    72,    93,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,    85,   -76,   523,    61,    61,    61,    61,
     587,    68,   -76,   -76,   -76,   -76,   587,   102,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   545,   -76,   -76,   -76,   -76,   103,
     -76,   -76,    95,     4,    98,   148,   176,   201,   226,   251,
     276,   587,   105,    81,   -76,   301,   326,   351,   376,   401,
     426,   451,   476,   -76,   -76,   115,   108,   566,   623,   639,
     653,   653,   109,   109,   109,   109,   -23,   -23,    61,    61,
      61,    61,   501,    86,    87,     4,   125,   112,    90,     4,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   115,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   587,   115,   115,   -76,
      10,    78,   -76,   116,    92,   -76,    96,     4,   135,   -76,
     137,   606,   150,    31,   118,    10,   -76,   -76,     4,    77,
     153,   117,   -76,   170,   -76,   142,   -76,    77,    36,   -76,
     -76,   143,    10,   -76,   -76,   144,   -76
};

  /* SeExprYYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when SeExprYYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const SeExprtype_int8 SeExprdefact[] =
{
       0,     0,     0,     0,    75,    74,    77,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     3,     0,    29,
      31,    26,     0,     9,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    74,     0,    62,    63,    64,    65,
      78,     0,     1,     5,     2,    30,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    11,    12,    13,     0,
      14,    16,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    81,    82,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    54,    55,
      56,    57,    58,    59,    60,    61,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    17,     0,     0,    22,    21,
      40,    41,    42,    43,    44,    45,    72,     0,    39,    33,
      34,    35,    36,    37,    38,    32,    79,    80,     0,    52,
      27,     9,    19,     0,    18,    23,     0,     0,     0,    83,
       0,    53,     0,     0,     0,    28,    15,     6,     0,     0,
       0,     0,    73,    47,    20,     0,    24,     0,     0,    46,
       8,     0,    27,    49,     7,     0,    48
};

  /* SeExprYYPGOTO[NTERM-NUM].  */
static const SeExprtype_int16 SeExprpgoto[] =
{
     -76,   -76,   -76,   179,   -14,    -2,   -76,   -76,    66,   -76,
     -13,    14,   -75,   -15,    19,   -76,    -4,   -76,    51,   -76,
      65
};

  /* SeExprYYDEFGOTO[NTERM-NUM].  */
static const SeExprtype_int16 SeExprdefgoto[] =
{
      -1,    14,    15,    16,    80,   126,   153,   154,   127,   128,
      17,   164,    18,    19,    20,   179,    21,    51,    92,    93,
      94
};

  /* SeExprYYTABLE[SeExprYYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If SeExprYYTABLE_NINF, syntax error.  */
static const SeExprtype_uint8 SeExprtable[] =
{
      25,    27,    54,    55,    45,    46,    47,    48,    49,    50,
      81,    23,    24,     1,    56,    69,    70,    71,    74,   162,
     163,    72,    73,    22,    85,    86,    87,    88,    89,    90,
      91,    95,    96,    97,    98,    99,   100,   101,   102,     1,
      52,    28,    29,    30,    31,    32,    33,    34,    36,    37,
      38,    39,    40,    41,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
       1,    82,     2,     3,    35,   165,     4,     5,     6,     7,
       1,    42,   182,    23,    24,    26,     4,     5,     6,     7,
       8,    75,    76,    77,    78,    83,    75,    76,    77,    78,
       8,   146,    84,     9,    10,    72,    73,   165,    34,    11,
      12,   106,    13,     9,    10,   124,   104,   105,   125,    11,
      12,   129,    13,   152,    43,    44,     6,     7,    79,   136,
     137,   147,   150,    91,   155,   151,   156,   166,     8,   157,
     167,   168,   169,    91,   161,    67,    68,    69,    70,    71,
      55,     9,    10,    72,    73,   170,   175,    11,    12,   171,
      13,   172,   176,   177,   181,   173,   174,    28,    29,    30,
      31,    32,    33,    57,   178,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,   180,
     184,   186,    72,    73,    53,   158,   185,   183,   160,   130,
      35,    57,   159,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   131,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   132,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   133,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   134,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   135,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   138,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   139,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   140,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   141,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   142,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   143,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,     0,     0,     0,
       0,    57,   144,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,     0,     0,     0,     0,    57,   145,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,     0,     0,     0,    72,    73,   103,    57,   149,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,     0,     0,     0,    72,    73,   123,
      57,     0,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,     0,     0,     0,    72,
      73,    57,   148,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,    57,     0,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,     0,     0,
       0,    72,    73,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
      72,    73,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,     0,     0,     0,    72,    73,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
       0,     0,     0,    72,    73,    63,    64,    65,    66,    67,
      68,    69,    70,    71,     0,     0,     0,    72,    73
};

static const SeExprtype_int16 SeExprcheck[] =
{
       2,     3,    15,    18,     8,     9,    10,    11,    12,    13,
      24,     7,     8,     3,    18,    38,    39,    40,    22,     9,
      10,    44,    45,    23,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     3,
       0,    17,    18,    19,    20,    21,    22,    23,    17,    18,
      19,    20,    21,    22,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
       3,     9,     5,     6,    50,   150,     9,    10,    11,    12,
       3,    50,    46,     7,     8,     9,     9,    10,    11,    12,
      23,    13,    14,    15,    16,    23,    13,    14,    15,    16,
      23,   105,     9,    36,    37,    44,    45,   182,    23,    42,
      43,     9,    45,    36,    37,    12,    48,    49,    23,    42,
      43,    23,    45,   125,     9,    10,    11,    12,    45,    24,
      49,    23,    46,   137,     9,    48,    24,   151,    23,    49,
      24,    49,    46,   147,   148,    36,    37,    38,    39,    40,
     165,    36,    37,    44,    45,   157,   169,    42,    43,    24,
      45,    24,     9,    46,   177,    47,   168,    17,    18,    19,
      20,    21,    22,    25,     4,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    47,
      47,    47,    44,    45,    15,   129,   182,   178,   147,    51,
      50,    25,   137,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      -1,    25,    51,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    -1,    25,    51,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    24,    25,    48,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    44,    45,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    44,
      45,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    44,    45,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      44,    45,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    44,    45,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    44,    45,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    44,    45
};

  /* SeExprYYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const SeExprtype_int8 SeExprstos[] =
{
       0,     3,     5,     6,     9,    10,    11,    12,    23,    36,
      37,    42,    43,    45,    53,    54,    55,    62,    64,    65,
      66,    68,    23,     7,     8,    57,     9,    57,    17,    18,
      19,    20,    21,    22,    23,    50,    17,    18,    19,    20,
      21,    22,    50,     9,    10,    68,    68,    68,    68,    68,
      68,    69,     0,    55,    62,    65,    68,    25,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    44,    45,    68,    13,    14,    15,    16,    45,
      56,    56,     9,    23,     9,    68,    68,    68,    68,    68,
      68,    68,    70,    71,    72,    68,    68,    68,    68,    68,
      68,    68,    68,    24,    48,    49,     9,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    24,    12,    23,    57,    60,    61,    23,
      51,    51,    51,    51,    51,    51,    24,    49,    51,    51,
      51,    51,    51,    51,    51,    51,    68,    23,    26,    48,
      46,    48,    57,    58,    59,     9,    24,    49,    60,    72,
      70,    68,     9,    10,    63,    64,    56,    24,    49,    46,
      57,    24,    24,    47,    57,    62,     9,    46,     4,    67,
      47,    62,    46,    66,    47,    63,    47
};

  /* SeExprYYR1[SeExprYYN] -- Symbol number of symbol that rule SeExprYYN derives.  */
static const SeExprtype_int8 SeExprr1[] =
{
       0,    52,    53,    53,    54,    54,    55,    55,    55,    56,
      56,    56,    56,    56,    57,    57,    57,    58,    58,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    66,    67,    67,    67,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    69,    69,
      70,    70,    71,    71,    72
};

  /* SeExprYYR2[SeExprYYN] -- Number of symbols on the right hand side of rule SeExprYYN.  */
static const SeExprtype_int8 SeExprr2[] =
{
       0,     2,     2,     1,     1,     2,     6,     9,     8,     0,
       1,     1,     1,     1,     2,     5,     2,     0,     1,     1,
       3,     0,     1,     2,     4,     2,     1,     0,     1,     1,
       2,     1,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     8,     0,     4,     2,
       3,     3,     4,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     4,     6,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     1
};


enum { SeExprYYENOMEM = -2 };

#define SeExprerrok         (SeExprerrstatus = 0)
#define SeExprclearin       (SeExprchar = SeExprYYEMPTY)

#define SeExprYYACCEPT        goto SeExpracceptlab
#define SeExprYYABORT         goto SeExprabortlab
#define SeExprYYERROR         goto SeExprerrorlab


#define SeExprYYRECOVERING()  (!!SeExprerrstatus)

#define SeExprYYBACKUP(Token, Value)                                    \
  do                                                              \
    if (SeExprchar == SeExprYYEMPTY)                                        \
      {                                                           \
        SeExprchar = (Token);                                         \
        SeExprlval = (Value);                                         \
        SeExprYYPOPSTACK (SeExprlen);                                       \
        SeExprstate = *SeExprssp;                                         \
        goto SeExprbackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        SeExprerror (SeExprYY_("syntax error: cannot back up")); \
        SeExprYYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SeExprYYerror or SeExprYYUNDEF. */
#define SeExprYYERRCODE SeExprYYUNDEF

/* SeExprYYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef SeExprYYLLOC_DEFAULT
# define SeExprYYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = SeExprYYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = SeExprYYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = SeExprYYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = SeExprYYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            SeExprYYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            SeExprYYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define SeExprYYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if SeExprYYDEBUG

# ifndef SeExprYYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define SeExprYYFPRINTF fprintf
# endif

# define SeExprYYDPRINTF(Args)                        \
do {                                            \
  if (SeExprdebug)                                  \
    SeExprYYFPRINTF Args;                             \
} while (0)


/* SeExprYY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef SeExprYY_LOCATION_PRINT
#  if defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL

/* Print *SeExprYYLOCP on SeExprYYO.  Private, do not rely on its existence. */

SeExprYY_ATTRIBUTE_UNUSED
static int
SeExpr_location_print_ (FILE *SeExpro, SeExprYYLTYPE const * const SeExprlocp)
{
  int res = 0;
  int end_col = 0 != SeExprlocp->last_column ? SeExprlocp->last_column - 1 : 0;
  if (0 <= SeExprlocp->first_line)
    {
      res += SeExprYYFPRINTF (SeExpro, "%d", SeExprlocp->first_line);
      if (0 <= SeExprlocp->first_column)
        res += SeExprYYFPRINTF (SeExpro, ".%d", SeExprlocp->first_column);
    }
  if (0 <= SeExprlocp->last_line)
    {
      if (SeExprlocp->first_line < SeExprlocp->last_line)
        {
          res += SeExprYYFPRINTF (SeExpro, "-%d", SeExprlocp->last_line);
          if (0 <= end_col)
            res += SeExprYYFPRINTF (SeExpro, ".%d", end_col);
        }
      else if (0 <= end_col && SeExprlocp->first_column < end_col)
        res += SeExprYYFPRINTF (SeExpro, "-%d", end_col);
    }
  return res;
 }

#   define SeExprYY_LOCATION_PRINT(File, Loc)          \
  SeExpr_location_print_ (File, &(Loc))

#  else
#   define SeExprYY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined SeExprYY_LOCATION_PRINT */


# define SeExprYY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (SeExprdebug)                                                            \
    {                                                                     \
      SeExprYYFPRINTF (stderr, "%s ", Title);                                   \
      SeExpr_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      SeExprYYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on SeExprYYO.  |
`-----------------------------------*/

static void
SeExpr_symbol_value_print (FILE *SeExpro,
                       SeExprsymbol_kind_t SeExprkind, SeExprYYSTYPE const * const SeExprvaluep, SeExprYYLTYPE const * const SeExprlocationp)
{
  FILE *SeExproutput = SeExpro;
  SeExprYYUSE (SeExproutput);
  SeExprYYUSE (SeExprlocationp);
  if (!SeExprvaluep)
    return;
# ifdef SeExprYYPRINT
  if (SeExprkind < SeExprYYNTOKENS)
    SeExprYYPRINT (SeExpro, SeExprtoknum[SeExprkind], *SeExprvaluep);
# endif
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  SeExprYYUSE (SeExprkind);
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on SeExprYYO.  |
`---------------------------*/

static void
SeExpr_symbol_print (FILE *SeExpro,
                 SeExprsymbol_kind_t SeExprkind, SeExprYYSTYPE const * const SeExprvaluep, SeExprYYLTYPE const * const SeExprlocationp)
{
  SeExprYYFPRINTF (SeExpro, "%s %s (",
             SeExprkind < SeExprYYNTOKENS ? "token" : "nterm", SeExprsymbol_name (SeExprkind));

  SeExprYY_LOCATION_PRINT (SeExpro, *SeExprlocationp);
  SeExprYYFPRINTF (SeExpro, ": ");
  SeExpr_symbol_value_print (SeExpro, SeExprkind, SeExprvaluep, SeExprlocationp);
  SeExprYYFPRINTF (SeExpro, ")");
}

/*------------------------------------------------------------------.
| SeExpr_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
SeExpr_stack_print (SeExpr_state_t *SeExprbottom, SeExpr_state_t *SeExprtop)
{
  SeExprYYFPRINTF (stderr, "Stack now");
  for (; SeExprbottom <= SeExprtop; SeExprbottom++)
    {
      int SeExprbot = *SeExprbottom;
      SeExprYYFPRINTF (stderr, " %d", SeExprbot);
    }
  SeExprYYFPRINTF (stderr, "\n");
}

# define SeExprYY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (SeExprdebug)                                                  \
    SeExpr_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the SeExprYYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
SeExpr_reduce_print (SeExpr_state_t *SeExprssp, SeExprYYSTYPE *SeExprvsp, SeExprYYLTYPE *SeExprlsp,
                 int SeExprrule)
{
  int SeExprlno = SeExprrline[SeExprrule];
  int SeExprnrhs = SeExprr2[SeExprrule];
  int SeExpri;
  SeExprYYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             SeExprrule - 1, SeExprlno);
  /* The symbols being reduced.  */
  for (SeExpri = 0; SeExpri < SeExprnrhs; SeExpri++)
    {
      SeExprYYFPRINTF (stderr, "   $%d = ", SeExpri + 1);
      SeExpr_symbol_print (stderr,
                       SeExprYY_ACCESSING_SYMBOL (+SeExprssp[SeExpri + 1 - SeExprnrhs]),
                       &SeExprvsp[(SeExpri + 1) - (SeExprnrhs)],
                       &(SeExprlsp[(SeExpri + 1) - (SeExprnrhs)]));
      SeExprYYFPRINTF (stderr, "\n");
    }
}

# define SeExprYY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (SeExprdebug)                          \
    SeExpr_reduce_print (SeExprssp, SeExprvsp, SeExprlsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int SeExprdebug;
#else /* !SeExprYYDEBUG */
# define SeExprYYDPRINTF(Args) ((void) 0)
# define SeExprYY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define SeExprYY_STACK_PRINT(Bottom, Top)
# define SeExprYY_REDUCE_PRINT(Rule)
#endif /* !SeExprYYDEBUG */


/* SeExprYYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef SeExprYYINITDEPTH
# define SeExprYYINITDEPTH 200
#endif

/* SeExprYYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SeExprYYSTACK_ALLOC_MAXIMUM < SeExprYYSTACK_BYTES (SeExprYYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef SeExprYYMAXDEPTH
# define SeExprYYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
SeExprdestruct (const char *SeExprmsg,
            SeExprsymbol_kind_t SeExprkind, SeExprYYSTYPE *SeExprvaluep, SeExprYYLTYPE *SeExprlocationp)
{
  SeExprYYUSE (SeExprvaluep);
  SeExprYYUSE (SeExprlocationp);
  if (!SeExprmsg)
    SeExprmsg = "Deleting";
  SeExprYY_SYMBOL_PRINT (SeExprmsg, SeExprkind, SeExprvaluep, SeExprlocationp);

  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  SeExprYYUSE (SeExprkind);
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* The lookahead symbol.  */
int SeExprchar;

/* The semantic value of the lookahead symbol.  */
SeExprYYSTYPE SeExprlval;
/* Location data for the lookahead symbol.  */
SeExprYYLTYPE SeExprlloc
# if defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int SeExprnerrs;




/*----------.
| SeExprparse.  |
`----------*/

int
SeExprparse (void)
{
    SeExpr_state_fast_t SeExprstate;
    /* Number of tokens to shift before error messages enabled.  */
    int SeExprerrstatus;

    /* The stacks and their tools:
       'SeExprss': related to states.
       'SeExprvs': related to semantic values.
       'SeExprls': related to locations.

       Refer to the stacks through separate pointers, to allow SeExproverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    SeExprYYPTRDIFF_T SeExprstacksize;

    /* The state stack.  */
    SeExpr_state_t SeExprssa[SeExprYYINITDEPTH];
    SeExpr_state_t *SeExprss;
    SeExpr_state_t *SeExprssp;

    /* The semantic value stack.  */
    SeExprYYSTYPE SeExprvsa[SeExprYYINITDEPTH];
    SeExprYYSTYPE *SeExprvs;
    SeExprYYSTYPE *SeExprvsp;

    /* The location stack.  */
    SeExprYYLTYPE SeExprlsa[SeExprYYINITDEPTH];
    SeExprYYLTYPE *SeExprls;
    SeExprYYLTYPE *SeExprlsp;

  int SeExprn;
  /* The return value of SeExprparse.  */
  int SeExprresult;
  /* Lookahead token as an internal (translated) token number.  */
  SeExprsymbol_kind_t SeExprtoken = SeExprYYSYMBOL_SeExprYYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  SeExprYYSTYPE SeExprval;
  SeExprYYLTYPE SeExprloc;

  /* The locations where the error started and ended.  */
  SeExprYYLTYPE SeExprerror_range[3];



#define SeExprYYPOPSTACK(N)   (SeExprvsp -= (N), SeExprssp -= (N), SeExprlsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int SeExprlen = 0;

  SeExprnerrs = 0;
  SeExprstate = 0;
  SeExprerrstatus = 0;

  SeExprstacksize = SeExprYYINITDEPTH;
  SeExprssp = SeExprss = SeExprssa;
  SeExprvsp = SeExprvs = SeExprvsa;
  SeExprlsp = SeExprls = SeExprlsa;


  SeExprYYDPRINTF ((stderr, "Starting parse\n"));

  SeExprchar = SeExprYYEMPTY; /* Cause a token to be read.  */
  SeExprlsp[0] = SeExprlloc;
  goto SeExprsetstate;


/*------------------------------------------------------------.
| SeExprnewstate -- push a new state, which is found in SeExprstate.  |
`------------------------------------------------------------*/
SeExprnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  SeExprssp++;


/*--------------------------------------------------------------------.
| SeExprsetstate -- set current state (the top of the stack) to SeExprstate.  |
`--------------------------------------------------------------------*/
SeExprsetstate:
  SeExprYYDPRINTF ((stderr, "Entering state %d\n", SeExprstate));
  SeExprYY_ASSERT (0 <= SeExprstate && SeExprstate < SeExprYYNSTATES);
  SeExprYY_IGNORE_USELESS_CAST_BEGIN
  *SeExprssp = SeExprYY_CAST (SeExpr_state_t, SeExprstate);
  SeExprYY_IGNORE_USELESS_CAST_END
  SeExprYY_STACK_PRINT (SeExprss, SeExprssp);

  if (SeExprss + SeExprstacksize - 1 <= SeExprssp)
#if !defined SeExproverflow && !defined SeExprYYSTACK_RELOCATE
    goto SeExprexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      SeExprYYPTRDIFF_T SeExprsize = SeExprssp - SeExprss + 1;

# if defined SeExproverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        SeExpr_state_t *SeExprss1 = SeExprss;
        SeExprYYSTYPE *SeExprvs1 = SeExprvs;
        SeExprYYLTYPE *SeExprls1 = SeExprls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if SeExproverflow is a macro.  */
        SeExproverflow (SeExprYY_("memory exhausted"),
                    &SeExprss1, SeExprsize * SeExprYYSIZEOF (*SeExprssp),
                    &SeExprvs1, SeExprsize * SeExprYYSIZEOF (*SeExprvsp),
                    &SeExprls1, SeExprsize * SeExprYYSIZEOF (*SeExprlsp),
                    &SeExprstacksize);
        SeExprss = SeExprss1;
        SeExprvs = SeExprvs1;
        SeExprls = SeExprls1;
      }
# else /* defined SeExprYYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (SeExprYYMAXDEPTH <= SeExprstacksize)
        goto SeExprexhaustedlab;
      SeExprstacksize *= 2;
      if (SeExprYYMAXDEPTH < SeExprstacksize)
        SeExprstacksize = SeExprYYMAXDEPTH;

      {
        SeExpr_state_t *SeExprss1 = SeExprss;
        union SeExpralloc *SeExprptr =
          SeExprYY_CAST (union SeExpralloc *,
                   SeExprYYSTACK_ALLOC (SeExprYY_CAST (SeExprYYSIZE_T, SeExprYYSTACK_BYTES (SeExprstacksize))));
        if (! SeExprptr)
          goto SeExprexhaustedlab;
        SeExprYYSTACK_RELOCATE (SeExprss_alloc, SeExprss);
        SeExprYYSTACK_RELOCATE (SeExprvs_alloc, SeExprvs);
        SeExprYYSTACK_RELOCATE (SeExprls_alloc, SeExprls);
#  undef SeExprYYSTACK_RELOCATE
        if (SeExprss1 != SeExprssa)
          SeExprYYSTACK_FREE (SeExprss1);
      }
# endif

      SeExprssp = SeExprss + SeExprsize - 1;
      SeExprvsp = SeExprvs + SeExprsize - 1;
      SeExprlsp = SeExprls + SeExprsize - 1;

      SeExprYY_IGNORE_USELESS_CAST_BEGIN
      SeExprYYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  SeExprYY_CAST (long, SeExprstacksize)));
      SeExprYY_IGNORE_USELESS_CAST_END

      if (SeExprss + SeExprstacksize - 1 <= SeExprssp)
        SeExprYYABORT;
    }
#endif /* !defined SeExproverflow && !defined SeExprYYSTACK_RELOCATE */

  if (SeExprstate == SeExprYYFINAL)
    SeExprYYACCEPT;

  goto SeExprbackup;


/*-----------.
| SeExprbackup.  |
`-----------*/
SeExprbackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  SeExprn = SeExprpact[SeExprstate];
  if (SeExprpact_value_is_default (SeExprn))
    goto SeExprdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* SeExprYYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (SeExprchar == SeExprYYEMPTY)
    {
      SeExprYYDPRINTF ((stderr, "Reading a token\n"));
      SeExprchar = SeExprlex ();
    }

  if (SeExprchar <= SeExprYYEOF)
    {
      SeExprchar = SeExprYYEOF;
      SeExprtoken = SeExprYYSYMBOL_SeExprYYEOF;
      SeExprYYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (SeExprchar == SeExprYYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      SeExprchar = SeExprYYUNDEF;
      SeExprtoken = SeExprYYSYMBOL_SeExprYYerror;
      SeExprerror_range[1] = SeExprlloc;
      goto SeExprerrlab1;
    }
  else
    {
      SeExprtoken = SeExprYYTRANSLATE (SeExprchar);
      SeExprYY_SYMBOL_PRINT ("Next token is", SeExprtoken, &SeExprlval, &SeExprlloc);
    }

  /* If the proper action on seeing token SeExprYYTOKEN is to reduce or to
     detect an error, take that action.  */
  SeExprn += SeExprtoken;
  if (SeExprn < 0 || SeExprYYLAST < SeExprn || SeExprcheck[SeExprn] != SeExprtoken)
    goto SeExprdefault;
  SeExprn = SeExprtable[SeExprn];
  if (SeExprn <= 0)
    {
      if (SeExprtable_value_is_error (SeExprn))
        goto SeExprerrlab;
      SeExprn = -SeExprn;
      goto SeExprreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (SeExprerrstatus)
    SeExprerrstatus--;

  /* Shift the lookahead token.  */
  SeExprYY_SYMBOL_PRINT ("Shifting", SeExprtoken, &SeExprlval, &SeExprlloc);
  SeExprstate = SeExprn;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++SeExprvsp = SeExprlval;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
  *++SeExprlsp = SeExprlloc;

  /* Discard the shifted token.  */
  SeExprchar = SeExprYYEMPTY;
  goto SeExprnewstate;


/*-----------------------------------------------------------.
| SeExprdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
SeExprdefault:
  SeExprn = SeExprdefact[SeExprstate];
  if (SeExprn == 0)
    goto SeExprerrlab;
  goto SeExprreduce;


/*-----------------------------.
| SeExprreduce -- do a reduction.  |
`-----------------------------*/
SeExprreduce:
  /* SeExprn is the number of a rule to reduce with.  */
  SeExprlen = SeExprr2[SeExprn];

  /* If SeExprYYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets SeExprYYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to SeExprYYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that SeExprYYVAL may be used uninitialized.  */
  SeExprval = SeExprvsp[1-SeExprlen];

  /* Default location. */
  SeExprYYLLOC_DEFAULT (SeExprloc, (SeExprlsp - SeExprlen), SeExprlen);
  SeExprerror_range[1] = SeExprloc;
  SeExprYY_REDUCE_PRINT (SeExprn);
  switch (SeExprn)
    {
  case 2:
#line 121 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { ParseResult = (SeExprvsp[-1].n); ParseResult->setPosition((SeExprloc).first_column, (SeExprloc).last_column);
                                  ParseResult->addChild((SeExprvsp[0].n)); }
#line 1629 "y.tab.c"
    break;

  case 3:
#line 123 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { ParseResult = NODE((SeExprloc).first_column, (SeExprloc).last_column, ModuleNode);
                                  ParseResult->addChild((SeExprvsp[0].n)); }
#line 1636 "y.tab.c"
    break;

  case 4:
#line 128 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column, (SeExprloc).last_column, ModuleNode); (SeExprval.n)->addChild((SeExprvsp[0].n)); }
#line 1642 "y.tab.c"
    break;

  case 5:
#line 130 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-1].n); (SeExprval.n)->setPosition((SeExprloc).first_column, (SeExprloc).last_column);
                                  (SeExprval.n)->addChild((SeExprvsp[0].n)); }
#line 1649 "y.tab.c"
    break;

  case 6:
#line 136 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[-4].t).type, (SeExprvsp[-4].t).dim, (SeExprvsp[-4].t).lifetime);
                                    KSeExpr::ExprPrototypeNode * prototype =
                                        (KSeExpr::ExprPrototypeNode*)NODE2((SeExprloc).first_column, (SeExprloc).last_column, PrototypeNode, (SeExprvsp[-3].s), type);
                                  prototype->addArgTypes((SeExprvsp[-1].n));
                                  Forget((SeExprvsp[-1].n));
                                  (SeExprval.n) = prototype;
                                  free((SeExprvsp[-3].s)); }
#line 1661 "y.tab.c"
    break;

  case 7:
#line 144 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[-7].t).type, (SeExprvsp[-7].t).dim, (SeExprvsp[-7].t).lifetime);
                                  KSeExpr::ExprPrototypeNode * prototype =
                                      (KSeExpr::ExprPrototypeNode*)NODE2((SeExprloc).first_column, (SeExprlsp[-3]).last_column, PrototypeNode, (SeExprvsp[-6].s), type);
                                  prototype->addArgs((SeExprvsp[-4].n));
                                  Forget((SeExprvsp[-4].n));
                                  (SeExprval.n) = NODE2((SeExprloc).first_column, (SeExprloc).last_column, LocalFunctionNode, prototype, (SeExprvsp[-1].n));
                                  free((SeExprvsp[-6].s)); }
#line 1673 "y.tab.c"
    break;

  case 8:
#line 152 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { KSeExpr::ExprPrototypeNode * prototype =
                                        (KSeExpr::ExprPrototypeNode*)NODE1((SeExprloc).first_column, (SeExprlsp[-3]).last_column, PrototypeNode, (SeExprvsp[-6].s));
                                  prototype->addArgs((SeExprvsp[-4].n));
                                  Forget((SeExprvsp[-4].n));
                                  (SeExprval.n) = NODE2((SeExprloc).first_column, (SeExprloc).last_column, LocalFunctionNode, prototype, (SeExprvsp[-1].n));
                                  free((SeExprvsp[-6].s)); }
#line 1684 "y.tab.c"
    break;

  case 9:
#line 161 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.l) = KSeExpr::ExprType::ltVARYING; }
#line 1690 "y.tab.c"
    break;

  case 10:
#line 162 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.l) = KSeExpr::ExprType::ltCONSTANT; }
#line 1696 "y.tab.c"
    break;

  case 11:
#line 163 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.l) = KSeExpr::ExprType::ltUNIFORM; }
#line 1702 "y.tab.c"
    break;

  case 12:
#line 164 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.l) = KSeExpr::ExprType::ltVARYING; }
#line 1708 "y.tab.c"
    break;

  case 13:
#line 165 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.l) = KSeExpr::ExprType::ltERROR; }
#line 1714 "y.tab.c"
    break;

  case 14:
#line 169 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                 {(SeExprval.t).type     = KSeExpr::ExprType::tFP;
                                  (SeExprval.t).dim      = 1;
                                  (SeExprval.t).lifetime = (SeExprvsp[0].l); }
#line 1722 "y.tab.c"
    break;

  case 15:
#line 173 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.t).type = ((SeExprvsp[-2].d) > 0 ? KSeExpr::ExprType::tFP : KSeExpr::ExprType::tERROR);
                                  //TODO: This causes an error but does not report it to user. Change this.
                                  (SeExprval.t).dim  = ((SeExprvsp[-2].d) > 0 ? (SeExprvsp[-2].d) : 0);
                                  (SeExprval.t).lifetime = (SeExprvsp[0].l); }
#line 1731 "y.tab.c"
    break;

  case 16:
#line 177 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.t).type = KSeExpr::ExprType::tSTRING;
                                  (SeExprval.t).dim  = 1;
                                  (SeExprval.t).lifetime = (SeExprvsp[0].l); }
#line 1739 "y.tab.c"
    break;

  case 17:
#line 183 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column, (SeExprloc).last_column, Node); }
#line 1745 "y.tab.c"
    break;

  case 18:
#line 184 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1751 "y.tab.c"
    break;

  case 19:
#line 188 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column, (SeExprloc).last_column, Node);
                                  KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[0].t).type, (SeExprvsp[0].t).dim, (SeExprvsp[0].t).lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2((SeExprloc).first_column, (SeExprloc).last_column, VarNode, "", type);
                                  (SeExprval.n)->addChild(varNode); }
#line 1760 "y.tab.c"
    break;

  case 20:
#line 192 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-2].n);
                                  KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[0].t).type, (SeExprvsp[0].t).dim, (SeExprvsp[0].t).lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2((SeExprlsp[0]).first_column, (SeExprlsp[0]).last_column, VarNode, "", type);
                                  (SeExprval.n)->addChild(varNode); }
#line 1769 "y.tab.c"
    break;

  case 21:
#line 199 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column, (SeExprloc).last_column, Node); }
#line 1775 "y.tab.c"
    break;

  case 22:
#line 200 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1781 "y.tab.c"
    break;

  case 23:
#line 204 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                 { (SeExprval.n) = NODE((SeExprloc).first_column, (SeExprloc).last_column, Node);
                                  KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[-1].t).type, (SeExprvsp[-1].t).dim, (SeExprvsp[-1].t).lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2((SeExprloc).first_column, (SeExprloc).last_column, VarNode, (SeExprvsp[0].s), type);
                                  (SeExprval.n)->addChild(varNode);
                                  free((SeExprvsp[0].s)); }
#line 1791 "y.tab.c"
    break;

  case 24:
#line 210 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-3].n);
                                  KSeExpr::ExprType type = KSeExpr::ExprType((SeExprvsp[-1].t).type, (SeExprvsp[-1].t).dim, (SeExprvsp[-1].t).lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2((SeExprlsp[-1]).first_column, (SeExprlsp[0]).last_column, VarNode, (SeExprvsp[0].s), type);
                                  (SeExprval.n)->addChild(varNode);
                                  free((SeExprvsp[0].s)); }
#line 1801 "y.tab.c"
    break;

  case 25:
#line 218 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,BlockNode, (SeExprvsp[-1].n), (SeExprvsp[0].n)); }
#line 1807 "y.tab.c"
    break;

  case 26:
#line 219 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1813 "y.tab.c"
    break;

  case 27:
#line 224 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column,(SeExprloc).last_column,Node); /* create empty node */; }
#line 1819 "y.tab.c"
    break;

  case 28:
#line 225 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1825 "y.tab.c"
    break;

  case 29:
#line 229 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,Node, (SeExprvsp[0].n)); /* create var list */}
#line 1831 "y.tab.c"
    break;

  case 30:
#line 230 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-1].n); (SeExprvsp[-1].n)->addChild((SeExprvsp[0].n)); /* add to list */}
#line 1837 "y.tab.c"
    break;

  case 31:
#line 234 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1843 "y.tab.c"
    break;

  case 32:
#line 235 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), (SeExprvsp[-1].n)); free((SeExprvsp[-3].s)); }
#line 1849 "y.tab.c"
    break;

  case 33:
#line 236 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                   {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'+');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1857 "y.tab.c"
    break;

  case 34:
#line 239 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                   {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'-');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1865 "y.tab.c"
    break;

  case 35:
#line 242 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'*');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1873 "y.tab.c"
    break;

  case 36:
#line 245 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                   {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'/');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1881 "y.tab.c"
    break;

  case 37:
#line 248 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                   {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'^');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1889 "y.tab.c"
    break;

  case 38:
#line 251 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                   {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'%');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1897 "y.tab.c"
    break;

  case 39:
#line 254 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), (SeExprvsp[-1].n)); free((SeExprvsp[-3].s)); }
#line 1903 "y.tab.c"
    break;

  case 40:
#line 255 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'+');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1911 "y.tab.c"
    break;

  case 41:
#line 258 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'-');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1919 "y.tab.c"
    break;

  case 42:
#line 261 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                     {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'*');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1927 "y.tab.c"
    break;

  case 43:
#line 264 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'/');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1935 "y.tab.c"
    break;

  case 44:
#line 267 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'^');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1943 "y.tab.c"
    break;

  case 45:
#line 270 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                    {KSeExpr::ExprNode* varNode=NODE1((SeExprlsp[-3]).first_column,(SeExprlsp[-3]).first_column,VarNode, (SeExprvsp[-3].s));
                               KSeExpr::ExprNode* opNode=NODE3((SeExprlsp[-1]).first_column,(SeExprlsp[-1]).first_column,BinaryOpNode,varNode,(SeExprvsp[-1].n),'%');
                                (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,AssignNode, (SeExprvsp[-3].s), opNode);free((SeExprvsp[-3].s));}
#line 1951 "y.tab.c"
    break;

  case 46:
#line 277 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,IfThenElseNode, (SeExprvsp[-5].n), (SeExprvsp[-2].n), (SeExprvsp[0].n)); }
#line 1957 "y.tab.c"
    break;

  case 47:
#line 281 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column,(SeExprloc).last_column,Node); /* create empty node */ }
#line 1963 "y.tab.c"
    break;

  case 48:
#line 282 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-1].n); }
#line 1969 "y.tab.c"
    break;

  case 49:
#line 283 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 1975 "y.tab.c"
    break;

  case 50:
#line 288 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-1].n); }
#line 1981 "y.tab.c"
    break;

  case 51:
#line 289 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { KSeExpr::ExprNode* newNode = NODE((SeExprloc).first_column,(SeExprloc).last_column,VecNode); newNode->addChildren((SeExprvsp[-1].n)); Forget((SeExprvsp[-1].n)); (SeExprval.n)=newNode;}
#line 1987 "y.tab.c"
    break;

  case 52:
#line 290 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,SubscriptNode, (SeExprvsp[-3].n), (SeExprvsp[-1].n)); }
#line 1993 "y.tab.c"
    break;

  case 53:
#line 291 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CondNode, (SeExprvsp[-4].n), (SeExprvsp[-2].n), (SeExprvsp[0].n)); }
#line 1999 "y.tab.c"
    break;

  case 54:
#line 292 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '|'); }
#line 2005 "y.tab.c"
    break;

  case 55:
#line 293 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '&'); }
#line 2011 "y.tab.c"
    break;

  case 56:
#line 294 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareEqNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'='); }
#line 2017 "y.tab.c"
    break;

  case 57:
#line 295 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareEqNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'!'); }
#line 2023 "y.tab.c"
    break;

  case 58:
#line 296 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'<'); }
#line 2029 "y.tab.c"
    break;

  case 59:
#line 297 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'>'); }
#line 2035 "y.tab.c"
    break;

  case 60:
#line 298 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                        { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'l'); }
#line 2041 "y.tab.c"
    break;

  case 61:
#line 299 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                        { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,CompareNode, (SeExprvsp[-2].n), (SeExprvsp[0].n),'g'); }
#line 2047 "y.tab.c"
    break;

  case 62:
#line 300 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 2053 "y.tab.c"
    break;

  case 63:
#line 301 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,UnaryOpNode, (SeExprvsp[0].n), '-'); }
#line 2059 "y.tab.c"
    break;

  case 64:
#line 302 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,UnaryOpNode, (SeExprvsp[0].n), '!'); }
#line 2065 "y.tab.c"
    break;

  case 65:
#line 303 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE2((SeExprloc).first_column,(SeExprloc).last_column,UnaryOpNode, (SeExprvsp[0].n), '~'); }
#line 2071 "y.tab.c"
    break;

  case 66:
#line 304 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '+'); }
#line 2077 "y.tab.c"
    break;

  case 67:
#line 305 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '-'); }
#line 2083 "y.tab.c"
    break;

  case 68:
#line 306 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '*'); }
#line 2089 "y.tab.c"
    break;

  case 69:
#line 307 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '/'); }
#line 2095 "y.tab.c"
    break;

  case 70:
#line 308 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '%'); }
#line 2101 "y.tab.c"
    break;

  case 71:
#line 309 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE3((SeExprloc).first_column,(SeExprloc).last_column,BinaryOpNode, (SeExprvsp[-2].n), (SeExprvsp[0].n), '^'); }
#line 2107 "y.tab.c"
    break;

  case 72:
#line 310 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,FuncNode, (SeExprvsp[-3].s));
				  free((SeExprvsp[-3].s)); // free name string
				  // add args directly and discard arg list node
				  (SeExprval.n)->addChildren((SeExprvsp[-1].n)); Forget((SeExprvsp[-1].n)); }
#line 2116 "y.tab.c"
    break;

  case 73:
#line 315 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,FuncNode, (SeExprvsp[-3].s));
				  free((SeExprvsp[-3].s)); // free name string
				  (SeExprval.n)->addChild((SeExprvsp[-5].n));
				  // add args directly and discard arg list node
				  (SeExprval.n)->addChildren((SeExprvsp[-1].n)); Forget((SeExprvsp[-1].n)); }
#line 2126 "y.tab.c"
    break;

  case 74:
#line 320 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,VarNode, (SeExprvsp[0].s)); free((SeExprvsp[0].s)); /* free name string */ }
#line 2132 "y.tab.c"
    break;

  case 75:
#line 321 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,VarNode, (SeExprvsp[0].s)); free((SeExprvsp[0].s)); /* free name string */ }
#line 2138 "y.tab.c"
    break;

  case 76:
#line 322 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,NumNode, (SeExprvsp[0].d)); /*printf("line %d",@$.last_column);*/}
#line 2144 "y.tab.c"
    break;

  case 77:
#line 323 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,StrNode, (SeExprvsp[0].s)); free((SeExprvsp[0].s)); /* free string */}
#line 2150 "y.tab.c"
    break;

  case 78:
#line 327 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,Node,(SeExprvsp[0].n)); }
#line 2156 "y.tab.c"
    break;

  case 79:
#line 328 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-2].n);
                                  (SeExprvsp[-2].n)->addChild((SeExprvsp[0].n)); }
#line 2163 "y.tab.c"
    break;

  case 80:
#line 334 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE((SeExprloc).first_column,(SeExprloc).last_column,Node); /* create empty node */}
#line 2169 "y.tab.c"
    break;

  case 81:
#line 335 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 2175 "y.tab.c"
    break;

  case 82:
#line 340 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = NODE1((SeExprloc).first_column,(SeExprloc).last_column,Node, (SeExprvsp[0].n)); /* create arg list */}
#line 2181 "y.tab.c"
    break;

  case 83:
#line 341 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[-2].n); (SeExprvsp[-2].n)->addChild((SeExprvsp[0].n)); /* add to list */}
#line 2187 "y.tab.c"
    break;

  case 84:
#line 345 "@@PATH@@/src/KSeExpr/ExprParser.y"
                                { (SeExprval.n) = (SeExprvsp[0].n); }
#line 2193 "y.tab.c"
    break;


#line 2197 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter SeExprchar, and that requires
     that SeExprtoken be updated with the new translation.  We take the
     approach of translating immediately before every use of SeExprtoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     SeExprYYABORT, SeExprYYACCEPT, or SeExprYYERROR immediately after altering SeExprchar or
     if it invokes SeExprYYBACKUP.  In the case of SeExprYYABORT or SeExprYYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of SeExprYYERROR or SeExprYYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  SeExprYY_SYMBOL_PRINT ("-> $$ =", SeExprYY_CAST (SeExprsymbol_kind_t, SeExprr1[SeExprn]), &SeExprval, &SeExprloc);

  SeExprYYPOPSTACK (SeExprlen);
  SeExprlen = 0;

  *++SeExprvsp = SeExprval;
  *++SeExprlsp = SeExprloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int SeExprlhs = SeExprr1[SeExprn] - SeExprYYNTOKENS;
    const int SeExpri = SeExprpgoto[SeExprlhs] + *SeExprssp;
    SeExprstate = (0 <= SeExpri && SeExpri <= SeExprYYLAST && SeExprcheck[SeExpri] == *SeExprssp
               ? SeExprtable[SeExpri]
               : SeExprdefgoto[SeExprlhs]);
  }

  goto SeExprnewstate;


/*--------------------------------------.
| SeExprerrlab -- here on detecting error.  |
`--------------------------------------*/
SeExprerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  SeExprtoken = SeExprchar == SeExprYYEMPTY ? SeExprYYSYMBOL_SeExprYYEMPTY : SeExprYYTRANSLATE (SeExprchar);
  /* If not already recovering from an error, report this error.  */
  if (!SeExprerrstatus)
    {
      ++SeExprnerrs;
      SeExprerror (SeExprYY_("syntax error"));
    }

  SeExprerror_range[1] = SeExprlloc;
  if (SeExprerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (SeExprchar <= SeExprYYEOF)
        {
          /* Return failure if at end of input.  */
          if (SeExprchar == SeExprYYEOF)
            SeExprYYABORT;
        }
      else
        {
          SeExprdestruct ("Error: discarding",
                      SeExprtoken, &SeExprlval, &SeExprlloc);
          SeExprchar = SeExprYYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto SeExprerrlab1;


/*---------------------------------------------------.
| SeExprerrorlab -- error raised explicitly by SeExprYYERROR.  |
`---------------------------------------------------*/
SeExprerrorlab:
  /* Pacify compilers when the user code never invokes SeExprYYERROR and the
     label SeExprerrorlab therefore never appears in user code.  */
  if (0)
    SeExprYYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this SeExprYYERROR.  */
  SeExprYYPOPSTACK (SeExprlen);
  SeExprlen = 0;
  SeExprYY_STACK_PRINT (SeExprss, SeExprssp);
  SeExprstate = *SeExprssp;
  goto SeExprerrlab1;


/*-------------------------------------------------------------.
| SeExprerrlab1 -- common code for both syntax error and SeExprYYERROR.  |
`-------------------------------------------------------------*/
SeExprerrlab1:
  SeExprerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      SeExprn = SeExprpact[SeExprstate];
      if (!SeExprpact_value_is_default (SeExprn))
        {
          SeExprn += SeExprYYSYMBOL_SeExprYYerror;
          if (0 <= SeExprn && SeExprn <= SeExprYYLAST && SeExprcheck[SeExprn] == SeExprYYSYMBOL_SeExprYYerror)
            {
              SeExprn = SeExprtable[SeExprn];
              if (0 < SeExprn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (SeExprssp == SeExprss)
        SeExprYYABORT;

      SeExprerror_range[1] = *SeExprlsp;
      SeExprdestruct ("Error: popping",
                  SeExprYY_ACCESSING_SYMBOL (SeExprstate), SeExprvsp, SeExprlsp);
      SeExprYYPOPSTACK (1);
      SeExprstate = *SeExprssp;
      SeExprYY_STACK_PRINT (SeExprss, SeExprssp);
    }

  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++SeExprvsp = SeExprlval;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END

  SeExprerror_range[2] = SeExprlloc;
  ++SeExprlsp;
  SeExprYYLLOC_DEFAULT (*SeExprlsp, SeExprerror_range, 2);

  /* Shift the error token.  */
  SeExprYY_SYMBOL_PRINT ("Shifting", SeExprYY_ACCESSING_SYMBOL (SeExprn), SeExprvsp, SeExprlsp);

  SeExprstate = SeExprn;
  goto SeExprnewstate;


/*-------------------------------------.
| SeExpracceptlab -- SeExprYYACCEPT comes here.  |
`-------------------------------------*/
SeExpracceptlab:
  SeExprresult = 0;
  goto SeExprreturn;


/*-----------------------------------.
| SeExprabortlab -- SeExprYYABORT comes here.  |
`-----------------------------------*/
SeExprabortlab:
  SeExprresult = 1;
  goto SeExprreturn;


#if !defined SeExproverflow
/*-------------------------------------------------.
| SeExprexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
SeExprexhaustedlab:
  SeExprerror (SeExprYY_("memory exhausted"));
  SeExprresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| SeExprreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
SeExprreturn:
  if (SeExprchar != SeExprYYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      SeExprtoken = SeExprYYTRANSLATE (SeExprchar);
      SeExprdestruct ("Cleanup: discarding lookahead",
                  SeExprtoken, &SeExprlval, &SeExprlloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this SeExprYYABORT or SeExprYYACCEPT.  */
  SeExprYYPOPSTACK (SeExprlen);
  SeExprYY_STACK_PRINT (SeExprss, SeExprssp);
  while (SeExprssp != SeExprss)
    {
      SeExprdestruct ("Cleanup: popping",
                  SeExprYY_ACCESSING_SYMBOL (+*SeExprssp), SeExprvsp, SeExprlsp);
      SeExprYYPOPSTACK (1);
    }
#ifndef SeExproverflow
  if (SeExprss != SeExprssa)
    SeExprYYSTACK_FREE (SeExprss);
#endif

  return SeExprresult;
}

#line 348 "@@PATH@@/src/KSeExpr/ExprParser.y"


      /* SeExprerror - Report an error.  This is called by the parser.
	 (Note: the "msg" param is useless as it is usually just "parse error".
	 so it's ignored.)
      */
static void SeExprerror(const char* /*msg*/)
{
    // find start of line containing error
    int pos = SeExprpos(), lineno = 1, start = 0, end = strlen(ParseStr);

    for (int i = start; i < pos; i++)
	if (ParseStr[i] == '\n') { start = i + 1; lineno++; }

    // find end of line containing error
    for (int i = end; i > pos; i--)
	if (ParseStr[i] == '\n') { end = i - 1; }

    ParseErrorCode = SeExprtext[0] ? KSeExpr::ErrorCode::SyntaxError : KSeExpr::ErrorCode::UnexpectedEndOfExpression;

    ParseErrorId = "";

    int s = std::max(start, pos-30);
    int e = std::min(end, pos+30);

    if (s != start) ParseErrorId += "...";
    ParseErrorId += std::string(ParseStr, s, e-s+1);
    if (e != end) ParseErrorId += "...";
}


/* CallParser - This is our entrypoint from the rest of the expr library.
   A string is passed in and a parse tree is returned.	If the tree is null,
   an error string is returned.  Any flags set during parsing are passed
   along.
 */

extern void SeExprLexerResetState(std::vector<std::pair<int,int> >& comments);

static std::mutex mutex;

namespace KSeExpr {
bool ExprParse(KSeExpr::ExprNode*& parseTree,
    KSeExpr::ErrorCode& errorCode,
    std::vector<std::string>& errorIds,
    int& errorStart,
    int& errorEnd,
    std::vector<std::pair<int,int> >& comments,
    const KSeExpr::Expression* expr,
    const char* str,
    bool wantVec)
{
    std::lock_guard<std::mutex> locker(mutex);

    // glue around crippled C interface - ugh!
    Expr = expr;
    ParseStr = str;
    SeExprLexerResetState(comments);
    SeExpr_buffer_state* buffer = SeExpr_scan_string(str);
    ParseResult = 0;
    int resultCode = SeExprparse();
    SeExpr_delete_buffer(buffer);

    if (resultCode == 0) {
        // success
        errorCode = ErrorCode::None;
        errorIds = {};
	    parseTree = ParseResult;
    }
    else {
        // failure
        errorCode = ParseErrorCode;
        errorIds = { ParseErrorId };
        errorStart=SeExprlloc.first_column;
        errorEnd=SeExprlloc.last_column;
        parseTree = nullptr;

        // gather list of nodes with no parent
        std::vector<KSeExpr::ExprNode*> delnodes;
        std::vector<KSeExpr::ExprNode*>::iterator iter;
        for (iter = ParseNodes.begin(); iter != ParseNodes.end(); iter++) {
            if (!(*iter)->parent()) {
                delnodes.push_back(*iter);
            }
        }

        // now delete them (they will delete their own children)
        for (iter = delnodes.begin(); iter != delnodes.end(); iter++) {
            delete *iter;
        }
    }
    ParseNodes.clear();

    return parseTree != nullptr;
}
}
