/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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
   especially those whose name start with ExprSpecYY_ or ExprSpec_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with ExprSpec or ExprSpecYY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define ExprSpecYYBISON 30706

/* Bison version string.  */
#define ExprSpecYYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define ExprSpecYYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define ExprSpecYYPURE 0

/* Push parsers.  */
#define ExprSpecYYPUSH 0

/* Pull parsers.  */
#define ExprSpecYYPULL 1


/* Substitute the variable and function names.  */
#define ExprSpecparse         ExprSpecparse
#define ExprSpeclex           ExprSpeclex
#define ExprSpecerror         ExprSpecerror
#define ExprSpecdebug         ExprSpecdebug
#define ExprSpecnerrs         ExprSpecnerrs
#define ExprSpeclval          ExprSpeclval
#define ExprSpecchar          ExprSpecchar
#define ExprSpeclloc          ExprSpeclloc

/* First part of user prologue.  */
#line 8 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"

#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <typeinfo>
#include <mutex>
#include "ExprSpecType.h"
#include "Editable.h"
#include "Debug.h"

/******************
 lexer declarations
 ******************/


#define SPEC_IS_NUMBER(x) \
    (dynamic_cast<ExprSpecScalarNode*>(x) != 0)
#define SPEC_IS_VECTOR(x) \
    (dynamic_cast<ExprSpecVectorNode*>(x) != 0)
#define SPEC_IS_STR(x) \
    (dynamic_cast<ExprSpecStringNode*>(x) != 0)

// declarations of functions and data in ExprParser.y
int ExprSpeclex();
int ExprSpecpos();
extern int ExprSpec_start;
extern char* ExprSpectext;
struct ExprSpec_buffer_state;
ExprSpec_buffer_state* ExprSpec_scan_string(const char *str);
void ExprSpec_delete_buffer(ExprSpec_buffer_state*);

//#####################################
// Keep track of mini parse tree nodes

// temporary to the parse... all pointers deleted at end of parse
static std::vector<ExprSpecNode*> specNodes;
/// Remember the spec node, so we can delete it later
static ExprSpecNode* remember(ExprSpecNode* node)
{specNodes.push_back(node);return node;}


/// list of strings duplicated by lexer to avoid error mem leak
static std::vector<char*> tokens;

char* specRegisterToken(char* rawString)
{
    char* tok=strdup(rawString);
    tokens.push_back(tok);
    return tok;
}

//######################################################################
// Expose parser API inputs/outputs to yacc as statics

// these are pointers to the arguments send into parse API
// made static here so the parser can see them in yacc actions
static std::vector<Editable*>* editables;
static std::vector<std::string>* variables;

static const char* ParseStr;    // string being parsed
static std::string ParseError;  // error (set from ExprSpecerror)
static ExprSpecNode* ParseResult; // must set result here since ExprSpecparse can't return it


//######################################################################
// Helpers used by actions to register data


/// Remember that there is an assignment to this variable (For autocomplete)
static void specRegisterVariable(const char* var)
{
    variables->push_back(var);
}

/// Variable Assignment/String literal should be turned into an editable
/// an editable is the data part of a control (it's model essentially)
static void specRegisterEditable(const char* var,ExprSpecNode* node)
{
    //dbgSeExpr <<"we have editable var "<<var;
    if(!node){
        //dbgSeExpr <<"   null ptr "<<var;
    }else if(ExprSpecScalarNode* n=dynamic_cast<ExprSpecScalarNode*>(node)){
        editables->push_back(new NumberEditable(var,node->startPos,node->endPos,n->v));
    }else if(ExprSpecVectorNode* n=dynamic_cast<ExprSpecVectorNode*>(node)){
        editables->push_back(new VectorEditable(var,node->startPos,node->endPos,n->v));
    }else if(ExprSpecStringNode* n=dynamic_cast<ExprSpecStringNode*>(node)){
        editables->push_back(new StringEditable(node->startPos,node->endPos,n->v));
    }else if(ExprSpecCCurveNode* n=dynamic_cast<ExprSpecCCurveNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if((args->nodes.size())%3==0){
                ColorCurveEditable* ccurve=new ColorCurveEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i+=3){
                    ExprSpecScalarNode* xnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i]);
                    ExprSpecVectorNode* ynode=dynamic_cast<ExprSpecVectorNode*>(args->nodes[i+1]);
                    ExprSpecScalarNode* interpnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+2]);
                    if (xnode && ynode && interpnode) {
                        if (interpnode->v >= 0 && interpnode->v <= 4) {
                            ccurve->add(xnode->v, ynode->v, interpnode->v);
                        } else {
                            // invalid interpolant type -- Amyspark
                            valid = false;
                        }
                    } else {
                        valid = false;
                    }
                }
                if(valid) editables->push_back(ccurve);
                else delete ccurve;
            }else{
                //dbgSeExpr <<"Curve has wrong # of args"<<args->nodes.size();
            }
        }
    }else if(ExprSpecCurveNode* n=dynamic_cast<ExprSpecCurveNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if((args->nodes.size())%3==0){
                CurveEditable* ccurve=new CurveEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i+=3){
                    ExprSpecScalarNode* xnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i]);
                    ExprSpecScalarNode* ynode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+1]);
                    ExprSpecScalarNode *interpnode = dynamic_cast<ExprSpecScalarNode *>(args->nodes[i + 2]);
                    if (xnode && ynode && interpnode) {
                        if (interpnode->v >= 0 && interpnode->v <= 4) {
                            ccurve->add(xnode->v, ynode->v, interpnode->v);
                        } else {
                            // invalid interpolant type -- Amyspark
                            valid = false;
                        }
                    } else {
                        valid = false;
                    }
                }
                if(valid) editables->push_back(ccurve);
                else{
                    delete ccurve;
                }
            }
        }
    }else if(ExprSpecColorSwatchNode* n=dynamic_cast<ExprSpecColorSwatchNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if(args->nodes.size()>0){
                ColorSwatchEditable* swatch=new ColorSwatchEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i++){
                    ExprSpecVectorNode* colornode=dynamic_cast<ExprSpecVectorNode*>(args->nodes[i]);
                    if(colornode){
                        swatch->add(colornode->v);
                    }else{
                        valid=false;
                    }
                }
                if(valid) editables->push_back(swatch);
                else delete swatch;
            }
        }
    }else{
        dbgSeExpr <<"SEEXPREDITOR LOGIC ERROR: We didn't recognize the Spec";
    }
}


/*******************
 parser declarations
 *******************/

// forward declaration
static void ExprSpecerror(const char* msg);


#line 252 "y.tab.c"

# ifndef ExprSpecYY_CAST
#  ifdef __cplusplus
#   define ExprSpecYY_CAST(Type, Val) static_cast<Type> (Val)
#   define ExprSpecYY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define ExprSpecYY_CAST(Type, Val) ((Type) (Val))
#   define ExprSpecYY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef ExprSpecYY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define ExprSpecYY_NULLPTR nullptr
#   else
#    define ExprSpecYY_NULLPTR 0
#   endif
#  else
#   define ExprSpecYY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef ExprSpecYY_EXPRSPEC_Y_TAB_H_INCLUDED
# define ExprSpecYY_EXPRSPEC_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef ExprSpecYYDEBUG
# define ExprSpecYYDEBUG 0
#endif
#if ExprSpecYYDEBUG
extern int ExprSpecdebug;
#endif

/* Token kinds.  */
#ifndef ExprSpecYYTOKENTYPE
# define ExprSpecYYTOKENTYPE
  enum ExprSpectokentype
  {
    ExprSpecYYEMPTY = -2,
    ExprSpecYYEOF = 0,                     /* "end of file"  */
    ExprSpecYYerror = 256,                 /* error  */
    ExprSpecYYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    NAME = 260,                    /* NAME  */
    VAR = 261,                     /* VAR  */
    STR = 262,                     /* STR  */
    NUMBER = 263,                  /* NUMBER  */
    AddEq = 264,                   /* AddEq  */
    SubEq = 265,                   /* SubEq  */
    MultEq = 266,                  /* MultEq  */
    DivEq = 267,                   /* DivEq  */
    ExpEq = 268,                   /* ExpEq  */
    ModEq = 269,                   /* ModEq  */
    ARROW = 270,                   /* ARROW  */
    OR = 271,                      /* OR  */
    AND = 272,                     /* AND  */
    EQ = 273,                      /* EQ  */
    NE = 274,                      /* NE  */
    LE = 275,                      /* LE  */
    GE = 276,                      /* GE  */
    UNARY = 277                    /* UNARY  */
  };
  typedef enum ExprSpectokentype ExprSpectoken_kind_t;
#endif

/* Value type.  */
#if ! defined ExprSpecYYSTYPE && ! defined ExprSpecYYSTYPE_IS_DECLARED
union ExprSpecYYSTYPE
{
#line 181 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"

    ExprSpecNode* n;
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: UNLIKE the regular parser, this is not strdup()'dthe string */

#line 330 "y.tab.c"

};
typedef union ExprSpecYYSTYPE ExprSpecYYSTYPE;
# define ExprSpecYYSTYPE_IS_TRIVIAL 1
# define ExprSpecYYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined ExprSpecYYLTYPE && ! defined ExprSpecYYLTYPE_IS_DECLARED
typedef struct ExprSpecYYLTYPE ExprSpecYYLTYPE;
struct ExprSpecYYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define ExprSpecYYLTYPE_IS_DECLARED 1
# define ExprSpecYYLTYPE_IS_TRIVIAL 1
#endif


