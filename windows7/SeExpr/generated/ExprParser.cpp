/* A Bison parser, made by GNU Bison 3.5.4.  */

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

/* All symbols defined below should begin with SeExpr2 or SeExprYY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with SeExprYY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define SeExprYYBISON 1

/* Bison version.  */
#define SeExprYYBISON_VERSION "3.5.4"

/* Skeleton name.  */
#define SeExprYYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define SeExprYYPURE 0

/* Push parsers.  */
#define SeExprYYPUSH 0

/* Pull parsers.  */
#define SeExprYYPULL 1


/* Substitute the variable and function names.  */
#define SeExpr2parse         SeExpr2parse
#define SeExpr2lex           SeExpr2lex
#define SeExpr2error         SeExpr2error
#define SeExpr2debug         SeExpr2debug
#define SeExpr2nerrs         SeExpr2nerrs
#define SeExpr2lval          SeExpr2lval
#define SeExpr2char          SeExpr2char
#define SeExpr2lloc          SeExpr2lloc

/* First part of user prologue.  */
#line 18 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"

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
#include "Mutex.h"

/******************
 lexer declarations
 ******************/

// declarations of functions and data in ExprParser.l
int SeExpr2lex();
int SeExpr2pos();
extern int SeExpr2_start;
extern char* SeExpr2text;
struct SeExpr2_buffer_state;
SeExpr2_buffer_state* SeExpr2_scan_string(const char *str);
void SeExpr2_delete_buffer(SeExpr2_buffer_state*);

/*******************
 parser declarations
 *******************/

// forward declaration
static void SeExpr2error(const char* msg);

// local data
static const char* ParseStr;    // string being parsed
static std::string ParseError;  // error (set from SeExpr2error)
static SeExpr2::ExprNode* ParseResult; // must set result here since SeExpr2parse can't return it
static const SeExpr2::Expression* Expr;// used for parenting created SeExprOp's

/* The list of nodes being built is remembered locally here.
   Eventually (if there are no syntax errors) ownership of the nodes
   will belong solely to the parse tree and the parent expression.
   However, if there is a syntax error, we must loop through this list
   and free any nodes that were allocated before the error to avoid a
   memory leak. */
static std::vector<SeExpr2::ExprNode*> ParseNodes;
inline SeExpr2::ExprNode* Remember(SeExpr2::ExprNode* n,const int startPos,const int endPos)
    { ParseNodes.push_back(n); n->setPosition(startPos,endPos); return n; }
inline void Forget(SeExpr2::ExprNode* n)
    { ParseNodes.erase(std::find(ParseNodes.begin(), ParseNodes.end(), n)); }
/* These are handy node constructors for 0-3 arguments */
#define NODE(startPos,endPos,name) Remember(new SeExpr2::Expr##name(Expr),startPos,endPos)
#define NODE1(startPos,endPos,name,a) Remember(new SeExpr2::Expr##name(Expr,a),startPos,endPos)
#define NODE2(startPos,endPos,name,a,b) Remember(new SeExpr2::Expr##name(Expr,a,b),startPos,endPos)
#define NODE3(startPos,endPos,name,a,b,c) Remember(new SeExpr2::Expr##name(Expr,a,b,c),startPos,endPos)
#define NODE4(startPos,endPos,name,a,b,c,t) Remember(new SeExpr2::Expr##name(Expr,a,b,c,t),startPos,endPos)

#line 137 "y.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef SeExprYYERROR_VERBOSE
# undef SeExprYYERROR_VERBOSE
# define SeExprYYERROR_VERBOSE 1
#else
# define SeExprYYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef SeExprYY_SEEXPR2_Y_TAB_H_INCLUDED
# define SeExprYY_SEEXPR2_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef SeExprYYDEBUG
# define SeExprYYDEBUG 0
#endif
#if SeExprYYDEBUG
extern int SeExpr2debug;
#endif

/* Token type.  */
#ifndef SeExprYYTOKENTYPE
# define SeExprYYTOKENTYPE
  enum SeExpr2tokentype
  {
    IF = 258,
    ELSE = 259,
    EXTERN = 260,
    DEF = 261,
    FLOATPOINT = 262,
    STRING = 263,
    NAME = 264,
    VAR = 265,
    STR = 266,
    NUMBER = 267,
    LIFETIME_CONSTANT = 268,
    LIFETIME_UNIFORM = 269,
    LIFETIME_VARYING = 270,
    LIFETIME_ERROR = 271,
    AddEq = 272,
    SubEq = 273,
    MultEq = 274,
    DivEq = 275,
    ExpEq = 276,
    ModEq = 277,
    ARROW = 278,
    OR = 279,
    AND = 280,
    EQ = 281,
    NE = 282,
    SEEXPR_LE = 283,
    SEEXPR_GE = 284,
    UNARY = 285
  };
#endif

/* Value type.  */
#if ! defined SeExprYYSTYPE && ! defined SeExprYYSTYPE_IS_DECLARED
union SeExprYYSTYPE
{
#line 77 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"

