// SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
// SPDX-License-Identifier: LicenseRef-Apache-2.0
// SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
// SPDX-License-Identifier: GPL-3.0-or-later

%{
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
int yylex();
int yypos();
extern int yy_start;
extern char* yytext;
struct yy_buffer_state;
yy_buffer_state* yy_scan_string(const char *str);
void yy_delete_buffer(yy_buffer_state*);

/*******************
 parser declarations
 *******************/

// forward declaration
static void yyerror(const char* msg);

// local data
static const char* ParseStr;    // string being parsed
static KSeExpr::ErrorCode ParseErrorCode;  // error (set from yyerror)
static std::string ParseErrorId; // string that failed parsing (set from yyerror)
static KSeExpr::ExprNode* ParseResult; // must set result here since yyparse can't return it
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
%}

%union {
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
}

%token IF ELSE EXTERN DEF FLOATPOINT STRING
%token <s> NAME VAR STR
%token <d> NUMBER
%token <l> LIFETIME_CONSTANT LIFETIME_UNIFORM LIFETIME_VARYING LIFETIME_ERROR
%token AddEq SubEq MultEq DivEq ExpEq ModEq
%token '(' ')'
%left ARROW
%nonassoc ':'
%nonassoc '?'
%left OR
%left AND
%left EQ NE
%left '<' '>' SEEXPR_LE SEEXPR_GE
%left '+' '-'
%left '*' '/' '%'
%right UNARY '!' '~'
%right '^'
%left '['
%type <t> typeDeclare
%type <l> lifetimeOptional
%type <n> module declarationList declaration typeListOptional typeList formalTypeListOptional formalTypeList
%type <n> block optassigns assigns assign ifthenelse optelse e optargs args arg exprlist

/* Some notes about the parse tree construction:

   Each rule first parses its children and then returns a new node
   that implements the particular rule (an arithmetic op, a function
   call, or whatever).  Sometimes the child node is just passed up (in
   the case of a parenthesized expression or a unary '+' for
   instance).  But in all cases, a rule returns a parse node which
   represents a complete sub-tree.  Finally, the "expr" rule returns
   the root node which represents the completed parse tree.
*/
%%

// TODO: Change grammar to have option to choose to allow variables of the form
//       $foo or foo. Currently we allow either.

/* The root expression rule */
module:
      declarationList block     { ParseResult = $1; ParseResult->setPosition(@$.first_column, @$.last_column);
                                  ParseResult->addChild($2); }
    | block                     { ParseResult = NODE(@$.first_column, @$.last_column, ModuleNode);
                                  ParseResult->addChild($1); }
    ;

declarationList:
      declaration               { $$ = NODE(@$.first_column, @$.last_column, ModuleNode); $$->addChild($1); }
    | declarationList declaration
                                { $$ = $1; $$->setPosition(@$.first_column, @$.last_column);
                                  $$->addChild($2); }
    ;

declaration:
      EXTERN typeDeclare NAME '(' typeListOptional       ')'
                                { KSeExpr::ExprType type = KSeExpr::ExprType($2.type, $2.dim, $2.lifetime);
                                    KSeExpr::ExprPrototypeNode * prototype =
                                        (KSeExpr::ExprPrototypeNode*)NODE2(@$.first_column, @$.last_column, PrototypeNode, $3, type);
                                  prototype->addArgTypes($5);
                                  Forget($5);
                                  $$ = prototype;
                                  free($3); }
    | DEF    typeDeclare NAME '(' formalTypeListOptional ')' '{' block '}'
                                { KSeExpr::ExprType type = KSeExpr::ExprType($2.type, $2.dim, $2.lifetime);
                                  KSeExpr::ExprPrototypeNode * prototype =
                                      (KSeExpr::ExprPrototypeNode*)NODE2(@$.first_column, @6.last_column, PrototypeNode, $3, type);
                                  prototype->addArgs($5);
                                  Forget($5);
                                  $$ = NODE2(@$.first_column, @$.last_column, LocalFunctionNode, prototype, $8);
                                  free($3); }
    | DEF                NAME '(' formalTypeListOptional ')' '{' block '}'
                                { KSeExpr::ExprPrototypeNode * prototype =
                                        (KSeExpr::ExprPrototypeNode*)NODE1(@$.first_column, @5.last_column, PrototypeNode, $2);
                                  prototype->addArgs($4);
                                  Forget($4);
                                  $$ = NODE2(@$.first_column, @$.last_column, LocalFunctionNode, prototype, $7);
                                  free($2); }
    ;