extern ExprSpecYYSTYPE ExprSpeclval;
extern ExprSpecYYLTYPE ExprSpeclloc;
int ExprSpecparse (void);

#endif /* !ExprSpecYY_EXPRSPEC_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum ExprSpecsymbol_kind_t
{
  ExprSpecYYSYMBOL_ExprSpecYYEMPTY = -2,
  ExprSpecYYSYMBOL_ExprSpecYYEOF = 0,                      /* "end of file"  */
  ExprSpecYYSYMBOL_ExprSpecYYerror = 1,                    /* error  */
  ExprSpecYYSYMBOL_ExprSpecYYUNDEF = 2,                    /* "invalid token"  */
  ExprSpecYYSYMBOL_IF = 3,                         /* IF  */
  ExprSpecYYSYMBOL_ELSE = 4,                       /* ELSE  */
  ExprSpecYYSYMBOL_NAME = 5,                       /* NAME  */
  ExprSpecYYSYMBOL_VAR = 6,                        /* VAR  */
  ExprSpecYYSYMBOL_STR = 7,                        /* STR  */
  ExprSpecYYSYMBOL_NUMBER = 8,                     /* NUMBER  */
  ExprSpecYYSYMBOL_AddEq = 9,                      /* AddEq  */
  ExprSpecYYSYMBOL_SubEq = 10,                     /* SubEq  */
  ExprSpecYYSYMBOL_MultEq = 11,                    /* MultEq  */
  ExprSpecYYSYMBOL_DivEq = 12,                     /* DivEq  */
  ExprSpecYYSYMBOL_ExpEq = 13,                     /* ExpEq  */
  ExprSpecYYSYMBOL_ModEq = 14,                     /* ModEq  */
  ExprSpecYYSYMBOL_15_ = 15,                       /* '('  */
  ExprSpecYYSYMBOL_16_ = 16,                       /* ')'  */
  ExprSpecYYSYMBOL_ARROW = 17,                     /* ARROW  */
  ExprSpecYYSYMBOL_18_ = 18,                       /* ':'  */
  ExprSpecYYSYMBOL_19_ = 19,                       /* '?'  */
  ExprSpecYYSYMBOL_OR = 20,                        /* OR  */
  ExprSpecYYSYMBOL_AND = 21,                       /* AND  */
  ExprSpecYYSYMBOL_EQ = 22,                        /* EQ  */
  ExprSpecYYSYMBOL_NE = 23,                        /* NE  */
  ExprSpecYYSYMBOL_24_ = 24,                       /* '<'  */
  ExprSpecYYSYMBOL_25_ = 25,                       /* '>'  */
  ExprSpecYYSYMBOL_LE = 26,                        /* LE  */
  ExprSpecYYSYMBOL_GE = 27,                        /* GE  */
  ExprSpecYYSYMBOL_28_ = 28,                       /* '+'  */
  ExprSpecYYSYMBOL_29_ = 29,                       /* '-'  */
  ExprSpecYYSYMBOL_30_ = 30,                       /* '*'  */
  ExprSpecYYSYMBOL_31_ = 31,                       /* '/'  */
  ExprSpecYYSYMBOL_32_ = 32,                       /* '%'  */
  ExprSpecYYSYMBOL_UNARY = 33,                     /* UNARY  */
  ExprSpecYYSYMBOL_34_ = 34,                       /* '!'  */
  ExprSpecYYSYMBOL_35_ = 35,                       /* '~'  */
  ExprSpecYYSYMBOL_36_ = 36,                       /* '^'  */
  ExprSpecYYSYMBOL_37_ = 37,                       /* '['  */
  ExprSpecYYSYMBOL_38_ = 38,                       /* '='  */
  ExprSpecYYSYMBOL_39_ = 39,                       /* ';'  */
  ExprSpecYYSYMBOL_40_ = 40,                       /* '{'  */
  ExprSpecYYSYMBOL_41_ = 41,                       /* '}'  */
  ExprSpecYYSYMBOL_42_ = 42,                       /* ','  */
  ExprSpecYYSYMBOL_43_ = 43,                       /* ']'  */
  ExprSpecYYSYMBOL_ExprSpecYYACCEPT = 44,                  /* $accept  */
  ExprSpecYYSYMBOL_expr = 45,                      /* expr  */
  ExprSpecYYSYMBOL_optassigns = 46,                /* optassigns  */
  ExprSpecYYSYMBOL_assigns = 47,                   /* assigns  */
  ExprSpecYYSYMBOL_assign = 48,                    /* assign  */
  ExprSpecYYSYMBOL_ifthenelse = 49,                /* ifthenelse  */
  ExprSpecYYSYMBOL_optelse = 50,                   /* optelse  */
  ExprSpecYYSYMBOL_e = 51,                         /* e  */
  ExprSpecYYSYMBOL_optargs = 52,                   /* optargs  */
  ExprSpecYYSYMBOL_args = 53,                      /* args  */
  ExprSpecYYSYMBOL_arg = 54                        /* arg  */
};
typedef enum ExprSpecsymbol_kind_t ExprSpecsymbol_kind_t;




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
#  define ExprSpecYY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ ExprSpectype_int8;
#elif defined ExprSpecYY_STDINT_H
typedef int_least8_t ExprSpectype_int8;
#else
typedef signed char ExprSpectype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ ExprSpectype_int16;
#elif defined ExprSpecYY_STDINT_H
typedef int_least16_t ExprSpectype_int16;
#else
typedef short ExprSpectype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ ExprSpectype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined ExprSpecYY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t ExprSpectype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char ExprSpectype_uint8;
#else
typedef short ExprSpectype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ ExprSpectype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined ExprSpecYY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t ExprSpectype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short ExprSpectype_uint16;
#else
typedef int ExprSpectype_uint16;
#endif

#ifndef ExprSpecYYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define ExprSpecYYPTRDIFF_T __PTRDIFF_TYPE__
#  define ExprSpecYYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define ExprSpecYYPTRDIFF_T ptrdiff_t
#  define ExprSpecYYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define ExprSpecYYPTRDIFF_T long
#  define ExprSpecYYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef ExprSpecYYSIZE_T
# ifdef __SIZE_TYPE__
#  define ExprSpecYYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define ExprSpecYYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define ExprSpecYYSIZE_T size_t
# else
#  define ExprSpecYYSIZE_T unsigned
# endif
#endif

#define ExprSpecYYSIZE_MAXIMUM                                  \
  ExprSpecYY_CAST (ExprSpecYYPTRDIFF_T,                                 \
           (ExprSpecYYPTRDIFF_MAXIMUM < ExprSpecYY_CAST (ExprSpecYYSIZE_T, -1)  \
            ? ExprSpecYYPTRDIFF_MAXIMUM                         \
            : ExprSpecYY_CAST (ExprSpecYYSIZE_T, -1)))

#define ExprSpecYYSIZEOF(X) ExprSpecYY_CAST (ExprSpecYYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef ExprSpectype_uint8 ExprSpec_state_t;

/* State numbers in computations.  */
typedef int ExprSpec_state_fast_t;

#ifndef ExprSpecYY_
# if defined ExprSpecYYENABLE_NLS && ExprSpecYYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define ExprSpecYY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef ExprSpecYY_
#  define ExprSpecYY_(Msgid) Msgid
# endif
#endif


#ifndef ExprSpecYY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define ExprSpecYY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define ExprSpecYY_ATTRIBUTE_PURE
# endif
#endif

#ifndef ExprSpecYY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define ExprSpecYY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define ExprSpecYY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define ExprSpecYY_USE(E) ((void) (E))
#else
# define ExprSpecYY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about ExprSpeclval being uninitialized.  */
# define ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define ExprSpecYY_INITIAL_VALUE(Value) Value
#endif
#ifndef ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef ExprSpecYY_INITIAL_VALUE
# define ExprSpecYY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define ExprSpecYY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define ExprSpecYY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef ExprSpecYY_IGNORE_USELESS_CAST_BEGIN
# define ExprSpecYY_IGNORE_USELESS_CAST_BEGIN
# define ExprSpecYY_IGNORE_USELESS_CAST_END
#endif


#define ExprSpecYY_ASSERT(E) ((void) (0 && (E)))

#if !defined ExprSpecoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef ExprSpecYYSTACK_USE_ALLOCA
#  if ExprSpecYYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define ExprSpecYYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define ExprSpecYYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define ExprSpecYYSTACK_ALLOC alloca
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