    SeExpr2::ExprNode* n; /* a node is returned for all non-terminals to
		      build the parse tree from the leaves up. */
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: the string
		      is allocated with strdup() in the lexer and must
		      be freed with free() */
    struct {
        SeExpr2::ExprType::Type     type;
        int                  dim;
        SeExpr2::ExprType::Lifetime lifetime;
    } t;  // return value for types
    SeExpr2::ExprType::Lifetime l; // return value for lifetime qualifiers

#line 235 "y.tab.c"

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


extern SeExprYYSTYPE SeExpr2lval;
extern SeExprYYLTYPE SeExpr2lloc;
int SeExpr2parse (void);

#endif /* !SeExprYY_SEEXPR2_Y_TAB_H_INCLUDED  */



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
typedef __INT_LEAST8_TYPE__ SeExpr2type_int8;
#elif defined SeExprYY_STDINT_H
typedef int_least8_t SeExpr2type_int8;
#else
typedef signed char SeExpr2type_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ SeExpr2type_int16;
#elif defined SeExprYY_STDINT_H
typedef int_least16_t SeExpr2type_int16;
#else
typedef short SeExpr2type_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ SeExpr2type_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined SeExprYY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t SeExpr2type_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char SeExpr2type_uint8;
#else
typedef short SeExpr2type_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ SeExpr2type_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined SeExprYY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t SeExpr2type_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short SeExpr2type_uint16;
#else
typedef int SeExpr2type_uint16;
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
typedef SeExpr2type_uint8 SeExpr2_state_t;

/* State numbers in computations.  */
typedef int SeExpr2_state_fast_t;

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
/* Suppress an incorrect diagnostic about SeExpr2lval being uninitialized.  */
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

#if ! defined SeExpr2overflow || SeExprYYERROR_VERBOSE

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
#endif /* ! defined SeExpr2overflow || SeExprYYERROR_VERBOSE */


#if (! defined SeExpr2overflow \
     && (! defined __cplusplus \
         || (defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL \
             && defined SeExprYYSTYPE_IS_TRIVIAL && SeExprYYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union SeExpr2alloc
{
  SeExpr2_state_t SeExpr2ss_alloc;
  SeExprYYSTYPE SeExpr2vs_alloc;
  SeExprYYLTYPE SeExpr2ls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define SeExprYYSTACK_GAP_MAXIMUM (SeExprYYSIZEOF (union SeExpr2alloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define SeExprYYSTACK_BYTES(N) \
     ((N) * (SeExprYYSIZEOF (SeExpr2_state_t) + SeExprYYSIZEOF (SeExprYYSTYPE) \
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
        SeExprYYPTRDIFF_T SeExpr2newbytes;                                         \
        SeExprYYCOPY (&SeExpr2ptr->Stack_alloc, Stack, SeExpr2size);                    \
        Stack = &SeExpr2ptr->Stack_alloc;                                    \
        SeExpr2newbytes = SeExpr2stacksize * SeExprYYSIZEOF (*Stack) + SeExprYYSTACK_GAP_MAXIMUM; \
        SeExpr2ptr += SeExpr2newbytes / SeExprYYSIZEOF (*SeExpr2ptr);                        \
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
          SeExprYYPTRDIFF_T SeExpr2i;                      \
          for (SeExpr2i = 0; SeExpr2i < (Count); SeExpr2i++)   \
            (Dst)[SeExpr2i] = (Src)[SeExpr2i];            \
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

#define SeExprYYUNDEFTOK  2
#define SeExprYYMAXUTOK   285


/* SeExprYYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by SeExpr2lex, with out-of-bounds checking.  */
#define SeExprYYTRANSLATE(SeExprYYX)                                                \
  (0 <= (SeExprYYX) && (SeExprYYX) <= SeExprYYMAXUTOK ? SeExpr2translate[SeExprYYX] : SeExprYYUNDEFTOK)

/* SeExprYYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by SeExpr2lex.  */
static const SeExpr2type_int8 SeExpr2translate[] =
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
static const SeExpr2type_int16 SeExpr2rline[] =
{
       0,   132,   132,   134,   139,   140,   146,   154,   162,   172,
     173,   174,   175,   176,   180,   183,   188,   194,   195,   199,
     203,   210,   211,   215,   220,   229,   230,   235,   236,   240,
     241,   245,   246,   247,   250,   253,   256,   259,   262,   265,
     266,   269,   272,   275,   278,   281,   287,   292,   293,   294,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   325,   331,   332,   333,   334,   338,   339,
     345,   346,   351,   352,   356
};
#endif

#if SeExprYYDEBUG || SeExprYYERROR_VERBOSE || 0
/* SeExprYYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at SeExprYYNTOKENS, nonterminals.  */
static const char *const SeExpr2tname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "EXTERN", "DEF",
  "FLOATPOINT", "STRING", "NAME", "VAR", "STR", "NUMBER",
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
#endif

# ifdef SeExprYYPRINT
/* SeExprYYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const SeExpr2type_int16 SeExpr2toknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    40,    41,   278,    58,    63,   279,   280,
     281,   282,    60,    62,   283,   284,    43,    45,    42,    47,
      37,   285,    33,   126,    94,    91,   123,   125,    93,    44,
      61,    59
};
# endif

#define SeExprYYPACT_NINF (-76)

#define SeExpr2pact_value_is_default(Yyn) \
  ((Yyn) == SeExprYYPACT_NINF)

#define SeExprYYTABLE_NINF (-1)

#define SeExpr2table_value_is_error(Yyn) \
  0

  /* SeExprYYPACT[STATE-NUM] -- Index in SeExprYYTABLE of the portion describing
     STATE-NUM.  */
static const SeExpr2type_int16 SeExpr2pact[] =
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
static const SeExpr2type_int8 SeExpr2defact[] =
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
static const SeExpr2type_int16 SeExpr2pgoto[] =
{
     -76,   -76,   -76,   179,   -14,    -2,   -76,   -76,    66,   -76,
     -13,    14,   -75,   -15,    19,   -76,    -4,   -76,    51,   -76,
      65
};

  /* SeExprYYDEFGOTO[NTERM-NUM].  */
static const SeExpr2type_int16 SeExpr2defgoto[] =
{
      -1,    14,    15,    16,    80,   126,   153,   154,   127,   128,
      17,   164,    18,    19,    20,   179,    21,    51,    92,    93,
      94
};

  /* SeExprYYTABLE[SeExprYYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If SeExprYYTABLE_NINF, syntax error.  */
static const SeExpr2type_uint8 SeExpr2table[] =
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

static const SeExpr2type_int16 SeExpr2check[] =
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
static const SeExpr2type_int8 SeExpr2stos[] =
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
static const SeExpr2type_int8 SeExpr2r1[] =
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
static const SeExpr2type_int8 SeExpr2r2[] =
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


#define SeExpr2errok         (SeExpr2errstatus = 0)
#define SeExpr2clearin       (SeExpr2char = SeExprYYEMPTY)
#define SeExprYYEMPTY         (-2)
#define SeExprYYEOF           0

#define SeExprYYACCEPT        goto SeExpr2acceptlab
#define SeExprYYABORT         goto SeExpr2abortlab
#define SeExprYYERROR         goto SeExpr2errorlab


#define SeExprYYRECOVERING()  (!!SeExpr2errstatus)

#define SeExprYYBACKUP(Token, Value)                                    \
  do                                                              \
    if (SeExpr2char == SeExprYYEMPTY)                                        \
      {                                                           \
        SeExpr2char = (Token);                                         \
        SeExpr2lval = (Value);                                         \
        SeExprYYPOPSTACK (SeExpr2len);                                       \
        SeExpr2state = *SeExpr2ssp;                                         \
        goto SeExpr2backup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        SeExpr2error (SeExprYY_("syntax error: cannot back up")); \
        SeExprYYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define SeExprYYTERROR        1
#define SeExprYYERRCODE       256


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
  if (SeExpr2debug)                                  \
    SeExprYYFPRINTF Args;                             \
} while (0)


/* SeExprYY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef SeExprYY_LOCATION_PRINT
# if defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL

/* Print *SeExprYYLOCP on SeExprYYO.  Private, do not rely on its existence. */

SeExprYY_ATTRIBUTE_UNUSED
static int
SeExpr2_location_print_ (FILE *SeExpr2o, SeExprYYLTYPE const * const SeExpr2locp)
{
  int res = 0;
  int end_col = 0 != SeExpr2locp->last_column ? SeExpr2locp->last_column - 1 : 0;
  if (0 <= SeExpr2locp->first_line)
    {
      res += SeExprYYFPRINTF (SeExpr2o, "%d", SeExpr2locp->first_line);
      if (0 <= SeExpr2locp->first_column)
        res += SeExprYYFPRINTF (SeExpr2o, ".%d", SeExpr2locp->first_column);
    }
  if (0 <= SeExpr2locp->last_line)
    {
      if (SeExpr2locp->first_line < SeExpr2locp->last_line)
        {
          res += SeExprYYFPRINTF (SeExpr2o, "-%d", SeExpr2locp->last_line);
          if (0 <= end_col)
            res += SeExprYYFPRINTF (SeExpr2o, ".%d", end_col);
        }
      else if (0 <= end_col && SeExpr2locp->first_column < end_col)
        res += SeExprYYFPRINTF (SeExpr2o, "-%d", end_col);
    }
  return res;
 }

#  define SeExprYY_LOCATION_PRINT(File, Loc)          \
  SeExpr2_location_print_ (File, &(Loc))

# else
#  define SeExprYY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define SeExprYY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (SeExpr2debug)                                                            \
    {                                                                     \
      SeExprYYFPRINTF (stderr, "%s ", Title);                                   \
      SeExpr2_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      SeExprYYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on SeExprYYO.  |
`-----------------------------------*/

static void
SeExpr2_symbol_value_print (FILE *SeExpr2o, int SeExpr2type, SeExprYYSTYPE const * const SeExpr2valuep, SeExprYYLTYPE const * const SeExpr2locationp)
{
  FILE *SeExpr2output = SeExpr2o;
  SeExprYYUSE (SeExpr2output);
  SeExprYYUSE (SeExpr2locationp);
  if (!SeExpr2valuep)
    return;
# ifdef SeExprYYPRINT
  if (SeExpr2type < SeExprYYNTOKENS)
    SeExprYYPRINT (SeExpr2o, SeExpr2toknum[SeExpr2type], *SeExpr2valuep);
# endif
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  SeExprYYUSE (SeExpr2type);
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on SeExprYYO.  |
`---------------------------*/

static void
SeExpr2_symbol_print (FILE *SeExpr2o, int SeExpr2type, SeExprYYSTYPE const * const SeExpr2valuep, SeExprYYLTYPE const * const SeExpr2locationp)
{
  SeExprYYFPRINTF (SeExpr2o, "%s %s (",
             SeExpr2type < SeExprYYNTOKENS ? "token" : "nterm", SeExpr2tname[SeExpr2type]);

  SeExprYY_LOCATION_PRINT (SeExpr2o, *SeExpr2locationp);
  SeExprYYFPRINTF (SeExpr2o, ": ");
  SeExpr2_symbol_value_print (SeExpr2o, SeExpr2type, SeExpr2valuep, SeExpr2locationp);
  SeExprYYFPRINTF (SeExpr2o, ")");
}

/*------------------------------------------------------------------.
| SeExpr2_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
SeExpr2_stack_print (SeExpr2_state_t *SeExpr2bottom, SeExpr2_state_t *SeExpr2top)
{
  SeExprYYFPRINTF (stderr, "Stack now");
  for (; SeExpr2bottom <= SeExpr2top; SeExpr2bottom++)
    {
      int SeExpr2bot = *SeExpr2bottom;
      SeExprYYFPRINTF (stderr, " %d", SeExpr2bot);
    }
  SeExprYYFPRINTF (stderr, "\n");
}

# define SeExprYY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (SeExpr2debug)                                                  \
    SeExpr2_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the SeExprYYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
SeExpr2_reduce_print (SeExpr2_state_t *SeExpr2ssp, SeExprYYSTYPE *SeExpr2vsp, SeExprYYLTYPE *SeExpr2lsp, int SeExpr2rule)
{
  int SeExpr2lno = SeExpr2rline[SeExpr2rule];
  int SeExpr2nrhs = SeExpr2r2[SeExpr2rule];
  int SeExpr2i;
  SeExprYYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             SeExpr2rule - 1, SeExpr2lno);
  /* The symbols being reduced.  */
  for (SeExpr2i = 0; SeExpr2i < SeExpr2nrhs; SeExpr2i++)
    {
      SeExprYYFPRINTF (stderr, "   $%d = ", SeExpr2i + 1);
      SeExpr2_symbol_print (stderr,
                       SeExpr2stos[+SeExpr2ssp[SeExpr2i + 1 - SeExpr2nrhs]],
                       &SeExpr2vsp[(SeExpr2i + 1) - (SeExpr2nrhs)]
                       , &(SeExpr2lsp[(SeExpr2i + 1) - (SeExpr2nrhs)])                       );
      SeExprYYFPRINTF (stderr, "\n");
    }
}

# define SeExprYY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (SeExpr2debug)                          \
    SeExpr2_reduce_print (SeExpr2ssp, SeExpr2vsp, SeExpr2lsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int SeExpr2debug;
#else /* !SeExprYYDEBUG */
# define SeExprYYDPRINTF(Args)
# define SeExprYY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if SeExprYYERROR_VERBOSE

# ifndef SeExpr2strlen
#  if defined __GLIBC__ && defined _STRING_H
#   define SeExpr2strlen(S) (SeExprYY_CAST (SeExprYYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of SeExprYYSTR.  */
static SeExprYYPTRDIFF_T
SeExpr2strlen (const char *SeExpr2str)
{
  SeExprYYPTRDIFF_T SeExpr2len;
  for (SeExpr2len = 0; SeExpr2str[SeExpr2len]; SeExpr2len++)
    continue;
  return SeExpr2len;
}
#  endif
# endif

# ifndef SeExpr2stpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define SeExpr2stpcpy stpcpy
#  else
/* Copy SeExprYYSRC to SeExprYYDEST, returning the address of the terminating '\0' in
   SeExprYYDEST.  */
static char *
SeExpr2stpcpy (char *SeExpr2dest, const char *SeExpr2src)
{
  char *SeExpr2d = SeExpr2dest;
  const char *SeExpr2s = SeExpr2src;

  while ((*SeExpr2d++ = *SeExpr2s++) != '\0')
    continue;

  return SeExpr2d - 1;
}
#  endif
# endif

# ifndef SeExpr2tnamerr
/* Copy to SeExprYYRES the contents of SeExprYYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for SeExpr2error.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  SeExprYYSTR is taken from SeExpr2tname.  If SeExprYYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static SeExprYYPTRDIFF_T
SeExpr2tnamerr (char *SeExpr2res, const char *SeExpr2str)
{
  if (*SeExpr2str == '"')
    {
      SeExprYYPTRDIFF_T SeExpr2n = 0;
      char const *SeExpr2p = SeExpr2str;

      for (;;)
        switch (*++SeExpr2p)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++SeExpr2p != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (SeExpr2res)
              SeExpr2res[SeExpr2n] = *SeExpr2p;
            SeExpr2n++;
            break;

          case '"':
            if (SeExpr2res)
              SeExpr2res[SeExpr2n] = '\0';
            return SeExpr2n;
          }
    do_not_strip_quotes: ;
    }

  if (SeExpr2res)
    return SeExpr2stpcpy (SeExpr2res, SeExpr2str) - SeExpr2res;
  else
    return SeExpr2strlen (SeExpr2str);
}
# endif

/* Copy into *SeExprYYMSG, which is of size *SeExprYYMSG_ALLOC, an error message
   about the unexpected token SeExprYYTOKEN for the state stack whose top is
   SeExprYYSSP.

   Return 0 if *SeExprYYMSG was successfully written.  Return 1 if *SeExprYYMSG is
   not large enough to hold the message.  In that case, also set
   *SeExprYYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
SeExpr2syntax_error (SeExprYYPTRDIFF_T *SeExpr2msg_alloc, char **SeExpr2msg,
                SeExpr2_state_t *SeExpr2ssp, int SeExpr2token)
{
  enum { SeExprYYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *SeExpr2format = SeExprYY_NULLPTR;
  /* Arguments of SeExpr2format: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *SeExpr2arg[SeExprYYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of SeExprYYARG. */
  int SeExpr2count = 0;
  /* Cumulated lengths of SeExprYYARG.  */
  SeExprYYPTRDIFF_T SeExpr2size = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in SeExpr2char) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated SeExpr2char.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (SeExpr2token != SeExprYYEMPTY)
    {
      int SeExpr2n = SeExpr2pact[+*SeExpr2ssp];
      SeExprYYPTRDIFF_T SeExpr2size0 = SeExpr2tnamerr (SeExprYY_NULLPTR, SeExpr2tname[SeExpr2token]);
      SeExpr2size = SeExpr2size0;
      SeExpr2arg[SeExpr2count++] = SeExpr2tname[SeExpr2token];
      if (!SeExpr2pact_value_is_default (SeExpr2n))
        {
          /* Start SeExprYYX at -SeExprYYN if negative to avoid negative indexes in
             SeExprYYCHECK.  In other words, skip the first -SeExprYYN actions for
             this state because they are default actions.  */
          int SeExpr2xbegin = SeExpr2n < 0 ? -SeExpr2n : 0;
          /* Stay within bounds of both SeExpr2check and SeExpr2tname.  */
          int SeExpr2checklim = SeExprYYLAST - SeExpr2n + 1;
          int SeExpr2xend = SeExpr2checklim < SeExprYYNTOKENS ? SeExpr2checklim : SeExprYYNTOKENS;
          int SeExpr2x;

          for (SeExpr2x = SeExpr2xbegin; SeExpr2x < SeExpr2xend; ++SeExpr2x)
            if (SeExpr2check[SeExpr2x + SeExpr2n] == SeExpr2x && SeExpr2x != SeExprYYTERROR
                && !SeExpr2table_value_is_error (SeExpr2table[SeExpr2x + SeExpr2n]))
              {
                if (SeExpr2count == SeExprYYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    SeExpr2count = 1;
                    SeExpr2size = SeExpr2size0;
                    break;
                  }
                SeExpr2arg[SeExpr2count++] = SeExpr2tname[SeExpr2x];
                {
                  SeExprYYPTRDIFF_T SeExpr2size1
                    = SeExpr2size + SeExpr2tnamerr (SeExprYY_NULLPTR, SeExpr2tname[SeExpr2x]);
                  if (SeExpr2size <= SeExpr2size1 && SeExpr2size1 <= SeExprYYSTACK_ALLOC_MAXIMUM)
                    SeExpr2size = SeExpr2size1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (SeExpr2count)
    {
# define SeExprYYCASE_(N, S)                      \
      case N:                               \
        SeExpr2format = S;                       \
      break
    default: /* Avoid compiler warnings. */
      SeExprYYCASE_(0, SeExprYY_("syntax error"));
      SeExprYYCASE_(1, SeExprYY_("syntax error, unexpected %s"));
      SeExprYYCASE_(2, SeExprYY_("syntax error, unexpected %s, expecting %s"));
      SeExprYYCASE_(3, SeExprYY_("syntax error, unexpected %s, expecting %s or %s"));
      SeExprYYCASE_(4, SeExprYY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      SeExprYYCASE_(5, SeExprYY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef SeExprYYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    SeExprYYPTRDIFF_T SeExpr2size1 = SeExpr2size + (SeExpr2strlen (SeExpr2format) - 2 * SeExpr2count) + 1;
    if (SeExpr2size <= SeExpr2size1 && SeExpr2size1 <= SeExprYYSTACK_ALLOC_MAXIMUM)
      SeExpr2size = SeExpr2size1;
    else
      return 2;
  }

  if (*SeExpr2msg_alloc < SeExpr2size)
    {
      *SeExpr2msg_alloc = 2 * SeExpr2size;
      if (! (SeExpr2size <= *SeExpr2msg_alloc
             && *SeExpr2msg_alloc <= SeExprYYSTACK_ALLOC_MAXIMUM))
        *SeExpr2msg_alloc = SeExprYYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *SeExpr2p = *SeExpr2msg;
    int SeExpr2i = 0;
    while ((*SeExpr2p = *SeExpr2format) != '\0')
      if (*SeExpr2p == '%' && SeExpr2format[1] == 's' && SeExpr2i < SeExpr2count)
        {
          SeExpr2p += SeExpr2tnamerr (SeExpr2p, SeExpr2arg[SeExpr2i++]);
          SeExpr2format += 2;
        }
      else
        {
          ++SeExpr2p;
          ++SeExpr2format;
        }
  }
  return 0;
}
#endif /* SeExprYYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
SeExpr2destruct (const char *SeExpr2msg, int SeExpr2type, SeExprYYSTYPE *SeExpr2valuep, SeExprYYLTYPE *SeExpr2locationp)
{
  SeExprYYUSE (SeExpr2valuep);
  SeExprYYUSE (SeExpr2locationp);
  if (!SeExpr2msg)
    SeExpr2msg = "Deleting";
  SeExprYY_SYMBOL_PRINT (SeExpr2msg, SeExpr2type, SeExpr2valuep, SeExpr2locationp);

  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  SeExprYYUSE (SeExpr2type);
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int SeExpr2char;

/* The semantic value of the lookahead symbol.  */
SeExprYYSTYPE SeExpr2lval;
/* Location data for the lookahead symbol.  */
SeExprYYLTYPE SeExpr2lloc
# if defined SeExprYYLTYPE_IS_TRIVIAL && SeExprYYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int SeExpr2nerrs;


/*----------.
| SeExpr2parse.  |
`----------*/

int
SeExpr2parse (void)
{
    SeExpr2_state_fast_t SeExpr2state;
    /* Number of tokens to shift before error messages enabled.  */
    int SeExpr2errstatus;

    /* The stacks and their tools:
       'SeExpr2ss': related to states.
       'SeExpr2vs': related to semantic values.
       'SeExpr2ls': related to locations.

       Refer to the stacks through separate pointers, to allow SeExpr2overflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    SeExpr2_state_t SeExpr2ssa[SeExprYYINITDEPTH];
    SeExpr2_state_t *SeExpr2ss;
    SeExpr2_state_t *SeExpr2ssp;

    /* The semantic value stack.  */
    SeExprYYSTYPE SeExpr2vsa[SeExprYYINITDEPTH];
    SeExprYYSTYPE *SeExpr2vs;
    SeExprYYSTYPE *SeExpr2vsp;

    /* The location stack.  */
    SeExprYYLTYPE SeExpr2lsa[SeExprYYINITDEPTH];
    SeExprYYLTYPE *SeExpr2ls;
    SeExprYYLTYPE *SeExpr2lsp;

    /* The locations where the error started and ended.  */
    SeExprYYLTYPE SeExpr2error_range[3];

    SeExprYYPTRDIFF_T SeExpr2stacksize;

  int SeExpr2n;
  int SeExpr2result;
  /* Lookahead token as an internal (translated) token number.  */
  int SeExpr2token = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  SeExprYYSTYPE SeExpr2val;
  SeExprYYLTYPE SeExpr2loc;

#if SeExprYYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char SeExpr2msgbuf[128];
  char *SeExpr2msg = SeExpr2msgbuf;
  SeExprYYPTRDIFF_T SeExpr2msg_alloc = sizeof SeExpr2msgbuf;
#endif

#define SeExprYYPOPSTACK(N)   (SeExpr2vsp -= (N), SeExpr2ssp -= (N), SeExpr2lsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int SeExpr2len = 0;

  SeExpr2ssp = SeExpr2ss = SeExpr2ssa;
  SeExpr2vsp = SeExpr2vs = SeExpr2vsa;
  SeExpr2lsp = SeExpr2ls = SeExpr2lsa;
  SeExpr2stacksize = SeExprYYINITDEPTH;

  SeExprYYDPRINTF ((stderr, "Starting parse\n"));

  SeExpr2state = 0;
  SeExpr2errstatus = 0;
  SeExpr2nerrs = 0;
  SeExpr2char = SeExprYYEMPTY; /* Cause a token to be read.  */
  SeExpr2lsp[0] = SeExpr2lloc;
  goto SeExpr2setstate;


/*------------------------------------------------------------.
| SeExpr2newstate -- push a new state, which is found in SeExpr2state.  |
`------------------------------------------------------------*/
SeExpr2newstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  SeExpr2ssp++;


/*--------------------------------------------------------------------.
| SeExpr2setstate -- set current state (the top of the stack) to SeExpr2state.  |
`--------------------------------------------------------------------*/
SeExpr2setstate:
  SeExprYYDPRINTF ((stderr, "Entering state %d\n", SeExpr2state));
  SeExprYY_ASSERT (0 <= SeExpr2state && SeExpr2state < SeExprYYNSTATES);
  SeExprYY_IGNORE_USELESS_CAST_BEGIN
  *SeExpr2ssp = SeExprYY_CAST (SeExpr2_state_t, SeExpr2state);
  SeExprYY_IGNORE_USELESS_CAST_END

  if (SeExpr2ss + SeExpr2stacksize - 1 <= SeExpr2ssp)
#if !defined SeExpr2overflow && !defined SeExprYYSTACK_RELOCATE
    goto SeExpr2exhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      SeExprYYPTRDIFF_T SeExpr2size = SeExpr2ssp - SeExpr2ss + 1;

# if defined SeExpr2overflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        SeExpr2_state_t *SeExpr2ss1 = SeExpr2ss;
        SeExprYYSTYPE *SeExpr2vs1 = SeExpr2vs;
        SeExprYYLTYPE *SeExpr2ls1 = SeExpr2ls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if SeExpr2overflow is a macro.  */
        SeExpr2overflow (SeExprYY_("memory exhausted"),
                    &SeExpr2ss1, SeExpr2size * SeExprYYSIZEOF (*SeExpr2ssp),
                    &SeExpr2vs1, SeExpr2size * SeExprYYSIZEOF (*SeExpr2vsp),
                    &SeExpr2ls1, SeExpr2size * SeExprYYSIZEOF (*SeExpr2lsp),
                    &SeExpr2stacksize);
        SeExpr2ss = SeExpr2ss1;
        SeExpr2vs = SeExpr2vs1;
        SeExpr2ls = SeExpr2ls1;
      }
# else /* defined SeExprYYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (SeExprYYMAXDEPTH <= SeExpr2stacksize)
        goto SeExpr2exhaustedlab;
      SeExpr2stacksize *= 2;
      if (SeExprYYMAXDEPTH < SeExpr2stacksize)
        SeExpr2stacksize = SeExprYYMAXDEPTH;

      {
        SeExpr2_state_t *SeExpr2ss1 = SeExpr2ss;
        union SeExpr2alloc *SeExpr2ptr =
          SeExprYY_CAST (union SeExpr2alloc *,
                   SeExprYYSTACK_ALLOC (SeExprYY_CAST (SeExprYYSIZE_T, SeExprYYSTACK_BYTES (SeExpr2stacksize))));
        if (! SeExpr2ptr)
          goto SeExpr2exhaustedlab;
        SeExprYYSTACK_RELOCATE (SeExpr2ss_alloc, SeExpr2ss);
        SeExprYYSTACK_RELOCATE (SeExpr2vs_alloc, SeExpr2vs);
        SeExprYYSTACK_RELOCATE (SeExpr2ls_alloc, SeExpr2ls);
# undef SeExprYYSTACK_RELOCATE
        if (SeExpr2ss1 != SeExpr2ssa)
          SeExprYYSTACK_FREE (SeExpr2ss1);
      }
# endif

      SeExpr2ssp = SeExpr2ss + SeExpr2size - 1;
      SeExpr2vsp = SeExpr2vs + SeExpr2size - 1;
      SeExpr2lsp = SeExpr2ls + SeExpr2size - 1;

      SeExprYY_IGNORE_USELESS_CAST_BEGIN
      SeExprYYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  SeExprYY_CAST (long, SeExpr2stacksize)));
      SeExprYY_IGNORE_USELESS_CAST_END

      if (SeExpr2ss + SeExpr2stacksize - 1 <= SeExpr2ssp)
        SeExprYYABORT;
    }
#endif /* !defined SeExpr2overflow && !defined SeExprYYSTACK_RELOCATE */

  if (SeExpr2state == SeExprYYFINAL)
    SeExprYYACCEPT;

  goto SeExpr2backup;


/*-----------.
| SeExpr2backup.  |
`-----------*/
SeExpr2backup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  SeExpr2n = SeExpr2pact[SeExpr2state];
  if (SeExpr2pact_value_is_default (SeExpr2n))
    goto SeExpr2default;

  /* Not known => get a lookahead token if don't already have one.  */

  /* SeExprYYCHAR is either SeExprYYEMPTY or SeExprYYEOF or a valid lookahead symbol.  */
  if (SeExpr2char == SeExprYYEMPTY)
    {
      SeExprYYDPRINTF ((stderr, "Reading a token: "));
      SeExpr2char = SeExpr2lex ();
    }

  if (SeExpr2char <= SeExprYYEOF)
    {
      SeExpr2char = SeExpr2token = SeExprYYEOF;
      SeExprYYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      SeExpr2token = SeExprYYTRANSLATE (SeExpr2char);
      SeExprYY_SYMBOL_PRINT ("Next token is", SeExpr2token, &SeExpr2lval, &SeExpr2lloc);
    }

  /* If the proper action on seeing token SeExprYYTOKEN is to reduce or to
     detect an error, take that action.  */
  SeExpr2n += SeExpr2token;
  if (SeExpr2n < 0 || SeExprYYLAST < SeExpr2n || SeExpr2check[SeExpr2n] != SeExpr2token)
    goto SeExpr2default;
  SeExpr2n = SeExpr2table[SeExpr2n];
  if (SeExpr2n <= 0)
    {
      if (SeExpr2table_value_is_error (SeExpr2n))
        goto SeExpr2errlab;
      SeExpr2n = -SeExpr2n;
      goto SeExpr2reduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (SeExpr2errstatus)
    SeExpr2errstatus--;

  /* Shift the lookahead token.  */
  SeExprYY_SYMBOL_PRINT ("Shifting", SeExpr2token, &SeExpr2lval, &SeExpr2lloc);
  SeExpr2state = SeExpr2n;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++SeExpr2vsp = SeExpr2lval;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END
  *++SeExpr2lsp = SeExpr2lloc;

  /* Discard the shifted token.  */
  SeExpr2char = SeExprYYEMPTY;
  goto SeExpr2newstate;


/*-----------------------------------------------------------.
| SeExpr2default -- do the default action for the current state.  |
`-----------------------------------------------------------*/
SeExpr2default:
  SeExpr2n = SeExpr2defact[SeExpr2state];
  if (SeExpr2n == 0)
    goto SeExpr2errlab;
  goto SeExpr2reduce;


/*-----------------------------.
| SeExpr2reduce -- do a reduction.  |
`-----------------------------*/
SeExpr2reduce:
  /* SeExpr2n is the number of a rule to reduce with.  */
  SeExpr2len = SeExpr2r2[SeExpr2n];

  /* If SeExprYYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets SeExprYYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to SeExprYYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that SeExprYYVAL may be used uninitialized.  */
  SeExpr2val = SeExpr2vsp[1-SeExpr2len];

  /* Default location. */
  SeExprYYLLOC_DEFAULT (SeExpr2loc, (SeExpr2lsp - SeExpr2len), SeExpr2len);
  SeExpr2error_range[1] = SeExpr2loc;
  SeExprYY_REDUCE_PRINT (SeExpr2n);
  switch (SeExpr2n)
    {
  case 2:
#line 132 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { ParseResult = (SeExpr2vsp[-1].n); ParseResult->setPosition((SeExpr2loc).first_column, (SeExpr2loc).last_column);
                                  ParseResult->addChild((SeExpr2vsp[0].n)); }
#line 1748 "y.tab.c"
    break;

  case 3:
#line 134 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { ParseResult = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, ModuleNode);
                                  ParseResult->addChild((SeExpr2vsp[0].n)); }
#line 1755 "y.tab.c"
    break;

  case 4:
#line 139 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, ModuleNode); (SeExpr2val.n)->addChild((SeExpr2vsp[0].n)); }
#line 1761 "y.tab.c"
    break;

  case 5:
#line 141 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-1].n); (SeExpr2val.n)->setPosition((SeExpr2loc).first_column, (SeExpr2loc).last_column);
                                  (SeExpr2val.n)->addChild((SeExpr2vsp[0].n)); }
#line 1768 "y.tab.c"
    break;

  case 6:
#line 147 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[-4].t).type, (SeExpr2vsp[-4].t).dim, (SeExpr2vsp[-4].t).lifetime);
                                    SeExpr2::ExprPrototypeNode * prototype =
                                        (SeExpr2::ExprPrototypeNode*)NODE2((SeExpr2loc).first_column, (SeExpr2loc).last_column, PrototypeNode, (SeExpr2vsp[-3].s), type);
                                  prototype->addArgTypes((SeExpr2vsp[-1].n));
                                  Forget((SeExpr2vsp[-1].n));
                                  (SeExpr2val.n) = prototype;
                                  free((SeExpr2vsp[-3].s)); }
#line 1780 "y.tab.c"
    break;

  case 7:
#line 155 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[-7].t).type, (SeExpr2vsp[-7].t).dim, (SeExpr2vsp[-7].t).lifetime);
                                  SeExpr2::ExprPrototypeNode * prototype =
                                      (SeExpr2::ExprPrototypeNode*)NODE2((SeExpr2loc).first_column, (SeExpr2lsp[-3]).last_column, PrototypeNode, (SeExpr2vsp[-6].s), type);
                                  prototype->addArgs((SeExpr2vsp[-4].n));
                                  Forget((SeExpr2vsp[-4].n));
                                  (SeExpr2val.n) = NODE2((SeExpr2loc).first_column, (SeExpr2loc).last_column, LocalFunctionNode, prototype, (SeExpr2vsp[-1].n));
                                  free((SeExpr2vsp[-6].s)); }
#line 1792 "y.tab.c"
    break;

  case 8:
#line 163 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { SeExpr2::ExprPrototypeNode * prototype =
                                        (SeExpr2::ExprPrototypeNode*)NODE1((SeExpr2loc).first_column, (SeExpr2lsp[-3]).last_column, PrototypeNode, (SeExpr2vsp[-6].s));
                                  prototype->addArgs((SeExpr2vsp[-4].n));
                                  Forget((SeExpr2vsp[-4].n));
                                  (SeExpr2val.n) = NODE2((SeExpr2loc).first_column, (SeExpr2loc).last_column, LocalFunctionNode, prototype, (SeExpr2vsp[-1].n));
                                  free((SeExpr2vsp[-6].s)); }
#line 1803 "y.tab.c"
    break;

  case 9:
#line 172 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.l) = SeExpr2::ExprType::ltVARYING; }
#line 1809 "y.tab.c"
    break;

  case 10:
#line 173 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.l) = SeExpr2::ExprType::ltCONSTANT; }
#line 1815 "y.tab.c"
    break;

  case 11:
#line 174 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.l) = SeExpr2::ExprType::ltUNIFORM; }
#line 1821 "y.tab.c"
    break;

  case 12:
#line 175 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.l) = SeExpr2::ExprType::ltVARYING; }
#line 1827 "y.tab.c"
    break;

  case 13:
#line 176 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.l) = SeExpr2::ExprType::ltERROR; }
#line 1833 "y.tab.c"
    break;

  case 14:
#line 180 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                 {(SeExpr2val.t).type     = SeExpr2::ExprType::tFP;
                                  (SeExpr2val.t).dim      = 1;
                                  (SeExpr2val.t).lifetime = (SeExpr2vsp[0].l); }
#line 1841 "y.tab.c"
    break;

  case 15:
#line 184 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.t).type = ((SeExpr2vsp[-2].d) > 0 ? SeExpr2::ExprType::tFP : SeExpr2::ExprType::tERROR);
                                  //TODO: This causes an error but does not report it to user. Change this.
                                  (SeExpr2val.t).dim  = ((SeExpr2vsp[-2].d) > 0 ? (SeExpr2vsp[-2].d) : 0);
                                  (SeExpr2val.t).lifetime = (SeExpr2vsp[0].l); }
#line 1850 "y.tab.c"
    break;

  case 16:
#line 188 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.t).type = SeExpr2::ExprType::tSTRING;
                                  (SeExpr2val.t).dim  = 1;
                                  (SeExpr2val.t).lifetime = (SeExpr2vsp[0].l); }
#line 1858 "y.tab.c"
    break;

  case 17:
#line 194 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, Node); }
#line 1864 "y.tab.c"
    break;

  case 18:
#line 195 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 1870 "y.tab.c"
    break;

  case 19:
#line 199 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, Node);
                                  SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[0].t).type, (SeExpr2vsp[0].t).dim, (SeExpr2vsp[0].t).lifetime);
                                  SeExpr2::ExprNode* varNode = NODE2((SeExpr2loc).first_column, (SeExpr2loc).last_column, VarNode, "", type);
                                  (SeExpr2val.n)->addChild(varNode); }
#line 1879 "y.tab.c"
    break;