lifetimeOptional:
      /* empty */               { $$ = KSeExpr::ExprType::ltVARYING; }
    | LIFETIME_CONSTANT         { $$ = KSeExpr::ExprType::ltCONSTANT; }
    | LIFETIME_UNIFORM          { $$ = KSeExpr::ExprType::ltUNIFORM; }
    | LIFETIME_VARYING          { $$ = KSeExpr::ExprType::ltVARYING; }
    | LIFETIME_ERROR            { $$ = KSeExpr::ExprType::ltERROR; } //For testing purposes only
    ;

typeDeclare:
      FLOATPOINT lifetimeOptional{$$.type     = KSeExpr::ExprType::tFP;
                                  $$.dim      = 1;
                                  $$.lifetime = $2; }
    | FLOATPOINT '[' NUMBER ']' lifetimeOptional
                                { $$.type = ($3 > 0 ? KSeExpr::ExprType::tFP : KSeExpr::ExprType::tERROR);
                                  //TODO: This causes an error but does not report it to user. Change this.
                                  $$.dim  = ($3 > 0 ? $3 : 0);
                                  $$.lifetime = $5; }
    | STRING lifetimeOptional   { $$.type = KSeExpr::ExprType::tSTRING;
                                  $$.dim  = 1;
                                  $$.lifetime = $2; }
    ;

typeListOptional:
      /* empty */               { $$ = NODE(@$.first_column, @$.last_column, Node); }
    | typeList                  { $$ = $1; }
    ;

typeList:
      typeDeclare               { $$ = NODE(@$.first_column, @$.last_column, Node);
                                  KSeExpr::ExprType type = KSeExpr::ExprType($1.type, $1.dim, $1.lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2(@$.first_column, @$.last_column, VarNode, "", type);
                                  $$->addChild(varNode); }
    | typeList ',' typeDeclare  { $$ = $1;
                                  KSeExpr::ExprType type = KSeExpr::ExprType($3.type, $3.dim, $3.lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2(@3.first_column, @3.last_column, VarNode, "", type);
                                  $$->addChild(varNode); }
    ;

formalTypeListOptional:
      /* empty */               { $$ = NODE(@$.first_column, @$.last_column, Node); }
    | formalTypeList            { $$ = $1; }
    ;

formalTypeList:
      typeDeclare NAME           { $$ = NODE(@$.first_column, @$.last_column, Node);
                                  KSeExpr::ExprType type = KSeExpr::ExprType($1.type, $1.dim, $1.lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2(@$.first_column, @$.last_column, VarNode, $2, type);
                                  $$->addChild(varNode);
                                  free($2); }
    | formalTypeList ',' typeDeclare NAME
                                { $$ = $1;
                                  KSeExpr::ExprType type = KSeExpr::ExprType($3.type, $3.dim, $3.lifetime);
                                  KSeExpr::ExprNode* varNode = NODE2(@3.first_column, @4.last_column, VarNode, $4, type);
                                  $$->addChild(varNode);
                                  free($4); }
    ;

block:
      assigns e                 { $$ = NODE2(@$.first_column,@$.last_column,BlockNode, $1, $2); }
    | e                         { $$ = $1; }
    ;

/* local variable assignments */
optassigns:
      /* empty */		{ $$ = NODE(@$.first_column,@$.last_column,Node); /* create empty node */; }
    | assigns			{ $$ = $1; }
    ;

assigns:
      assign  		        { $$ = NODE1(@$.first_column,@$.last_column,Node, $1); /* create var list */}
    | assigns assign    	{ $$ = $1; $1->addChild($2); /* add to list */}
    ;

assign:
      ifthenelse		{ $$ = $1; }
    | VAR '=' e ';'		{ $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, $3); free($1); }
    | VAR AddEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'+');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | VAR SubEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'-');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | VAR MultEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'*');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | VAR DivEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'/');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | VAR ExpEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'^');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | VAR ModEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'%');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME '=' e ';'		{ $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, $3); free($1); }
    | NAME AddEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'+');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME SubEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'-');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME MultEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'*');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME DivEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'/');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME ExpEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'^');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    | NAME ModEq e ';'              {KSeExpr::ExprNode* varNode=NODE1(@1.first_column,@1.first_column,VarNode, $1);
                               KSeExpr::ExprNode* opNode=NODE3(@3.first_column,@3.first_column,BinaryOpNode,varNode,$3,'%');
                                $$ = NODE2(@$.first_column,@$.last_column,AssignNode, $1, opNode);free($1);}
    ;

ifthenelse:
      IF '(' e ')' '{' optassigns '}' optelse
				{ $$ = NODE3(@$.first_column,@$.last_column,IfThenElseNode, $3, $6, $8); }
    ;

optelse:
      /* empty */		{ $$ = NODE(@$.first_column,@$.last_column,Node); /* create empty node */ }
    | ELSE '{' optassigns '}'   { $$ = $3; }
    | ELSE ifthenelse		{ $$ = $2; }
    ;