# ifdef ExprSpecYYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define ExprSpecYYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef ExprSpecYYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define ExprSpecYYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define ExprSpecYYSTACK_ALLOC ExprSpecYYMALLOC
#  define ExprSpecYYSTACK_FREE ExprSpecYYFREE
#  ifndef ExprSpecYYSTACK_ALLOC_MAXIMUM
#   define ExprSpecYYSTACK_ALLOC_MAXIMUM ExprSpecYYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined ExprSpecYYMALLOC || defined malloc) \
             && (defined ExprSpecYYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef ExprSpecYYMALLOC
#   define ExprSpecYYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (ExprSpecYYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef ExprSpecYYFREE
#   define ExprSpecYYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined ExprSpecoverflow */

#if (! defined ExprSpecoverflow \
     && (! defined __cplusplus \
         || (defined ExprSpecYYLTYPE_IS_TRIVIAL && ExprSpecYYLTYPE_IS_TRIVIAL \
             && defined ExprSpecYYSTYPE_IS_TRIVIAL && ExprSpecYYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union ExprSpecalloc
{
  ExprSpec_state_t ExprSpecss_alloc;
  ExprSpecYYSTYPE ExprSpecvs_alloc;
  ExprSpecYYLTYPE ExprSpecls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define ExprSpecYYSTACK_GAP_MAXIMUM (ExprSpecYYSIZEOF (union ExprSpecalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define ExprSpecYYSTACK_BYTES(N) \
     ((N) * (ExprSpecYYSIZEOF (ExprSpec_state_t) + ExprSpecYYSIZEOF (ExprSpecYYSTYPE) \
             + ExprSpecYYSIZEOF (ExprSpecYYLTYPE)) \
      + 2 * ExprSpecYYSTACK_GAP_MAXIMUM)

# define ExprSpecYYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables ExprSpecYYSIZE and ExprSpecYYSTACKSIZE give the old and new number of
   elements in the stack, and ExprSpecYYPTR gives the new location of the
   stack.  Advance ExprSpecYYPTR to a properly aligned location for the next
   stack.  */
# define ExprSpecYYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        ExprSpecYYPTRDIFF_T ExprSpecnewbytes;                                         \
        ExprSpecYYCOPY (&ExprSpecptr->Stack_alloc, Stack, ExprSpecsize);                    \
        Stack = &ExprSpecptr->Stack_alloc;                                    \
        ExprSpecnewbytes = ExprSpecstacksize * ExprSpecYYSIZEOF (*Stack) + ExprSpecYYSTACK_GAP_MAXIMUM; \
        ExprSpecptr += ExprSpecnewbytes / ExprSpecYYSIZEOF (*ExprSpecptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined ExprSpecYYCOPY_NEEDED && ExprSpecYYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef ExprSpecYYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define ExprSpecYYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, ExprSpecYY_CAST (ExprSpecYYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define ExprSpecYYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          ExprSpecYYPTRDIFF_T ExprSpeci;                      \
          for (ExprSpeci = 0; ExprSpeci < (Count); ExprSpeci++)   \
            (Dst)[ExprSpeci] = (Src)[ExprSpeci];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !ExprSpecYYCOPY_NEEDED */

/* ExprSpecYYFINAL -- State number of the termination state.  */
#define ExprSpecYYFINAL  41
/* ExprSpecYYLAST -- Last index in ExprSpecYYTABLE.  */
#define ExprSpecYYLAST   680

/* ExprSpecYYNTOKENS -- Number of terminals.  */
#define ExprSpecYYNTOKENS  44
/* ExprSpecYYNNTS -- Number of nonterminals.  */
#define ExprSpecYYNNTS  11
/* ExprSpecYYNRULES -- Number of rules.  */
#define ExprSpecYYNRULES  59
/* ExprSpecYYNSTATES -- Number of states.  */
#define ExprSpecYYNSTATES  139

/* ExprSpecYYMAXUTOK -- Last valid token kind.  */
#define ExprSpecYYMAXUTOK   277


/* ExprSpecYYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by ExprSpeclex, with out-of-bounds checking.  */
#define ExprSpecYYTRANSLATE(ExprSpecYYX)                                \
  (0 <= (ExprSpecYYX) && (ExprSpecYYX) <= ExprSpecYYMAXUTOK                     \
   ? ExprSpecYY_CAST (ExprSpecsymbol_kind_t, ExprSpectranslate[ExprSpecYYX])        \
   : ExprSpecYYSYMBOL_ExprSpecYYUNDEF)

/* ExprSpecYYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by ExprSpeclex.  */
static const ExprSpectype_int8 ExprSpectranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,     2,     2,    32,     2,     2,
      15,    16,    30,    28,    42,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    39,
      24,    38,    25,    19,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    43,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,    35,     2,     2,     2,
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
      17,    20,    21,    22,    23,    26,    27,    33
};

#if ExprSpecYYDEBUG
  /* ExprSpecYYRLINE[ExprSpecYYN] -- Source line where rule number ExprSpecYYN was defined.  */
static const ExprSpectype_int16 ExprSpecrline[] =
{
       0,   223,   223,   224,   229,   230,   234,   235,   240,   241,
     245,   246,   247,   248,   249,   250,   251,   255,   256,   257,
     258,   259,   260,   264,   269,   270,   271,   276,   277,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     331,   332,   333,   334,   335,   340,   341,   346,   355,   367
};
#endif

/** Accessing symbol of state STATE.  */
#define ExprSpecYY_ACCESSING_SYMBOL(State) ExprSpecYY_CAST (ExprSpecsymbol_kind_t, ExprSpecstos[State])

#if ExprSpecYYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   ExprSpecYYSYMBOL.  No bounds checking.  */
static const char *ExprSpecsymbol_name (ExprSpecsymbol_kind_t ExprSpecsymbol) ExprSpecYY_ATTRIBUTE_UNUSED;

/* ExprSpecYYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at ExprSpecYYNTOKENS, nonterminals.  */
static const char *const ExprSpectname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "NAME",
  "VAR", "STR", "NUMBER", "AddEq", "SubEq", "MultEq", "DivEq", "ExpEq",
  "ModEq", "'('", "')'", "ARROW", "':'", "'?'", "OR", "AND", "EQ", "NE",
  "'<'", "'>'", "LE", "GE", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY",
  "'!'", "'~'", "'^'", "'['", "'='", "';'", "'{'", "'}'", "','", "']'",
  "$accept", "expr", "optassigns", "assigns", "assign", "ifthenelse",
  "optelse", "e", "optargs", "args", "arg", ExprSpecYY_NULLPTR
};

static const char *
ExprSpecsymbol_name (ExprSpecsymbol_kind_t ExprSpecsymbol)
{
  return ExprSpectname[ExprSpecsymbol];
}
#endif

#ifdef ExprSpecYYPRINT
/* ExprSpecYYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const ExprSpectype_int16 ExprSpectoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    40,    41,   270,    58,    63,
     271,   272,   273,   274,    60,    62,   275,   276,    43,    45,
      42,    47,    37,   277,    33,   126,    94,    91,    61,    59,
     123,   125,    44,    93
};
#endif

#define ExprSpecYYPACT_NINF (-60)

#define ExprSpecpact_value_is_default(Yyn) \
  ((Yyn) == ExprSpecYYPACT_NINF)

#define ExprSpecYYTABLE_NINF (-1)

#define ExprSpectable_value_is_error(Yyn) \
  0

  /* ExprSpecYYPACT[STATE-NUM] -- Index in ExprSpecYYTABLE of the portion describing
     STATE-NUM.  */
static const ExprSpectype_int16 ExprSpecpact[] =
{
      58,   -10,    91,    24,   -60,   -60,    62,    62,    62,    62,
      62,    62,    16,    58,   -60,   -60,   577,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    25,   -60,   513,   -33,   -33,   -33,   -33,
     168,   -60,   -60,   577,    19,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,   535,   216,   237,   258,   279,   300,   321,   577,    26,
       1,   -60,   342,   363,   384,   405,   426,   447,   468,   489,
     -60,    62,    29,   556,   613,   629,   643,   643,    95,    95,
      95,    95,    48,    48,   -33,   -33,   -33,   -33,   118,    31,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,    62,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   192,    62,    62,   -60,
       9,   -60,    62,    56,   596,    98,    24,    33,     9,   143,
     -60,    37,   -60,    -1,   -60,     9,   -60,    34,   -60
};

  /* ExprSpecYYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when ExprSpecYYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const ExprSpectype_int8 ExprSpecdefact[] =
{
       0,     0,    52,    51,    54,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     8,     3,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    51,     0,    39,    40,    41,    42,
       0,     1,     7,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
      56,    57,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    43,    44,    45,    46,    47,    48,     0,     0,
      17,    18,    19,    20,    21,    22,    49,     0,    16,    10,
      11,    12,    13,    14,    15,     9,     0,    55,     0,    29,
       4,    58,     0,     0,    30,     0,     0,     0,     5,     0,
      50,    24,    28,     0,    23,     4,    26,     0,    25
};

  /* ExprSpecYYPGOTO[NTERM-NUM].  */
static const ExprSpectype_int8 ExprSpecpgoto[] =
{
     -60,   -60,   -59,    82,   -12,   -50,   -60,     0,   -29,   -60,
     -18
};

  /* ExprSpecYYDEFGOTO[NTERM-NUM].  */
static const ExprSpectype_uint8 ExprSpecdefgoto[] =
{
       0,    12,   127,   128,    14,    15,   134,    68,    69,    70,
      71
};

  /* ExprSpecYYTABLE[ExprSpecYYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If ExprSpecYYTABLE_NINF, syntax error.  */
static const ExprSpectype_uint8 ExprSpectable[] =
{
      16,    42,     1,    59,    60,    17,    35,    36,    37,    38,
      39,    40,     1,    43,   125,   126,    41,    61,    62,    63,
      64,    65,    66,    67,    82,    72,    73,    74,    75,    76,
      77,    78,    79,    26,    27,    28,    29,    30,    31,   135,
      24,   133,   106,   107,   117,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     1,    32,     2,     3,     4,     5,    33,    34,     4,
       5,   120,   130,     6,   131,   138,   137,     6,    56,    57,
      58,   116,    13,   136,    59,    60,     7,     8,   123,   121,
       7,     8,     9,    10,     0,    11,     9,    10,     0,    11,
      18,    19,    20,    21,    22,    23,    24,    18,    19,    20,
      21,    22,    23,     0,     0,     0,    42,     0,   124,     0,
       0,     0,   129,    54,    55,    56,    57,    58,     0,    25,
       0,    59,    60,     0,     0,    44,    25,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     0,     0,     0,    59,    60,     0,     0,     0,     0,
      44,   119,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,    59,
      60,     0,     0,     0,     0,    44,   132,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     0,     0,     0,    59,    60,     0,     0,     0,    44,
      81,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,     0,     0,     0,    59,    60,
       0,     0,     0,    44,   122,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,     0,    59,    60,    44,   100,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       0,     0,     0,    59,    60,    44,   101,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     0,     0,     0,    59,    60,    44,   102,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     0,     0,     0,    59,    60,    44,   103,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     0,     0,     0,    59,    60,    44,   104,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     0,     0,     0,    59,    60,    44,
     105,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,     0,     0,     0,    59,    60,
      44,   108,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,    59,
      60,    44,   109,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,     0,     0,     0,
      59,    60,    44,   110,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,     0,     0,
       0,    59,    60,    44,   111,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,     0,    59,    60,    44,   112,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       0,     0,     0,    59,    60,    44,   113,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     0,     0,     0,    59,    60,    44,   114,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     0,     0,     0,    59,    60,     0,   115,    80,
      44,     0,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,    59,
      60,    99,    44,     0,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,     0,     0,
       0,    59,    60,    44,   118,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,     0,    59,    60,    44,     0,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       0,     0,     0,    59,    60,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,     0,    59,    60,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,    59,
      60,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     0,     0,     0,    59,    60,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,    59,
      60
};

static const ExprSpectype_int16 ExprSpeccheck[] =
{
       0,    13,     3,    36,    37,    15,     6,     7,     8,     9,
      10,    11,     3,    13,     5,     6,     0,    17,    18,    19,
      20,    21,    22,    23,     5,    25,    26,    27,    28,    29,
      30,    31,    32,     9,    10,    11,    12,    13,    14,    40,
      15,     4,    16,    42,    15,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,     3,    38,     5,     6,     7,     8,     5,     6,     7,
       8,    40,    16,    15,    41,    41,   135,    15,    30,    31,
      32,    81,     0,   133,    36,    37,    28,    29,   117,   107,
      28,    29,    34,    35,    -1,    37,    34,    35,    -1,    37,
       9,    10,    11,    12,    13,    14,    15,     9,    10,    11,
      12,    13,    14,    -1,    -1,    -1,   128,    -1,   118,    -1,
      -1,    -1,   122,    28,    29,    30,    31,    32,    -1,    38,
      -1,    36,    37,    -1,    -1,    17,    38,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      17,    43,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    17,    43,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    17,
      42,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    17,    42,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    17,    39,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    37,    17,    39,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    17,    39,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    17,    39,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    37,    17,    39,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    36,    37,    17,
      39,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    36,    37,
      17,    39,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    17,    39,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    37,    17,    39,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    37,    17,    39,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    17,    39,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    37,    17,    39,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    17,    39,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    -1,    39,    16,
      17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    16,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    37,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    17,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    37,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37
};

  /* ExprSpecYYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const ExprSpectype_int8 ExprSpecstos[] =
{
       0,     3,     5,     6,     7,     8,    15,    28,    29,    34,
      35,    37,    45,    47,    48,    49,    51,    15,     9,    10,
      11,    12,    13,    14,    15,    38,     9,    10,    11,    12,
      13,    14,    38,     5,     6,    51,    51,    51,    51,    51,
      51,     0,    48,    51,    17,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    36,
      37,    51,    51,    51,    51,    51,    51,    51,    51,    52,
      53,    54,    51,    51,    51,    51,    51,    51,    51,    51,
      16,    42,     5,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    16,
      39,    39,    39,    39,    39,    39,    16,    42,    39,    39,
      39,    39,    39,    39,    39,    39,    51,    15,    18,    43,
      40,    54,    42,    52,    51,     5,     6,    46,    47,    51,
      16,    41,    43,     4,    50,    40,    49,    46,    41
};

  /* ExprSpecYYR1[ExprSpecYYN] -- Symbol number of symbol that rule ExprSpecYYN derives.  */
static const ExprSpectype_int8 ExprSpecr1[] =
{
       0,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    49,    50,    50,    50,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    52,    52,    53,    53,    54
};

  /* ExprSpecYYR2[ExprSpecYYN] -- Number of symbols on the right hand side of rule ExprSpecYYN.  */
static const ExprSpectype_int8 ExprSpecr2[] =
{
       0,     2,     2,     1,     0,     1,     1,     2,     1,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     0,     4,     2,     3,     7,     4,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     4,
       6,     1,     1,     1,     1,     0,     1,     1,     3,     1
};


enum { ExprSpecYYENOMEM = -2 };

#define ExprSpecerrok         (ExprSpecerrstatus = 0)
#define ExprSpecclearin       (ExprSpecchar = ExprSpecYYEMPTY)

#define ExprSpecYYACCEPT        goto ExprSpecacceptlab
#define ExprSpecYYABORT         goto ExprSpecabortlab
#define ExprSpecYYERROR         goto ExprSpecerrorlab


#define ExprSpecYYRECOVERING()  (!!ExprSpecerrstatus)

#define ExprSpecYYBACKUP(Token, Value)                                    \
  do                                                              \
    if (ExprSpecchar == ExprSpecYYEMPTY)                                        \
      {                                                           \
        ExprSpecchar = (Token);                                         \
        ExprSpeclval = (Value);                                         \
        ExprSpecYYPOPSTACK (ExprSpeclen);                                       \
        ExprSpecstate = *ExprSpecssp;                                         \
        goto ExprSpecbackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        ExprSpecerror (ExprSpecYY_("syntax error: cannot back up")); \
        ExprSpecYYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use ExprSpecYYerror or ExprSpecYYUNDEF. */
#define ExprSpecYYERRCODE ExprSpecYYUNDEF

/* ExprSpecYYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef ExprSpecYYLLOC_DEFAULT
# define ExprSpecYYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = ExprSpecYYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = ExprSpecYYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = ExprSpecYYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = ExprSpecYYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            ExprSpecYYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            ExprSpecYYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define ExprSpecYYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if ExprSpecYYDEBUG

# ifndef ExprSpecYYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define ExprSpecYYFPRINTF fprintf
# endif

# define ExprSpecYYDPRINTF(Args)                        \
do {                                            \
  if (ExprSpecdebug)                                  \
    ExprSpecYYFPRINTF Args;                             \
} while (0)


/* ExprSpecYY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef ExprSpecYY_LOCATION_PRINT
#  if defined ExprSpecYYLTYPE_IS_TRIVIAL && ExprSpecYYLTYPE_IS_TRIVIAL

/* Print *ExprSpecYYLOCP on ExprSpecYYO.  Private, do not rely on its existence. */

ExprSpecYY_ATTRIBUTE_UNUSED
static int
ExprSpec_location_print_ (FILE *ExprSpeco, ExprSpecYYLTYPE const * const ExprSpeclocp)
{
  int res = 0;
  int end_col = 0 != ExprSpeclocp->last_column ? ExprSpeclocp->last_column - 1 : 0;
  if (0 <= ExprSpeclocp->first_line)
    {
      res += ExprSpecYYFPRINTF (ExprSpeco, "%d", ExprSpeclocp->first_line);
      if (0 <= ExprSpeclocp->first_column)
        res += ExprSpecYYFPRINTF (ExprSpeco, ".%d", ExprSpeclocp->first_column);
    }
  if (0 <= ExprSpeclocp->last_line)
    {
      if (ExprSpeclocp->first_line < ExprSpeclocp->last_line)
        {
          res += ExprSpecYYFPRINTF (ExprSpeco, "-%d", ExprSpeclocp->last_line);
          if (0 <= end_col)
            res += ExprSpecYYFPRINTF (ExprSpeco, ".%d", end_col);
        }
      else if (0 <= end_col && ExprSpeclocp->first_column < end_col)
        res += ExprSpecYYFPRINTF (ExprSpeco, "-%d", end_col);
    }
  return res;
 }

#   define ExprSpecYY_LOCATION_PRINT(File, Loc)          \
  ExprSpec_location_print_ (File, &(Loc))

#  else
#   define ExprSpecYY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined ExprSpecYY_LOCATION_PRINT */


# define ExprSpecYY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (ExprSpecdebug)                                                            \
    {                                                                     \
      ExprSpecYYFPRINTF (stderr, "%s ", Title);                                   \
      ExprSpec_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      ExprSpecYYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on ExprSpecYYO.  |
`-----------------------------------*/

static void
ExprSpec_symbol_value_print (FILE *ExprSpeco,
                       ExprSpecsymbol_kind_t ExprSpeckind, ExprSpecYYSTYPE const * const ExprSpecvaluep, ExprSpecYYLTYPE const * const ExprSpeclocationp)
{
  FILE *ExprSpecoutput = ExprSpeco;
  ExprSpecYY_USE (ExprSpecoutput);
  ExprSpecYY_USE (ExprSpeclocationp);
  if (!ExprSpecvaluep)
    return;
# ifdef ExprSpecYYPRINT
  if (ExprSpeckind < ExprSpecYYNTOKENS)
    ExprSpecYYPRINT (ExprSpeco, ExprSpectoknum[ExprSpeckind], *ExprSpecvaluep);
# endif
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ExprSpecYY_USE (ExprSpeckind);
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on ExprSpecYYO.  |
`---------------------------*/

static void
ExprSpec_symbol_print (FILE *ExprSpeco,
                 ExprSpecsymbol_kind_t ExprSpeckind, ExprSpecYYSTYPE const * const ExprSpecvaluep, ExprSpecYYLTYPE const * const ExprSpeclocationp)
{
  ExprSpecYYFPRINTF (ExprSpeco, "%s %s (",
             ExprSpeckind < ExprSpecYYNTOKENS ? "token" : "nterm", ExprSpecsymbol_name (ExprSpeckind));

  ExprSpecYY_LOCATION_PRINT (ExprSpeco, *ExprSpeclocationp);
  ExprSpecYYFPRINTF (ExprSpeco, ": ");
  ExprSpec_symbol_value_print (ExprSpeco, ExprSpeckind, ExprSpecvaluep, ExprSpeclocationp);
  ExprSpecYYFPRINTF (ExprSpeco, ")");
}

/*------------------------------------------------------------------.
| ExprSpec_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
ExprSpec_stack_print (ExprSpec_state_t *ExprSpecbottom, ExprSpec_state_t *ExprSpectop)
{
  ExprSpecYYFPRINTF (stderr, "Stack now");
  for (; ExprSpecbottom <= ExprSpectop; ExprSpecbottom++)
    {
      int ExprSpecbot = *ExprSpecbottom;
      ExprSpecYYFPRINTF (stderr, " %d", ExprSpecbot);
    }
  ExprSpecYYFPRINTF (stderr, "\n");
}

# define ExprSpecYY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (ExprSpecdebug)                                                  \
    ExprSpec_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the ExprSpecYYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
ExprSpec_reduce_print (ExprSpec_state_t *ExprSpecssp, ExprSpecYYSTYPE *ExprSpecvsp, ExprSpecYYLTYPE *ExprSpeclsp,
                 int ExprSpecrule)
{
  int ExprSpeclno = ExprSpecrline[ExprSpecrule];
  int ExprSpecnrhs = ExprSpecr2[ExprSpecrule];
  int ExprSpeci;
  ExprSpecYYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             ExprSpecrule - 1, ExprSpeclno);
  /* The symbols being reduced.  */
  for (ExprSpeci = 0; ExprSpeci < ExprSpecnrhs; ExprSpeci++)
    {
      ExprSpecYYFPRINTF (stderr, "   $%d = ", ExprSpeci + 1);
      ExprSpec_symbol_print (stderr,
                       ExprSpecYY_ACCESSING_SYMBOL (+ExprSpecssp[ExprSpeci + 1 - ExprSpecnrhs]),
                       &ExprSpecvsp[(ExprSpeci + 1) - (ExprSpecnrhs)],
                       &(ExprSpeclsp[(ExprSpeci + 1) - (ExprSpecnrhs)]));
      ExprSpecYYFPRINTF (stderr, "\n");
    }
}

# define ExprSpecYY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (ExprSpecdebug)                          \
    ExprSpec_reduce_print (ExprSpecssp, ExprSpecvsp, ExprSpeclsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int ExprSpecdebug;
#else /* !ExprSpecYYDEBUG */
# define ExprSpecYYDPRINTF(Args) ((void) 0)
# define ExprSpecYY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define ExprSpecYY_STACK_PRINT(Bottom, Top)
# define ExprSpecYY_REDUCE_PRINT(Rule)
#endif /* !ExprSpecYYDEBUG */


/* ExprSpecYYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef ExprSpecYYINITDEPTH
# define ExprSpecYYINITDEPTH 200
#endif

/* ExprSpecYYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   ExprSpecYYSTACK_ALLOC_MAXIMUM < ExprSpecYYSTACK_BYTES (ExprSpecYYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef ExprSpecYYMAXDEPTH
# define ExprSpecYYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
ExprSpecdestruct (const char *ExprSpecmsg,
            ExprSpecsymbol_kind_t ExprSpeckind, ExprSpecYYSTYPE *ExprSpecvaluep, ExprSpecYYLTYPE *ExprSpeclocationp)
{
  ExprSpecYY_USE (ExprSpecvaluep);
  ExprSpecYY_USE (ExprSpeclocationp);
  if (!ExprSpecmsg)
    ExprSpecmsg = "Deleting";
  ExprSpecYY_SYMBOL_PRINT (ExprSpecmsg, ExprSpeckind, ExprSpecvaluep, ExprSpeclocationp);

  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ExprSpecYY_USE (ExprSpeckind);
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int ExprSpecchar;

/* The semantic value of the lookahead symbol.  */
ExprSpecYYSTYPE ExprSpeclval;
/* Location data for the lookahead symbol.  */
ExprSpecYYLTYPE ExprSpeclloc
# if defined ExprSpecYYLTYPE_IS_TRIVIAL && ExprSpecYYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int ExprSpecnerrs;




/*----------.
| ExprSpecparse.  |
`----------*/

int
ExprSpecparse (void)
{
    ExprSpec_state_fast_t ExprSpecstate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int ExprSpecerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow ExprSpecoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    ExprSpecYYPTRDIFF_T ExprSpecstacksize = ExprSpecYYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    ExprSpec_state_t ExprSpecssa[ExprSpecYYINITDEPTH];
    ExprSpec_state_t *ExprSpecss = ExprSpecssa;
    ExprSpec_state_t *ExprSpecssp = ExprSpecss;

    /* The semantic value stack: array, bottom, top.  */
    ExprSpecYYSTYPE ExprSpecvsa[ExprSpecYYINITDEPTH];
    ExprSpecYYSTYPE *ExprSpecvs = ExprSpecvsa;
    ExprSpecYYSTYPE *ExprSpecvsp = ExprSpecvs;

    /* The location stack: array, bottom, top.  */
    ExprSpecYYLTYPE ExprSpeclsa[ExprSpecYYINITDEPTH];
    ExprSpecYYLTYPE *ExprSpecls = ExprSpeclsa;
    ExprSpecYYLTYPE *ExprSpeclsp = ExprSpecls;

  int ExprSpecn;
  /* The return value of ExprSpecparse.  */
  int ExprSpecresult;
  /* Lookahead symbol kind.  */
  ExprSpecsymbol_kind_t ExprSpectoken = ExprSpecYYSYMBOL_ExprSpecYYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  ExprSpecYYSTYPE ExprSpecval;
  ExprSpecYYLTYPE ExprSpecloc;

  /* The locations where the error started and ended.  */
  ExprSpecYYLTYPE ExprSpecerror_range[3];



#define ExprSpecYYPOPSTACK(N)   (ExprSpecvsp -= (N), ExprSpecssp -= (N), ExprSpeclsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int ExprSpeclen = 0;

  ExprSpecYYDPRINTF ((stderr, "Starting parse\n"));

  ExprSpecchar = ExprSpecYYEMPTY; /* Cause a token to be read.  */
  ExprSpeclsp[0] = ExprSpeclloc;
  goto ExprSpecsetstate;


/*------------------------------------------------------------.
| ExprSpecnewstate -- push a new state, which is found in ExprSpecstate.  |
`------------------------------------------------------------*/
ExprSpecnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  ExprSpecssp++;


/*--------------------------------------------------------------------.
| ExprSpecsetstate -- set current state (the top of the stack) to ExprSpecstate.  |
`--------------------------------------------------------------------*/
ExprSpecsetstate:
  ExprSpecYYDPRINTF ((stderr, "Entering state %d\n", ExprSpecstate));
  ExprSpecYY_ASSERT (0 <= ExprSpecstate && ExprSpecstate < ExprSpecYYNSTATES);
  ExprSpecYY_IGNORE_USELESS_CAST_BEGIN
  *ExprSpecssp = ExprSpecYY_CAST (ExprSpec_state_t, ExprSpecstate);
  ExprSpecYY_IGNORE_USELESS_CAST_END
  ExprSpecYY_STACK_PRINT (ExprSpecss, ExprSpecssp);

  if (ExprSpecss + ExprSpecstacksize - 1 <= ExprSpecssp)
#if !defined ExprSpecoverflow && !defined ExprSpecYYSTACK_RELOCATE
    goto ExprSpecexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      ExprSpecYYPTRDIFF_T ExprSpecsize = ExprSpecssp - ExprSpecss + 1;

# if defined ExprSpecoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        ExprSpec_state_t *ExprSpecss1 = ExprSpecss;
        ExprSpecYYSTYPE *ExprSpecvs1 = ExprSpecvs;
        ExprSpecYYLTYPE *ExprSpecls1 = ExprSpecls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if ExprSpecoverflow is a macro.  */
        ExprSpecoverflow (ExprSpecYY_("memory exhausted"),
                    &ExprSpecss1, ExprSpecsize * ExprSpecYYSIZEOF (*ExprSpecssp),
                    &ExprSpecvs1, ExprSpecsize * ExprSpecYYSIZEOF (*ExprSpecvsp),
                    &ExprSpecls1, ExprSpecsize * ExprSpecYYSIZEOF (*ExprSpeclsp),
                    &ExprSpecstacksize);
        ExprSpecss = ExprSpecss1;
        ExprSpecvs = ExprSpecvs1;
        ExprSpecls = ExprSpecls1;
      }
# else /* defined ExprSpecYYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (ExprSpecYYMAXDEPTH <= ExprSpecstacksize)
        goto ExprSpecexhaustedlab;
      ExprSpecstacksize *= 2;
      if (ExprSpecYYMAXDEPTH < ExprSpecstacksize)
        ExprSpecstacksize = ExprSpecYYMAXDEPTH;

      {
        ExprSpec_state_t *ExprSpecss1 = ExprSpecss;
        union ExprSpecalloc *ExprSpecptr =
          ExprSpecYY_CAST (union ExprSpecalloc *,
                   ExprSpecYYSTACK_ALLOC (ExprSpecYY_CAST (ExprSpecYYSIZE_T, ExprSpecYYSTACK_BYTES (ExprSpecstacksize))));
        if (! ExprSpecptr)
          goto ExprSpecexhaustedlab;
        ExprSpecYYSTACK_RELOCATE (ExprSpecss_alloc, ExprSpecss);
        ExprSpecYYSTACK_RELOCATE (ExprSpecvs_alloc, ExprSpecvs);
        ExprSpecYYSTACK_RELOCATE (ExprSpecls_alloc, ExprSpecls);
#  undef ExprSpecYYSTACK_RELOCATE
        if (ExprSpecss1 != ExprSpecssa)
          ExprSpecYYSTACK_FREE (ExprSpecss1);
      }
# endif

      ExprSpecssp = ExprSpecss + ExprSpecsize - 1;
      ExprSpecvsp = ExprSpecvs + ExprSpecsize - 1;
      ExprSpeclsp = ExprSpecls + ExprSpecsize - 1;

      ExprSpecYY_IGNORE_USELESS_CAST_BEGIN
      ExprSpecYYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  ExprSpecYY_CAST (long, ExprSpecstacksize)));
      ExprSpecYY_IGNORE_USELESS_CAST_END

      if (ExprSpecss + ExprSpecstacksize - 1 <= ExprSpecssp)
        ExprSpecYYABORT;
    }
#endif /* !defined ExprSpecoverflow && !defined ExprSpecYYSTACK_RELOCATE */

  if (ExprSpecstate == ExprSpecYYFINAL)
    ExprSpecYYACCEPT;

  goto ExprSpecbackup;


/*-----------.
| ExprSpecbackup.  |
`-----------*/
ExprSpecbackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  ExprSpecn = ExprSpecpact[ExprSpecstate];
  if (ExprSpecpact_value_is_default (ExprSpecn))
    goto ExprSpecdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* ExprSpecYYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (ExprSpecchar == ExprSpecYYEMPTY)
    {
      ExprSpecYYDPRINTF ((stderr, "Reading a token\n"));
      ExprSpecchar = ExprSpeclex ();
    }

  if (ExprSpecchar <= ExprSpecYYEOF)
    {
      ExprSpecchar = ExprSpecYYEOF;
      ExprSpectoken = ExprSpecYYSYMBOL_ExprSpecYYEOF;
      ExprSpecYYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (ExprSpecchar == ExprSpecYYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      ExprSpecchar = ExprSpecYYUNDEF;
      ExprSpectoken = ExprSpecYYSYMBOL_ExprSpecYYerror;
      ExprSpecerror_range[1] = ExprSpeclloc;
      goto ExprSpecerrlab1;
    }
  else
    {
      ExprSpectoken = ExprSpecYYTRANSLATE (ExprSpecchar);
      ExprSpecYY_SYMBOL_PRINT ("Next token is", ExprSpectoken, &ExprSpeclval, &ExprSpeclloc);
    }

  /* If the proper action on seeing token ExprSpecYYTOKEN is to reduce or to
     detect an error, take that action.  */
  ExprSpecn += ExprSpectoken;
  if (ExprSpecn < 0 || ExprSpecYYLAST < ExprSpecn || ExprSpeccheck[ExprSpecn] != ExprSpectoken)
    goto ExprSpecdefault;
  ExprSpecn = ExprSpectable[ExprSpecn];
  if (ExprSpecn <= 0)
    {
      if (ExprSpectable_value_is_error (ExprSpecn))
        goto ExprSpecerrlab;
      ExprSpecn = -ExprSpecn;
      goto ExprSpecreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (ExprSpecerrstatus)
    ExprSpecerrstatus--;

  /* Shift the lookahead token.  */
  ExprSpecYY_SYMBOL_PRINT ("Shifting", ExprSpectoken, &ExprSpeclval, &ExprSpeclloc);
  ExprSpecstate = ExprSpecn;
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++ExprSpecvsp = ExprSpeclval;
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END
  *++ExprSpeclsp = ExprSpeclloc;

  /* Discard the shifted token.  */
  ExprSpecchar = ExprSpecYYEMPTY;
  goto ExprSpecnewstate;


/*-----------------------------------------------------------.
| ExprSpecdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
ExprSpecdefault:
  ExprSpecn = ExprSpecdefact[ExprSpecstate];
  if (ExprSpecn == 0)
    goto ExprSpecerrlab;
  goto ExprSpecreduce;


/*-----------------------------.
| ExprSpecreduce -- do a reduction.  |
`-----------------------------*/
ExprSpecreduce:
  /* ExprSpecn is the number of a rule to reduce with.  */
  ExprSpeclen = ExprSpecr2[ExprSpecn];

  /* If ExprSpecYYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets ExprSpecYYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to ExprSpecYYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that ExprSpecYYVAL may be used uninitialized.  */
  ExprSpecval = ExprSpecvsp[1-ExprSpeclen];

  /* Default location. */
  ExprSpecYYLLOC_DEFAULT (ExprSpecloc, (ExprSpeclsp - ExprSpeclen), ExprSpeclen);
  ExprSpecerror_range[1] = ExprSpecloc;
  ExprSpecYY_REDUCE_PRINT (ExprSpecn);
  switch (ExprSpecn)
    {
  case 2: /* expr: assigns e  */
#line 223 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { ParseResult = 0; }
#line 1669 "y.tab.c"
    break;

  case 3: /* expr: e  */
#line 224 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { ParseResult = 0; }
#line 1675 "y.tab.c"
    break;

  case 4: /* optassigns: %empty  */
#line 229 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1681 "y.tab.c"
    break;

  case 5: /* optassigns: assigns  */
#line 230 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1687 "y.tab.c"
    break;

  case 6: /* assigns: assign  */
#line 234 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1693 "y.tab.c"
    break;

  case 7: /* assigns: assigns assign  */
#line 235 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1699 "y.tab.c"
    break;

  case 8: /* assign: ifthenelse  */
#line 240 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1705 "y.tab.c"
    break;

  case 9: /* assign: VAR '=' e ';'  */
#line 241 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {
        specRegisterVariable((ExprSpecvsp[-3].s));
        specRegisterEditable((ExprSpecvsp[-3].s),(ExprSpecvsp[-1].n));
      }
#line 1714 "y.tab.c"
    break;

  case 10: /* assign: VAR AddEq e ';'  */
#line 245 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1720 "y.tab.c"
    break;

  case 11: /* assign: VAR SubEq e ';'  */
#line 246 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1726 "y.tab.c"
    break;

  case 12: /* assign: VAR MultEq e ';'  */
#line 247 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1732 "y.tab.c"
    break;

  case 13: /* assign: VAR DivEq e ';'  */
#line 248 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1738 "y.tab.c"
    break;

  case 14: /* assign: VAR ExpEq e ';'  */
#line 249 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1744 "y.tab.c"
    break;

  case 15: /* assign: VAR ModEq e ';'  */
#line 250 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1750 "y.tab.c"
    break;

  case 16: /* assign: NAME '=' e ';'  */
#line 251 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {
        specRegisterVariable((ExprSpecvsp[-3].s));
        specRegisterEditable((ExprSpecvsp[-3].s),(ExprSpecvsp[-1].n));
      }
#line 1759 "y.tab.c"
    break;

  case 17: /* assign: NAME AddEq e ';'  */
#line 255 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1765 "y.tab.c"
    break;

  case 18: /* assign: NAME SubEq e ';'  */
#line 256 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1771 "y.tab.c"
    break;

  case 19: /* assign: NAME MultEq e ';'  */
#line 257 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1777 "y.tab.c"
    break;

  case 20: /* assign: NAME DivEq e ';'  */
#line 258 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1783 "y.tab.c"
    break;

  case 21: /* assign: NAME ExpEq e ';'  */
#line 259 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1789 "y.tab.c"
    break;

  case 22: /* assign: NAME ModEq e ';'  */
#line 260 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 1795 "y.tab.c"
    break;

  case 23: /* ifthenelse: IF '(' e ')' '{' optassigns '}' optelse  */
#line 265 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
    { (ExprSpecval.n) = 0; }
#line 1801 "y.tab.c"
    break;

  case 24: /* optelse: %empty  */
#line 269 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                        { (ExprSpecval.n) = 0; }
#line 1807 "y.tab.c"
    break;

  case 25: /* optelse: ELSE '{' optassigns '}'  */
#line 270 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                            { (ExprSpecval.n) = 0;}
#line 1813 "y.tab.c"
    break;

  case 26: /* optelse: ELSE ifthenelse  */
#line 271 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0;}
#line 1819 "y.tab.c"
    break;

  case 27: /* e: '(' e ')'  */
#line 276 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1825 "y.tab.c"
    break;

  case 28: /* e: '[' e ',' e ',' e ']'  */
#line 277 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {
        if(SPEC_IS_NUMBER((ExprSpecvsp[-5].n)) && SPEC_IS_NUMBER((ExprSpecvsp[-3].n)) && SPEC_IS_NUMBER((ExprSpecvsp[-1].n))){
            (ExprSpecval.n)=remember(new ExprSpecVectorNode((ExprSpecloc).first_column,(ExprSpecloc).last_column,(ExprSpecvsp[-5].n),(ExprSpecvsp[-3].n),(ExprSpecvsp[-1].n)));
        }else (ExprSpecval.n)=0;
      }
#line 1835 "y.tab.c"
    break;

  case 29: /* e: e '[' e ']'  */
#line 282 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1841 "y.tab.c"
    break;

  case 30: /* e: e '?' e ':' e  */
#line 283 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1847 "y.tab.c"
    break;

  case 31: /* e: e OR e  */
#line 284 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1853 "y.tab.c"
    break;

  case 32: /* e: e AND e  */
#line 285 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1859 "y.tab.c"
    break;

  case 33: /* e: e EQ e  */
#line 286 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1865 "y.tab.c"
    break;

  case 34: /* e: e NE e  */
#line 287 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1871 "y.tab.c"
    break;

  case 35: /* e: e '<' e  */
#line 288 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1877 "y.tab.c"
    break;

  case 36: /* e: e '>' e  */
#line 289 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1883 "y.tab.c"
    break;

  case 37: /* e: e LE e  */
#line 290 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1889 "y.tab.c"
    break;

  case 38: /* e: e GE e  */
#line 291 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1895 "y.tab.c"
    break;

  case 39: /* e: '+' e  */
#line 292 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = (ExprSpecvsp[0].n); }
#line 1901 "y.tab.c"
    break;

  case 40: /* e: '-' e  */
#line 293 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {
        if(SPEC_IS_NUMBER((ExprSpecvsp[0].n))){
            ExprSpecScalarNode* node=(ExprSpecScalarNode*)(ExprSpecvsp[0].n);
            node->v*=-1;
            node->startPos=(ExprSpecloc).first_column;
            node->endPos=(ExprSpecloc).last_column;
            (ExprSpecval.n)=(ExprSpecvsp[0].n);
        }else (ExprSpecval.n)=0;
      }
#line 1915 "y.tab.c"
    break;

  case 41: /* e: '!' e  */
#line 302 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1921 "y.tab.c"
    break;

  case 42: /* e: '~' e  */
#line 303 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1927 "y.tab.c"
    break;

  case 43: /* e: e '+' e  */
#line 304 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1933 "y.tab.c"
    break;

  case 44: /* e: e '-' e  */
#line 305 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1939 "y.tab.c"
    break;

  case 45: /* e: e '*' e  */
#line 306 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1945 "y.tab.c"
    break;

  case 46: /* e: e '/' e  */
#line 307 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1951 "y.tab.c"
    break;

  case 47: /* e: e '%' e  */
#line 308 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1957 "y.tab.c"
    break;

  case 48: /* e: e '^' e  */
#line 309 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0; }
#line 1963 "y.tab.c"
    break;

  case 49: /* e: NAME '(' optargs ')'  */
#line 310 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {
        if((ExprSpecvsp[-1].n) && strcmp((ExprSpecvsp[-3].s),"curve")==0){
            (ExprSpecval.n)=remember(new ExprSpecCurveNode((ExprSpecvsp[-1].n)));
        }else if((ExprSpecvsp[-1].n) && strcmp((ExprSpecvsp[-3].s),"ccurve")==0){
            (ExprSpecval.n)=remember(new ExprSpecCCurveNode((ExprSpecvsp[-1].n)));
        }else if((ExprSpecvsp[-1].n) && strcmp((ExprSpecvsp[-3].s),"swatch")==0){
            (ExprSpecval.n)=remember(new ExprSpecColorSwatchNode((ExprSpecvsp[-1].n)));
        }else if((ExprSpecvsp[-1].n)){
            // function arguments not parse of curve, ccurve, or animCurve
            // check if there are any string args that need to be made into controls
            // but be sure to return 0 as this parseable
            if(ExprSpecListNode* list=dynamic_cast<ExprSpecListNode*>((ExprSpecvsp[-1].n))){
                for(size_t i=0;i<list->nodes.size();i++){
                    if(ExprSpecStringNode* str=dynamic_cast<ExprSpecStringNode*>(list->nodes[i])){
                        specRegisterEditable("<UNKNOWN>",str);
                    }
                }
            }
            (ExprSpecval.n)=0;
        }else (ExprSpecval.n)=0;
      }
#line 1989 "y.tab.c"
    break;

  case 50: /* e: e ARROW NAME '(' optargs ')'  */
#line 331 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                  {(ExprSpecval.n) = 0; }
#line 1995 "y.tab.c"
    break;

  case 51: /* e: VAR  */
#line 332 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 2001 "y.tab.c"
    break;

  case 52: /* e: NAME  */
#line 333 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                {  (ExprSpecval.n) = 0; }
#line 2007 "y.tab.c"
    break;

  case 53: /* e: NUMBER  */
#line 334 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n)=remember(new ExprSpecScalarNode((ExprSpecloc).first_column,(ExprSpecloc).last_column,(ExprSpecvsp[0].d))); }
#line 2013 "y.tab.c"
    break;

  case 54: /* e: STR  */
#line 335 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                    { (ExprSpecval.n) = remember(new ExprSpecStringNode((ExprSpecloc).first_column,(ExprSpecloc).last_column,(ExprSpecvsp[0].s))); }
#line 2019 "y.tab.c"
    break;

  case 55: /* optargs: %empty  */
#line 340 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = 0;}
#line 2025 "y.tab.c"
    break;

  case 56: /* optargs: args  */
#line 341 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = (ExprSpecvsp[0].n);}
#line 2031 "y.tab.c"
    break;

  case 57: /* args: arg  */
#line 346 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
        {
       // ignore first argument unless it is a string (because we parse strings in weird ways)
       ExprSpecListNode* list=new ExprSpecListNode((ExprSpecloc).last_column,(ExprSpecloc).last_column);
       if((ExprSpecvsp[0].n) && SPEC_IS_STR((ExprSpecvsp[0].n))){
           list->add((ExprSpecvsp[0].n));
       }
       remember(list);
       (ExprSpecval.n)=list;
   }
#line 2045 "y.tab.c"
    break;

  case 58: /* args: args ',' arg  */
#line 355 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                 {

      if((ExprSpecvsp[-2].n) && (ExprSpecvsp[0].n) && ((SPEC_IS_NUMBER((ExprSpecvsp[0].n)) || SPEC_IS_VECTOR((ExprSpecvsp[0].n)) || SPEC_IS_STR((ExprSpecvsp[0].n))))){
          (ExprSpecval.n)=(ExprSpecvsp[-2].n);
          dynamic_cast<ExprSpecListNode*>((ExprSpecvsp[-2].n))->add((ExprSpecvsp[0].n));
      }else{
          (ExprSpecval.n)=0;
      }
    }
#line 2059 "y.tab.c"
    break;

  case 59: /* arg: e  */
#line 367 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"
                                { (ExprSpecval.n) = (ExprSpecvsp[0].n);}
#line 2065 "y.tab.c"
    break;


#line 2069 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter ExprSpecchar, and that requires
     that ExprSpectoken be updated with the new translation.  We take the
     approach of translating immediately before every use of ExprSpectoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     ExprSpecYYABORT, ExprSpecYYACCEPT, or ExprSpecYYERROR immediately after altering ExprSpecchar or
     if it invokes ExprSpecYYBACKUP.  In the case of ExprSpecYYABORT or ExprSpecYYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of ExprSpecYYERROR or ExprSpecYYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  ExprSpecYY_SYMBOL_PRINT ("-> $$ =", ExprSpecYY_CAST (ExprSpecsymbol_kind_t, ExprSpecr1[ExprSpecn]), &ExprSpecval, &ExprSpecloc);

  ExprSpecYYPOPSTACK (ExprSpeclen);
  ExprSpeclen = 0;

  *++ExprSpecvsp = ExprSpecval;
  *++ExprSpeclsp = ExprSpecloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int ExprSpeclhs = ExprSpecr1[ExprSpecn] - ExprSpecYYNTOKENS;
    const int ExprSpeci = ExprSpecpgoto[ExprSpeclhs] + *ExprSpecssp;
    ExprSpecstate = (0 <= ExprSpeci && ExprSpeci <= ExprSpecYYLAST && ExprSpeccheck[ExprSpeci] == *ExprSpecssp
               ? ExprSpectable[ExprSpeci]
               : ExprSpecdefgoto[ExprSpeclhs]);
  }

  goto ExprSpecnewstate;


/*--------------------------------------.
| ExprSpecerrlab -- here on detecting error.  |
`--------------------------------------*/
ExprSpecerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  ExprSpectoken = ExprSpecchar == ExprSpecYYEMPTY ? ExprSpecYYSYMBOL_ExprSpecYYEMPTY : ExprSpecYYTRANSLATE (ExprSpecchar);
  /* If not already recovering from an error, report this error.  */
  if (!ExprSpecerrstatus)
    {
      ++ExprSpecnerrs;
      ExprSpecerror (ExprSpecYY_("syntax error"));
    }

  ExprSpecerror_range[1] = ExprSpeclloc;
  if (ExprSpecerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (ExprSpecchar <= ExprSpecYYEOF)
        {
          /* Return failure if at end of input.  */
          if (ExprSpecchar == ExprSpecYYEOF)
            ExprSpecYYABORT;
        }
      else
        {
          ExprSpecdestruct ("Error: discarding",
                      ExprSpectoken, &ExprSpeclval, &ExprSpeclloc);
          ExprSpecchar = ExprSpecYYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto ExprSpecerrlab1;


/*---------------------------------------------------.
| ExprSpecerrorlab -- error raised explicitly by ExprSpecYYERROR.  |
`---------------------------------------------------*/
ExprSpecerrorlab:
  /* Pacify compilers when the user code never invokes ExprSpecYYERROR and the
     label ExprSpecerrorlab therefore never appears in user code.  */
  if (0)
    ExprSpecYYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this ExprSpecYYERROR.  */
  ExprSpecYYPOPSTACK (ExprSpeclen);
  ExprSpeclen = 0;
  ExprSpecYY_STACK_PRINT (ExprSpecss, ExprSpecssp);
  ExprSpecstate = *ExprSpecssp;
  goto ExprSpecerrlab1;


/*-------------------------------------------------------------.
| ExprSpecerrlab1 -- common code for both syntax error and ExprSpecYYERROR.  |
`-------------------------------------------------------------*/
ExprSpecerrlab1:
  ExprSpecerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      ExprSpecn = ExprSpecpact[ExprSpecstate];
      if (!ExprSpecpact_value_is_default (ExprSpecn))
        {
          ExprSpecn += ExprSpecYYSYMBOL_ExprSpecYYerror;
          if (0 <= ExprSpecn && ExprSpecn <= ExprSpecYYLAST && ExprSpeccheck[ExprSpecn] == ExprSpecYYSYMBOL_ExprSpecYYerror)
            {
              ExprSpecn = ExprSpectable[ExprSpecn];
              if (0 < ExprSpecn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (ExprSpecssp == ExprSpecss)
        ExprSpecYYABORT;

      ExprSpecerror_range[1] = *ExprSpeclsp;
      ExprSpecdestruct ("Error: popping",
                  ExprSpecYY_ACCESSING_SYMBOL (ExprSpecstate), ExprSpecvsp, ExprSpeclsp);
      ExprSpecYYPOPSTACK (1);
      ExprSpecstate = *ExprSpecssp;
      ExprSpecYY_STACK_PRINT (ExprSpecss, ExprSpecssp);
    }

  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++ExprSpecvsp = ExprSpeclval;
  ExprSpecYY_IGNORE_MAYBE_UNINITIALIZED_END

  ExprSpecerror_range[2] = ExprSpeclloc;
  ++ExprSpeclsp;
  ExprSpecYYLLOC_DEFAULT (*ExprSpeclsp, ExprSpecerror_range, 2);

  /* Shift the error token.  */
  ExprSpecYY_SYMBOL_PRINT ("Shifting", ExprSpecYY_ACCESSING_SYMBOL (ExprSpecn), ExprSpecvsp, ExprSpeclsp);

  ExprSpecstate = ExprSpecn;
  goto ExprSpecnewstate;


/*-------------------------------------.
| ExprSpecacceptlab -- ExprSpecYYACCEPT comes here.  |
`-------------------------------------*/
ExprSpecacceptlab:
  ExprSpecresult = 0;
  goto ExprSpecreturn;


/*-----------------------------------.
| ExprSpecabortlab -- ExprSpecYYABORT comes here.  |
`-----------------------------------*/
ExprSpecabortlab:
  ExprSpecresult = 1;
  goto ExprSpecreturn;


#if !defined ExprSpecoverflow
/*-------------------------------------------------.
| ExprSpecexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
ExprSpecexhaustedlab:
  ExprSpecerror (ExprSpecYY_("memory exhausted"));
  ExprSpecresult = 2;
  goto ExprSpecreturn;
#endif


/*-------------------------------------------------------.
| ExprSpecreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
ExprSpecreturn:
  if (ExprSpecchar != ExprSpecYYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      ExprSpectoken = ExprSpecYYTRANSLATE (ExprSpecchar);
      ExprSpecdestruct ("Cleanup: discarding lookahead",
                  ExprSpectoken, &ExprSpeclval, &ExprSpeclloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this ExprSpecYYABORT or ExprSpecYYACCEPT.  */
  ExprSpecYYPOPSTACK (ExprSpeclen);
  ExprSpecYY_STACK_PRINT (ExprSpecss, ExprSpecssp);
  while (ExprSpecssp != ExprSpecss)
    {
      ExprSpecdestruct ("Cleanup: popping",
                  ExprSpecYY_ACCESSING_SYMBOL (+*ExprSpecssp), ExprSpecvsp, ExprSpeclsp);
      ExprSpecYYPOPSTACK (1);
    }
#ifndef ExprSpecoverflow
  if (ExprSpecss != ExprSpecssa)
    ExprSpecYYSTACK_FREE (ExprSpecss);
#endif

  return ExprSpecresult;
}

#line 370 "@@PATH@@/src/KSeExprUI/ExprSpecParser.y"


/* ExprSpecerror - Report an error.  This is called by the parser.
(Note: the "msg" param is useless as it is usually just "sparse error".
so it's ignored.)
*/
static void ExprSpecerror(const char* /*msg*/)
{
    // find start of line containing error
    int pos = ExprSpecpos(), lineno = 1, start = 0, end = strlen(ParseStr);
    bool multiline = 0;
    for (int i = start; i < pos; i++)
	if (ParseStr[i] == '\n') { start = i + 1; lineno++; multiline=1; }

    // find end of line containing error
    for (int i = end; i > pos; i--)
	if (ParseStr[i] == '\n') { end = i - 1; multiline=1; }

    ParseError = ExprSpectext[0] ? "Syntax error" : "Unexpected end of expression";
    if (multiline) {
	char buff[30];
	snprintf(buff, 30, " at line %d", lineno);
	ParseError += buff;
    }
    if (ExprSpectext[0]) {
	ParseError += " near '";
	ParseError += ExprSpectext;
    }
    ParseError += "':\n    ";

    int s = std::max(start, pos-30);
    int e = std::min(end, pos+30);

    if (s != start) ParseError += "...";
    ParseError += std::string(ParseStr, s, e-s+1);
    if (e != end) ParseError += "...";
}

namespace KSeExpr {
extern void specResetCounters(std::vector<std::pair<int,int> >& comments);
}


/* CallParser - This is our entrypoint from the rest of the expr library. 
   A string is passed in and a parse tree is returned.	If the tree is null,
   an error string is returned.  Any flags set during parsing are passed
   along.
 */

static std::mutex mutex;

/// Main entry point to parser
bool ExprSpecParse(std::vector<Editable*>& outputEditables,
    std::vector<std::string>& outputVariables,
    std::vector<std::pair<int,int> >& comments,
    const char* str)
{
    std::lock_guard<std::mutex> locker(mutex);

    /// Make inputs/outputs accessible to parser actions
    editables=&outputEditables;
    variables=&outputVariables;
    ParseStr=str;

    // setup and startup parser
    KSeExpr::specResetCounters(comments); // reset lineNumber and columnNumber in scanner
    ExprSpec_buffer_state* buffer = ExprSpec_scan_string(str); // setup lexer
    ParseResult = 0;
    int resultCode = ExprSpecparse(); // parser (don't care if it is a parse error)
    UNUSED(resultCode);
    ExprSpec_delete_buffer(buffer);

    // delete temporary data -- specs(mini parse tree) and tokens(strings)!
    for(size_t i=0;i<specNodes.size();i++) delete specNodes[i];
    specNodes.clear();
    for(size_t i=0;i<tokens.size();i++) free(tokens[i]);
    tokens.clear();
    return true;
}