  case 20:
#line 203 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-2].n);
                                  SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[0].t).type, (SeExpr2vsp[0].t).dim, (SeExpr2vsp[0].t).lifetime);
                                  SeExpr2::ExprNode* varNode = NODE2((SeExpr2lsp[0]).first_column, (SeExpr2lsp[0]).last_column, VarNode, "", type);
                                  (SeExpr2val.n)->addChild(varNode); }
#line 1888 "y.tab.c"
    break;

  case 21:
#line 210 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, Node); }
#line 1894 "y.tab.c"
    break;

  case 22:
#line 211 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 1900 "y.tab.c"
    break;

  case 23:
#line 215 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                 { (SeExpr2val.n) = NODE((SeExpr2loc).first_column, (SeExpr2loc).last_column, Node);
                                  SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[-1].t).type, (SeExpr2vsp[-1].t).dim, (SeExpr2vsp[-1].t).lifetime);
                                  SeExpr2::ExprNode* varNode = NODE2((SeExpr2loc).first_column, (SeExpr2loc).last_column, VarNode, (SeExpr2vsp[0].s), type);
                                  (SeExpr2val.n)->addChild(varNode);
                                  free((SeExpr2vsp[0].s)); }
#line 1910 "y.tab.c"
    break;

  case 24:
#line 221 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-3].n);
                                  SeExpr2::ExprType type = SeExpr2::ExprType((SeExpr2vsp[-1].t).type, (SeExpr2vsp[-1].t).dim, (SeExpr2vsp[-1].t).lifetime);
                                  SeExpr2::ExprNode* varNode = NODE2((SeExpr2lsp[-1]).first_column, (SeExpr2lsp[0]).last_column, VarNode, (SeExpr2vsp[0].s), type);
                                  (SeExpr2val.n)->addChild(varNode);
                                  free((SeExpr2vsp[0].s)); }
