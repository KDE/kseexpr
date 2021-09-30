/**
 * SPDX-FileCopyrightText: 2011-2019 Disney Enterprises, Inc.
 * SPDX-License-Identifier: LicenseRef-Apache-2.0
 * SPDX-FileCopyrightText: 2020 L. E. Segovia <amy@amyspark.me>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

%{
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
int yylex();
int yypos();
extern int yy_start;
extern char* yytext;
struct yy_buffer_state;
yy_buffer_state* yy_scan_string(const char *str);
void yy_delete_buffer(yy_buffer_state*);

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
static std::string ParseError;  // error (set from yyerror)
static ExprSpecNode* ParseResult; // must set result here since yyparse can't return it


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
static void yyerror(const char* msg);

%}

%union {
    ExprSpecNode* n;
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: UNLIKE the regular parser, this is not strdup()'dthe string */
}

%token IF ELSE
%token <s> NAME VAR STR
%token <d> NUMBER
%token AddEq SubEq MultEq DivEq ExpEq ModEq
%token '(' ')'
%left ARROW
%nonassoc ':'
%nonassoc '?'
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right UNARY '!' '~'
%right '^'
%left '['
%type <n> optassigns assigns assign ifthenelse optelse e optargs args arg

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
expr:
      assigns e                 { ParseResult = 0; }
    | e                         { ParseResult = 0; }
    ;

/* local variable assignments */
optassigns:
      /* empty */		{ $$ = 0; }
    | assigns			{ $$ = 0; }
    ;

assigns:
      assign			{ $$ = 0; }
    | assigns assign		{ $$ = 0; }
    ;


assign:
      ifthenelse		{ $$ = 0; }
    | VAR '=' e ';'		{
        specRegisterVariable($1);
        specRegisterEditable($1,$3);
      }
    | VAR AddEq e ';'           { $$ = 0; }
    | VAR SubEq e ';'           { $$ = 0; }
    | VAR MultEq e ';'          { $$ = 0; }
    | VAR DivEq e ';'           { $$ = 0; }
    | VAR ExpEq e ';'           { $$ = 0; }
    | VAR ModEq e ';'           { $$ = 0; }
    | NAME '=' e ';'		{
        specRegisterVariable($1);
        specRegisterEditable($1,$3);
      }
    | NAME AddEq e ';'          {  $$ = 0; }
    | NAME SubEq e ';'          {  $$ = 0; }
    | NAME MultEq e ';'         {  $$ = 0; }
    | NAME DivEq e ';'          {  $$ = 0; }
    | NAME ExpEq e ';'          {  $$ = 0; }
    | NAME ModEq e ';'          {  $$ = 0; }
    ;

ifthenelse:
    IF '(' e ')' '{' optassigns '}' optelse
    { $$ = 0; }
    ;

optelse:
/* empty */		{ $$ = 0; }
| ELSE '{' optassigns '}'   { $$ = 0;}
| ELSE ifthenelse		{ $$ = 0;}
    ;

