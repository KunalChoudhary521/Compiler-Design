%{
/***********************************************************************
 * --YOUR GROUP INFO SHOULD GO HERE--
 *
 *  KUNAL CHOUDHARY - kunal.choudhary@mail.utoronto.ca - 0999981863
 *  BOYOWA DAVID OGBEIDE - boyowa.ogbeide@mail.utoronto.ca - 0999644693
 *   Interface to the parser module for CSC467 course project.
 *
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
//#include "ast.h"
//#include "symbol.h"
//#include "semantic.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */

enum {
  DP3 = 0,
  LIT = 1,
  RSQ = 2
};

%}

/***********************************************************************
 *  Yacc/Bison declarations.
 *  Phase 2:
 *    1. Add precedence declarations for operators (after %start declaration)
 *    2. If necessary, add %type declarations for some nonterminals
 *  Phase 3:
 *    1. Add fields to the union below to facilitate the construction of the
 *       AST (the two existing fields allow the lexical analyzer to pass back
 *       semantic info, so they shouldn't be touched).
 *    2. Add <type> modifiers to appropriate %token declarations (using the
 *       fields of the union) so that semantic information can by passed back
 *       by the scanner.
 *    3. Make the %type declarations for the language non-terminals, utilizing
 *       the fields of the union as well.
 ***********************************************************************/

%{
#define YYDEBUG 1
%}

// defines the yyval union
%union {
  int as_int;
  int as_vec;
  float as_float;
  char *as_str;
  int as_func;
}

%token          FLOAT_T
%token          INT_T
%token          BOOL_T
%token          CONST
%token          FALSE_C TRUE_C
//%token          FUNC
%token          IF WHILE ELSE
%token          AND OR NEQ EQ LEQ GEQ

// links specific values of tokens to yyval
%token <as_vec>   VEC_T
%token <as_vec>   BVEC_T
%token <as_vec>   IVEC_T
%token <as_float> FLOAT_C
%token <as_int>   INT_C
%token <as_str>   ID
%token <as_func>  FUNC

%left OR
%left AND
%left EQ NEQ '<' LEQ '>' GEQ
%left '+' '-'
%left '*' '/'
%right  '^'
%left UMINUS '!'
%left '[' ']' '(' ')'

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler

 priority is from bottom-to-top (top has the least priority)
 ***********************************************************************/
program
  :   scope {yTRACE("program -> scope");}
  ;

scope
  :  '{' declarations statements '}' {yTRACE("scope -> { declarations statements }");}
  ;

declarations
  : declarations declaration {yTRACE("declaration -> declarations declaration");}
  | /*epsilon*/ {yTRACE("declaration -> EPSILON");}
  ;

statements
  : statements statement {yTRACE("statements -> statements statement");}
  | /*epsilon*/ {yTRACE("statements -> EPSILON");}
  ;

declaration
  : type ID ';' {yTRACE("declaration -> type ID ;");}
  | type ID '=' expression ';' {yTRACE("declaration -> type ID = expression ;");}
  | CONST type ID '=' expression ';' {yTRACE("declaration -> const type ID = expression ;");}
  //| /*epsilon*/ {yTRACE("declaration -> EPSILON");}
  ;

statement
  : variable '=' expression ';' {yTRACE("statement -> variable = expression ;");}
  | IF '(' expression ')' statement else_statement {yTRACE("statement -> IF ( expression ) statement else_statement");}
  | WHILE '(' expression ')' statement {yTRACE("statement -> WHILE ( expression ) statement");}
  | scope {yTRACE("statement -> scope");}
  | ';' {yTRACE("statement -> ;");}
  ;

else_statement
  : ELSE statement {yTRACE("else_statement -> ELSE statement");}
  | /*epsilon*/ {yTRACE("else_statement -> EPSILON");}
  ;

type
  : INT_T {yTRACE("type -> INT_T");}
  | IVEC_T { char ivecX[16];
             sprintf(ivecX, "type -> IVEC%d_T", $1+1);
             yTRACE(ivecX);
           }
  | BOOL_T {yTRACE("type -> BOOL_T");}
  | BVEC_T { char bvecX[16];
             sprintf(bvecX, "type -> BVEC%d", $1+1);
             yTRACE(bvecX);
           }
  | FLOAT_T {yTRACE("type -> FLOAT_T");}
  | VEC_T { char vecX[16];
             sprintf(vecX, "type -> VEC%d_T", $1+1);
             yTRACE(vecX);
           }
  ;

expression
  : type '(' arguments ')' {yTRACE("constructor -> ( arguments )");}
  | function {yTRACE("expression -> function");}
  | variable {yTRACE("expression -> variable");}
  | '!' expression {yTRACE("expression -> ! expression");}
  | '-' expression %prec UMINUS {yTRACE("expression -> - expression");}

  | expression AND expression {yTRACE("expression -> expression AND expression");}
  | expression OR expression {yTRACE("expression -> expression OR expression");}
  | expression EQ expression {yTRACE("expression -> expression EQ expression");}
  | expression NEQ expression {yTRACE("expression -> expression NEQ expression");}
  | expression '<' expression {yTRACE("expression -> expression < expression");}
  | expression LEQ expression {yTRACE("expression -> expression LEQ expression");}
  | expression '>' expression {yTRACE("expression -> expression > expression");}
  | expression GEQ expression {yTRACE("expression -> expression GEQ expression");}
  | expression '+' expression {yTRACE("expression -> expression + expression");}
  | expression '-' expression {yTRACE("expression -> expression - expression");}
  | expression '*' expression {yTRACE("expression -> expression * expression");}
  | expression '/' expression {yTRACE("expression -> expression / expression");}
  | expression '^' expression {yTRACE("expression -> expression ^ expression");}

  | INT_C {yTRACE("expression -> INT_C");/*in scanner.l, INT_C is defined as int literal;*/}
  | FLOAT_C {yTRACE("expression -> FLOAT_C");}
  | TRUE_C {yTRACE("expression -> TRUE_C");}
  | FALSE_C {yTRACE("expression -> FALSE_C");}
  | '(' expression ')' {yTRACE("expression -> ( expression )");}
  ;

variable
  : ID {yTRACE("variable -> ID");}
  | ID '['INT_C']' {yTRACE("variable -> ID [ INT_C ]");}
  ;

function
  :   FUNC '(' arguments_opt ')'
      {
          if($1 == 0){ yTRACE("function -> dp3 ( arguments_opt )"); }
          else if($1 == 1){ yTRACE("function -> lit ( arguments_opt )"); }
          else if($1 == 2){ yTRACE("function -> rsq ( arguments_opt )"); }
      }

  ;

arguments_opt
  : arguments {yTRACE("arguments_opt -> arguments");}
  | /*epsilon*/ {yTRACE("arguments_opt -> EPSILON");}
  ;

arguments
  : arguments ',' expression {yTRACE("arguments -> arguments , expression");}
  | expression {yTRACE("arguments -> expression");}
  ;
%%

/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(const char* s) {
  if(errorOccurred) {
    return;    /* Error has already been reported by scanner */
  } else {
    errorOccurred = 1;
  }

  fprintf(errorFile, "\nPARSER ERROR, LINE %d", yyline);

  if(strcmp(s, "parse error")) {
    if(strncmp(s, "parse error, ", 13)) {
      fprintf(errorFile, ": %s\n", s);
    } else {
      fprintf(errorFile, ": %s\n", s+13);
    }
  } else {
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
  }
}