#line 1920 "y.tab.c"
    break;

  case 25:
#line 229 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,BlockNode, (SeExpr2vsp[-1].n), (SeExpr2vsp[0].n)); }
#line 1926 "y.tab.c"
    break;

  case 26:
#line 230 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 1932 "y.tab.c"
    break;

  case 27:
#line 235 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node); /* create empty node */; }
#line 1938 "y.tab.c"
    break;

  case 28:
#line 236 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 1944 "y.tab.c"
    break;

  case 29:
#line 240 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node, (SeExpr2vsp[0].n)); /* create var list */}
#line 1950 "y.tab.c"
    break;

  case 30:
#line 241 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-1].n); (SeExpr2vsp[-1].n)->addChild((SeExpr2vsp[0].n)); /* add to list */}
#line 1956 "y.tab.c"
    break;

  case 31:
#line 245 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 1962 "y.tab.c"
    break;

  case 32:
#line 246 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), (SeExpr2vsp[-1].n)); free((SeExpr2vsp[-3].s)); }
#line 1968 "y.tab.c"
    break;

  case 33:
#line 247 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                   {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'+');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 1976 "y.tab.c"
    break;

  case 34:
#line 250 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                   {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'-');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 1984 "y.tab.c"
    break;

  case 35:
#line 253 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'*');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 1992 "y.tab.c"
    break;

  case 36:
#line 256 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                   {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'/');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2000 "y.tab.c"
    break;

  case 37:
#line 259 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                   {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'^');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2008 "y.tab.c"
    break;

  case 38:
#line 262 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                   {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'%');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2016 "y.tab.c"
    break;

  case 39:
#line 265 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), (SeExpr2vsp[-1].n)); free((SeExpr2vsp[-3].s)); }
#line 2022 "y.tab.c"
    break;

  case 40:
#line 266 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'+');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2030 "y.tab.c"
    break;

  case 41:
#line 269 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'-');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2038 "y.tab.c"
    break;

  case 42:
#line 272 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                     {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'*');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2046 "y.tab.c"
    break;

  case 43:
#line 275 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'/');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2054 "y.tab.c"
    break;

  case 44:
#line 278 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'^');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2062 "y.tab.c"
    break;

  case 45:
#line 281 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                    {SeExpr2::ExprNode* varNode=NODE1((SeExpr2lsp[-3]).first_column,(SeExpr2lsp[-3]).first_column,VarNode, (SeExpr2vsp[-3].s));
                               SeExpr2::ExprNode* opNode=NODE3((SeExpr2lsp[-1]).first_column,(SeExpr2lsp[-1]).first_column,BinaryOpNode,varNode,(SeExpr2vsp[-1].n),'%');
                                (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,AssignNode, (SeExpr2vsp[-3].s), opNode);free((SeExpr2vsp[-3].s));}
