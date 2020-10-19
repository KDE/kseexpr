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
   especially those whose name start with ExprSpecYY_ or ExprSpec_.  They are
   private implementation details that can be changed or removed.  */

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
#line 172 "@@PATH@@src/KSeExprUI/ExprSpecParser.y"

    ExprSpecNode* n;
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: UNLIKE the regular parser, this is not strdup()'dthe string */

#line 92 "y.tab.h"

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