/* An expression or sub-expression */
e:
      '(' e ')'			{ $$ = 0; }
    | '[' e ',' e ',' e ']'     {
        if(SPEC_IS_NUMBER($2) && SPEC_IS_NUMBER($4) && SPEC_IS_NUMBER($6)){
            $$=remember(new ExprSpecVectorNode(@$.first_column,@$.last_column,$2,$4,$6));
        }else $$=0;
      }
    | e '[' e ']'               { $$ = 0; }
    | e '?' e ':' e		{ $$ = 0; }
    | e OR e			{ $$ = 0; }
    | e AND e			{ $$ = 0; }
    | e EQ e			{ $$ = 0; }
    | e NE e			{ $$ = 0; }
    | e '<' e			{ $$ = 0; }
    | e '>' e			{ $$ = 0; }
    | e LE e			{ $$ = 0; }
    | e GE e			{ $$ = 0; }
    | '+' e %prec UNARY		{ $$ = $2; }
    | '-' e %prec UNARY		{
        if(SPEC_IS_NUMBER($2)){
            ExprSpecScalarNode* node=(ExprSpecScalarNode*)$2;
            node->v*=-1;
            node->startPos=@$.first_column;
            node->endPos=@$.last_column;
            $$=$2;
        }else $$=0;
      }
    | '!' e			{ $$ = 0; }
    | '~' e			{ $$ = 0; }
    | e '+' e			{ $$ = 0; }
    | e '-' e			{ $$ = 0; }
    | e '*' e			{ $$ = 0; }
    | e '/' e			{ $$ = 0; }
    | e '%' e			{ $$ = 0; }
    | e '^' e			{ $$ = 0; }
    | NAME '(' optargs ')'	{
        if($3 && strcmp($1,"curve")==0){
            $$=remember(new ExprSpecCurveNode($3));
        }else if($3 && strcmp($1,"ccurve")==0){
            $$=remember(new ExprSpecCCurveNode($3));
        }else if($3 && strcmp($1,"swatch")==0){
            $$=remember(new ExprSpecColorSwatchNode($3));
        }else if($3){
            // function arguments not parse of curve, ccurve, or animCurve
            // check if there are any string args that need to be made into controls
            // but be sure to return 0 as this parseable
            if(ExprSpecListNode* list=dynamic_cast<ExprSpecListNode*>($3)){
                for(size_t i=0;i<list->nodes.size();i++){
                    if(ExprSpecStringNode* str=dynamic_cast<ExprSpecStringNode*>(list->nodes[i])){
                        specRegisterEditable("<UNKNOWN>",str);
                    }
                }
            }
            $$=0;
        }else $$=0;
      }
    | e ARROW NAME '(' optargs ')'{$$ = 0; }
    | VAR			{  $$ = 0; }
    | NAME			{  $$ = 0; }
    | NUMBER			{ $$=remember(new ExprSpecScalarNode(@$.first_column,@$.last_column,$1)); }
    | STR           { $$ = remember(new ExprSpecStringNode(@$.first_column,@$.last_column,$1)); }
    ;

/* An optional argument list */
optargs:
      /* empty */		{ $$ = 0;}
    | args			{ $$ = $1;}
    ;

/* Argument list (comma-separated expression list) */
args:
   arg	{
       // ignore first argument unless it is a string (because we parse strings in weird ways)
       ExprSpecListNode* list=new ExprSpecListNode(@$.last_column,@$.last_column);
       if($1 && SPEC_IS_STR($1)){
           list->add($1);
       }
       remember(list);
       $$=list;
   }
  | args ',' arg {

      if($1 && $3 && ((SPEC_IS_NUMBER($3) || SPEC_IS_VECTOR($3) || SPEC_IS_STR($3)))){
          $$=$1;
          dynamic_cast<ExprSpecListNode*>($1)->add($3);
      }else{
          $$=0;
      }
    }
    ;

arg:
      e				{ $$ = $1;}
    ;

%%

/* yyerror - Report an error.  This is called by the parser.
(Note: the "msg" param is useless as it is usually just "sparse error".
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

    ParseError = yytext[0] ? "Syntax error" : "Unexpected end of expression";
    if (multiline) {
	char buff[30];
	snprintf(buff, 30, " at line %d", lineno);
	ParseError += buff;
    }
    if (yytext[0]) {
	ParseError += " near '";
	ParseError += yytext;
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
    yy_buffer_state* buffer = yy_scan_string(str); // setup lexer
    ParseResult = 0;
    int resultCode = yyparse(); // parser (don't care if it is a parse error)
    UNUSED(resultCode);
    yy_delete_buffer(buffer);

    // delete temporary data -- specs(mini parse tree) and tokens(strings)!
    for(size_t i=0;i<specNodes.size();i++) delete specNodes[i];
    specNodes.clear();
    for(size_t i=0;i<tokens.size();i++) free(tokens[i]);
    tokens.clear();
    return true;
}