#line 2070 "y.tab.c"
    break;

  case 46:
#line 288 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,IfThenElseNode, (SeExpr2vsp[-5].n), (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n)); }
#line 2076 "y.tab.c"
    break;

  case 47:
#line 292 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node); /* create empty node */ }
#line 2082 "y.tab.c"
    break;

  case 48:
#line 293 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-1].n); }
#line 2088 "y.tab.c"
    break;

  case 49:
#line 294 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 2094 "y.tab.c"
    break;

  case 50:
#line 299 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-1].n); }
#line 2100 "y.tab.c"
    break;

  case 51:
#line 300 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { SeExpr2::ExprNode* newNode = NODE((SeExpr2loc).first_column,(SeExpr2loc).last_column,VecNode); newNode->addChildren((SeExpr2vsp[-1].n)); Forget((SeExpr2vsp[-1].n)); (SeExpr2val.n)=newNode;}
#line 2106 "y.tab.c"
    break;

  case 52:
#line 301 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,SubscriptNode, (SeExpr2vsp[-3].n), (SeExpr2vsp[-1].n)); }
#line 2112 "y.tab.c"
    break;

  case 53:
#line 302 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CondNode, (SeExpr2vsp[-4].n), (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n)); }
#line 2118 "y.tab.c"
    break;

  case 54:
#line 303 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '|'); }
#line 2124 "y.tab.c"
    break;

  case 55:
