/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with SeExprYY_ or SeExpr2_.  They are
   private implementation details that can be changed or removed.  */

#ifndef SeExprYY_SEEXPR2_Y_TAB_H_INCLUDED
# define SeExprYY_SEEXPR2_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef SeExprYYDEBUG
# define SeExprYYDEBUG 0
#endif
#if SeExprYYDEBUG
extern int SeExpr2debug;
#endif

/* Token kinds.  */
#ifndef SeExprYYTOKENTYPE
# define SeExprYYTOKENTYPE
  enum SeExpr2tokentype
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
  typedef enum SeExpr2tokentype SeExpr2token_kind_t;
#endif

/* Value type.  */
#if ! defined SeExprYYSTYPE && ! defined SeExprYYSTYPE_IS_DECLARED
union SeExprYYSTYPE
{
#line 78 "/disney/users/jberlin/projects/seexpr2/src/SeExpr2/ExprParser.y"

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

#line 109 "y.tab.h"

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