/* An expression or sub-expression */
e:
      '(' e ')'			{ $$ = $2; }
    | '[' exprlist ']'          { KSeExpr::ExprNode* newNode = NODE(@$.first_column,@$.last_column,VecNode); newNode->addChildren($2); Forget($2); $$=newNode;}
    | e '[' e ']'               { $$ = NODE2(@$.first_column,@$.last_column,SubscriptNode, $1, $3); }
    | e '?' e ':' e		{ $$ = NODE3(@$.first_column,@$.last_column,CondNode, $1, $3, $5); }
    | e OR e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3, '|'); }
    | e AND e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3, '&'); }
    | e EQ e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareEqNode, $1, $3,'='); }
    | e NE e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareEqNode, $1, $3,'!'); }
    | e '<' e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3,'<'); }
    | e '>' e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3,'>'); }
    | e SEEXPR_LE e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3,'l'); }
    | e SEEXPR_GE e			{ $$ = NODE3(@$.first_column,@$.last_column,CompareNode, $1, $3,'g'); }
    | '+' e %prec UNARY		{ $$ = $2; }
    | '-' e %prec UNARY		{ $$ = NODE2(@$.first_column,@$.last_column,UnaryOpNode, $2, '-'); }
    | '!' e			{ $$ = NODE2(@$.first_column,@$.last_column,UnaryOpNode, $2, '!'); }
    | '~' e			{ $$ = NODE2(@$.first_column,@$.last_column,UnaryOpNode, $2, '~'); }
    | e '+' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '+'); }
    | e '-' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '-'); }
    | e '*' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '*'); }
    | e '/' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '/'); }
    | e '%' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '%'); }
    | e '^' e			{ $$ = NODE3(@$.first_column,@$.last_column,BinaryOpNode, $1, $3, '^'); }
    | NAME '(' optargs ')'	{ $$ = NODE1(@$.first_column,@$.last_column,FuncNode, $1);
				  free($1); // free name string
				  // add args directly and discard arg list node
				  $$->addChildren($3); Forget($3); }
    | e ARROW NAME '(' optargs ')'
    				{ $$ = NODE1(@$.first_column,@$.last_column,FuncNode, $3);
				  free($3); // free name string
				  $$->addChild($1);
				  // add args directly and discard arg list node
				  $$->addChildren($5); Forget($5); }
    | VAR			{ $$ = NODE1(@$.first_column,@$.last_column,VarNode, $1); free($1); /* free name string */ }
    | NAME			{ $$ = NODE1(@$.first_column,@$.last_column,VarNode, $1); free($1); /* free name string */ }
    | NUMBER			{ $$ = NODE1(@$.first_column,@$.last_column,NumNode, $1); /*printf("line %d",@$.last_column);*/}
    | STR			{ $$ = NODE1(@$.first_column,@$.last_column,StrNode, $1); free($1); /* free string */}
    ;

exprlist:
      e                         { $$ = NODE1(@$.first_column,@$.last_column,Node,$1); }
    | exprlist ',' e            { $$ = $1;
                                  $1->addChild($3); }
    ;

/* An optional argument list */
optargs:
      /* empty */		{ $$ = NODE(@$.first_column,@$.last_column,Node); /* create empty node */}
    | args			{ $$ = $1; }
    ;

/* Argument list (comma-separated expression list) */
args:
      arg			{ $$ = NODE1(@$.first_column,@$.last_column,Node, $1); /* create arg list */}
    | args ',' arg		{ $$ = $1; $1->addChild($3); /* add to list */}
    ;

arg:
      e				{ $$ = $1; }
    ;

%%

      /* yyerror - Report an error.  This is called by the parser.
	 (Note: the "msg" param is useless as it is usually just "parse error".
	 so it's ignored.)
      */
static void yyerror(const char* /*msg*/)
{
    // find start of line containing error
    int pos = yypos(), lineno = 1, start = 0, end = strlen(ParseStr);
    bool multiline = 0;
    for (int i = start; i < pos; i++)
	if (ParseStr[i] == '\n') { start = i + 1; lineno++; multiline=1; }

    // find end of line containing error
    for (int i = end; i > pos; i--)
	if (ParseStr[i] == '\n') { end = i - 1; multiline=1; }

    ParseErrorCode = yytext[0] ? KSeExpr::ErrorCode::SyntaxError : KSeExpr::ErrorCode::UnexpectedEndOfExpression;

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
    yy_buffer_state* buffer = yy_scan_string(str);
    ParseResult = 0;
    int resultCode = yyparse();
    yy_delete_buffer(buffer);

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
        errorStart=yylloc.first_column;
        errorEnd=yylloc.last_column;
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