#line 304 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '&'); }
#line 2130 "y.tab.c"
    break;

  case 56:
#line 305 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareEqNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'='); }
#line 2136 "y.tab.c"
    break;

  case 57:
#line 306 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareEqNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'!'); }
#line 2142 "y.tab.c"
    break;

  case 58:
#line 307 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'<'); }
#line 2148 "y.tab.c"
    break;

  case 59:
#line 308 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'>'); }
#line 2154 "y.tab.c"
    break;

  case 60:
#line 309 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                        { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'l'); }
#line 2160 "y.tab.c"
    break;

  case 61:
#line 310 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                        { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,CompareNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n),'g'); }
#line 2166 "y.tab.c"
    break;

  case 62:
#line 311 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 2172 "y.tab.c"
    break;

  case 63:
#line 312 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,UnaryOpNode, (SeExpr2vsp[0].n), '-'); }
#line 2178 "y.tab.c"
    break;

  case 64:
#line 313 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,UnaryOpNode, (SeExpr2vsp[0].n), '!'); }
#line 2184 "y.tab.c"
    break;

  case 65:
#line 314 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE2((SeExpr2loc).first_column,(SeExpr2loc).last_column,UnaryOpNode, (SeExpr2vsp[0].n), '~'); }
#line 2190 "y.tab.c"
    break;

  case 66:
#line 315 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '+'); }
#line 2196 "y.tab.c"
    break;

  case 67:
#line 316 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '-'); }
#line 2202 "y.tab.c"
    break;

  case 68:
#line 317 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '*'); }
#line 2208 "y.tab.c"
    break;

  case 69:
#line 318 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '/'); }
#line 2214 "y.tab.c"
    break;

  case 70:
#line 319 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '%'); }
#line 2220 "y.tab.c"
    break;

  case 71:
#line 320 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE3((SeExpr2loc).first_column,(SeExpr2loc).last_column,BinaryOpNode, (SeExpr2vsp[-2].n), (SeExpr2vsp[0].n), '^'); }
#line 2226 "y.tab.c"
    break;

  case 72:
#line 321 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,FuncNode, (SeExpr2vsp[-3].s));
				  free((SeExpr2vsp[-3].s)); // free name string
				  // add args directly and discard arg list node
				  (SeExpr2val.n)->addChildren((SeExpr2vsp[-1].n)); Forget((SeExpr2vsp[-1].n)); }
#line 2235 "y.tab.c"
    break;

  case 73:
#line 326 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,FuncNode, (SeExpr2vsp[-3].s));
				  free((SeExpr2vsp[-3].s)); // free name string
				  (SeExpr2val.n)->addChild((SeExpr2vsp[-5].n));
				  // add args directly and discard arg list node
				  (SeExpr2val.n)->addChildren((SeExpr2vsp[-1].n)); Forget((SeExpr2vsp[-1].n)); }
#line 2245 "y.tab.c"
    break;

  case 74:
#line 331 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,VarNode, (SeExpr2vsp[0].s)); free((SeExpr2vsp[0].s)); /* free name string */ }
#line 2251 "y.tab.c"
    break;

  case 75:
#line 332 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,VarNode, (SeExpr2vsp[0].s)); free((SeExpr2vsp[0].s)); /* free name string */ }
#line 2257 "y.tab.c"
    break;

  case 76:
#line 333 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,NumNode, (SeExpr2vsp[0].d)); /*printf("line %d",@$.last_column);*/}
#line 2263 "y.tab.c"
    break;

  case 77:
#line 334 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,StrNode, (SeExpr2vsp[0].s)); free((SeExpr2vsp[0].s)); /* free string */}
#line 2269 "y.tab.c"
    break;

  case 78:
#line 338 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node,(SeExpr2vsp[0].n)); }
#line 2275 "y.tab.c"
    break;

  case 79:
#line 339 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-2].n);
                                  (SeExpr2vsp[-2].n)->addChild((SeExpr2vsp[0].n)); }
#line 2282 "y.tab.c"
    break;

  case 80:
#line 345 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node); /* create empty node */}
#line 2288 "y.tab.c"
    break;

  case 81:
#line 346 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 2294 "y.tab.c"
    break;

  case 82:
#line 351 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = NODE1((SeExpr2loc).first_column,(SeExpr2loc).last_column,Node, (SeExpr2vsp[0].n)); /* create arg list */}
#line 2300 "y.tab.c"
    break;

  case 83:
#line 352 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[-2].n); (SeExpr2vsp[-2].n)->addChild((SeExpr2vsp[0].n)); /* add to list */}
#line 2306 "y.tab.c"
    break;

  case 84:
#line 356 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"
                                { (SeExpr2val.n) = (SeExpr2vsp[0].n); }
#line 2312 "y.tab.c"
    break;


#line 2316 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter SeExpr2char, and that requires
     that SeExpr2token be updated with the new translation.  We take the
     approach of translating immediately before every use of SeExpr2token.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     SeExprYYABORT, SeExprYYACCEPT, or SeExprYYERROR immediately after altering SeExpr2char or
     if it invokes SeExprYYBACKUP.  In the case of SeExprYYABORT or SeExprYYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of SeExprYYERROR or SeExprYYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  SeExprYY_SYMBOL_PRINT ("-> $$ =", SeExpr2r1[SeExpr2n], &SeExpr2val, &SeExpr2loc);

  SeExprYYPOPSTACK (SeExpr2len);
  SeExpr2len = 0;
  SeExprYY_STACK_PRINT (SeExpr2ss, SeExpr2ssp);

  *++SeExpr2vsp = SeExpr2val;
  *++SeExpr2lsp = SeExpr2loc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int SeExpr2lhs = SeExpr2r1[SeExpr2n] - SeExprYYNTOKENS;
    const int SeExpr2i = SeExpr2pgoto[SeExpr2lhs] + *SeExpr2ssp;
    SeExpr2state = (0 <= SeExpr2i && SeExpr2i <= SeExprYYLAST && SeExpr2check[SeExpr2i] == *SeExpr2ssp
               ? SeExpr2table[SeExpr2i]
               : SeExpr2defgoto[SeExpr2lhs]);
  }

  goto SeExpr2newstate;


/*--------------------------------------.
| SeExpr2errlab -- here on detecting error.  |
`--------------------------------------*/
SeExpr2errlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  SeExpr2token = SeExpr2char == SeExprYYEMPTY ? SeExprYYEMPTY : SeExprYYTRANSLATE (SeExpr2char);

  /* If not already recovering from an error, report this error.  */
  if (!SeExpr2errstatus)
    {
      ++SeExpr2nerrs;
#if ! SeExprYYERROR_VERBOSE
      SeExpr2error (SeExprYY_("syntax error"));
#else
# define SeExprYYSYNTAX_ERROR SeExpr2syntax_error (&SeExpr2msg_alloc, &SeExpr2msg, \
                                        SeExpr2ssp, SeExpr2token)
      {
        char const *SeExpr2msgp = SeExprYY_("syntax error");
        int SeExpr2syntax_error_status;
        SeExpr2syntax_error_status = SeExprYYSYNTAX_ERROR;
        if (SeExpr2syntax_error_status == 0)
          SeExpr2msgp = SeExpr2msg;
        else if (SeExpr2syntax_error_status == 1)
          {
            if (SeExpr2msg != SeExpr2msgbuf)
              SeExprYYSTACK_FREE (SeExpr2msg);
            SeExpr2msg = SeExprYY_CAST (char *, SeExprYYSTACK_ALLOC (SeExprYY_CAST (SeExprYYSIZE_T, SeExpr2msg_alloc)));
            if (!SeExpr2msg)
              {
                SeExpr2msg = SeExpr2msgbuf;
                SeExpr2msg_alloc = sizeof SeExpr2msgbuf;
                SeExpr2syntax_error_status = 2;
              }
            else
              {
                SeExpr2syntax_error_status = SeExprYYSYNTAX_ERROR;
                SeExpr2msgp = SeExpr2msg;
              }
          }
        SeExpr2error (SeExpr2msgp);
        if (SeExpr2syntax_error_status == 2)
          goto SeExpr2exhaustedlab;
      }
# undef SeExprYYSYNTAX_ERROR
#endif
    }

  SeExpr2error_range[1] = SeExpr2lloc;

  if (SeExpr2errstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (SeExpr2char <= SeExprYYEOF)
        {
          /* Return failure if at end of input.  */
          if (SeExpr2char == SeExprYYEOF)
            SeExprYYABORT;
        }
      else
        {
          SeExpr2destruct ("Error: discarding",
                      SeExpr2token, &SeExpr2lval, &SeExpr2lloc);
          SeExpr2char = SeExprYYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto SeExpr2errlab1;


/*---------------------------------------------------.
| SeExpr2errorlab -- error raised explicitly by SeExprYYERROR.  |
`---------------------------------------------------*/
SeExpr2errorlab:
  /* Pacify compilers when the user code never invokes SeExprYYERROR and the
     label SeExpr2errorlab therefore never appears in user code.  */
  if (0)
    SeExprYYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this SeExprYYERROR.  */
  SeExprYYPOPSTACK (SeExpr2len);
  SeExpr2len = 0;
  SeExprYY_STACK_PRINT (SeExpr2ss, SeExpr2ssp);
  SeExpr2state = *SeExpr2ssp;
  goto SeExpr2errlab1;


/*-------------------------------------------------------------.
| SeExpr2errlab1 -- common code for both syntax error and SeExprYYERROR.  |
`-------------------------------------------------------------*/
SeExpr2errlab1:
  SeExpr2errstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      SeExpr2n = SeExpr2pact[SeExpr2state];
      if (!SeExpr2pact_value_is_default (SeExpr2n))
        {
          SeExpr2n += SeExprYYTERROR;
          if (0 <= SeExpr2n && SeExpr2n <= SeExprYYLAST && SeExpr2check[SeExpr2n] == SeExprYYTERROR)
            {
              SeExpr2n = SeExpr2table[SeExpr2n];
              if (0 < SeExpr2n)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (SeExpr2ssp == SeExpr2ss)
        SeExprYYABORT;

      SeExpr2error_range[1] = *SeExpr2lsp;
      SeExpr2destruct ("Error: popping",
                  SeExpr2stos[SeExpr2state], SeExpr2vsp, SeExpr2lsp);
      SeExprYYPOPSTACK (1);
      SeExpr2state = *SeExpr2ssp;
      SeExprYY_STACK_PRINT (SeExpr2ss, SeExpr2ssp);
    }

  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++SeExpr2vsp = SeExpr2lval;
  SeExprYY_IGNORE_MAYBE_UNINITIALIZED_END

  SeExpr2error_range[2] = SeExpr2lloc;
  /* Using SeExprYYLLOC is tempting, but would change the location of
     the lookahead.  SeExprYYLOC is available though.  */
  SeExprYYLLOC_DEFAULT (SeExpr2loc, SeExpr2error_range, 2);
  *++SeExpr2lsp = SeExpr2loc;

  /* Shift the error token.  */
  SeExprYY_SYMBOL_PRINT ("Shifting", SeExpr2stos[SeExpr2n], SeExpr2vsp, SeExpr2lsp);

  SeExpr2state = SeExpr2n;
  goto SeExpr2newstate;


/*-------------------------------------.
| SeExpr2acceptlab -- SeExprYYACCEPT comes here.  |
`-------------------------------------*/
SeExpr2acceptlab:
  SeExpr2result = 0;
  goto SeExpr2return;


/*-----------------------------------.
| SeExpr2abortlab -- SeExprYYABORT comes here.  |
`-----------------------------------*/
SeExpr2abortlab:
  SeExpr2result = 1;
  goto SeExpr2return;


#if !defined SeExpr2overflow || SeExprYYERROR_VERBOSE
/*-------------------------------------------------.
| SeExpr2exhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
SeExpr2exhaustedlab:
  SeExpr2error (SeExprYY_("memory exhausted"));
  SeExpr2result = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| SeExpr2return -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
SeExpr2return:
  if (SeExpr2char != SeExprYYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      SeExpr2token = SeExprYYTRANSLATE (SeExpr2char);
      SeExpr2destruct ("Cleanup: discarding lookahead",
                  SeExpr2token, &SeExpr2lval, &SeExpr2lloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this SeExprYYABORT or SeExprYYACCEPT.  */
  SeExprYYPOPSTACK (SeExpr2len);
  SeExprYY_STACK_PRINT (SeExpr2ss, SeExpr2ssp);
  while (SeExpr2ssp != SeExpr2ss)
    {
      SeExpr2destruct ("Cleanup: popping",
                  SeExpr2stos[+*SeExpr2ssp], SeExpr2vsp, SeExpr2lsp);
      SeExprYYPOPSTACK (1);
    }
#ifndef SeExpr2overflow
  if (SeExpr2ss != SeExpr2ssa)
    SeExprYYSTACK_FREE (SeExpr2ss);
#endif
#if SeExprYYERROR_VERBOSE
  if (SeExpr2msg != SeExpr2msgbuf)
    SeExprYYSTACK_FREE (SeExpr2msg);
#endif
  return SeExpr2result;
}
#line 359 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"


      /* SeExpr2error - Report an error.  This is called by the parser.
	 (Note: the "msg" param is useless as it is usually just "parse error".
	 so it's ignored.)
      */
static void SeExpr2error(const char* /*msg*/)
{
    // find start of line containing error
    int pos = SeExpr2pos(), lineno = 1, start = 0, end = strlen(ParseStr);
    bool multiline = 0;
    for (int i = start; i < pos; i++)
	if (ParseStr[i] == '\n') { start = i + 1; lineno++; multiline=1; }

    // find end of line containing error
    for (int i = end; i > pos; i--)
	if (ParseStr[i] == '\n') { end = i - 1; multiline=1; }

    ParseError = SeExpr2text[0] ? "Syntax error" : "Unexpected end of expression";
    if (multiline) {
	char buff[30];
	snprintf(buff, 30, " at line %d", lineno);
	ParseError += buff;
    }
    if (SeExpr2text[0]) {
	ParseError += " near '";
	ParseError += SeExpr2text;
    }
    ParseError += "':\n    ";

    int s = std::max(start, pos-30);
    int e = std::min(end, pos+30);

    if (s != start) ParseError += "...";
    ParseError += std::string(ParseStr, s, e-s+1);
    if (e != end) ParseError += "...";
}


/* CallParser - This is our entrypoint from the rest of the expr library.
   A string is passed in and a parse tree is returned.	If the tree is null,
   an error string is returned.  Any flags set during parsing are passed
   along.
 */

extern void SeExprLexerResetState(std::vector<std::pair<int,int> >& comments);

static SeExprInternal2::Mutex mutex;

namespace SeExpr2 {
bool ExprParse(SeExpr2::ExprNode*& parseTree,
    std::string& error, int& errorStart, int& errorEnd,
    std::vector<std::pair<int,int> >& comments,
    const SeExpr2::Expression* expr, const char* str, bool wantVec)
{
    SeExprInternal2::AutoMutex locker(mutex);

    // glue around crippled C interface - ugh!
    Expr = expr;
    ParseStr = str;
    SeExprLexerResetState(comments);
    SeExpr2_buffer_state* buffer = SeExpr2_scan_string(str);
    ParseResult = 0;
    int resultCode = SeExpr2parse();
    SeExpr2_delete_buffer(buffer);

    if (resultCode == 0) {
	// success
	error = "";
	parseTree = ParseResult;
    }
    else {
	// failure
	error = ParseError;
        errorStart=SeExpr2lloc.first_column;
        errorEnd=SeExpr2lloc.last_column;
	parseTree = 0;
	// gather list of nodes with no parent
	std::vector<SeExpr2::ExprNode*> delnodes;
	std::vector<SeExpr2::ExprNode*>::iterator iter;
	for (iter = ParseNodes.begin(); iter != ParseNodes.end(); iter++)
	    if (!(*iter)->parent()) { delnodes.push_back(*iter); }
	// now delete them (they will delete their own children)
	for (iter = delnodes.begin(); iter != delnodes.end(); iter++)
	    delete *iter;
    }
    ParseNodes.clear();

    return parseTree != 0;
}
}
