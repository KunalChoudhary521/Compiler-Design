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
#include "ast.h"
#include "symbol.h"
#include "semantic.h"
#include "parser.tab.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */
int parser_idx;
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
  node *as_ast;
}

%token          FLOAT_T
%token          INT_T
%token          BOOL_T
%token          CONST
%token          FALSE_C TRUE_C
//%token          FUNC
%token          IF ELSE
%token          AND OR NEQ EQ LEQ GEQ

// links specific values of tokens to yyval
%token <as_vec>   VEC_T
%token <as_vec>   BVEC_T
%token <as_vec>   IVEC_T
%token <as_float> FLOAT_C
%token <as_int>   INT_C
%token <as_str>   ID
%token <as_func>  FUNC

// operator precedence
%left     OR                        // 7
%left     AND                       // 6
%left     EQ NEQ '<' LEQ '>' GEQ    // 5
%left     '+' '-'                   // 4
%left     '*' '/'                   // 3
%right    '^'                       // 2
%right    '!' UMINUS                // 1
%left     '(' '['                   // 0

// resolve dangling else shift/reduce conflict with associativity
%left     WITHOUT_ELSE
%left     WITH_ELSE

// type declarations
// TODO: fill this out
%type <as_ast> program
%type <as_ast> scope
%type <as_ast> declarations
%type <as_ast> statements
%type <as_ast> declaration
%type <as_ast> statement
%type <as_ast> type
%type <as_ast> expression
%type <as_ast> variable
%type <as_ast> arguments
%type <as_ast> arguments_opt


// expect one shift/reduce conflict, where Bison chooses to shift
// the ELSE.
%expect 1

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 ***********************************************************************/
program
  : scope
      { yTRACE("program -> scope\n")
        ast = ast_allocate(NONE,PROGRAM,$1,NULL,NULL);
      }
  ;

scope
  : '{' declarations statements '}'
      { yTRACE("scope -> { declarations statements }\n")
        $$ = ast_allocate(NONE,SCOPE, $2, $3, NULL);
      }
  ;

declarations
  : declarations declaration
      { yTRACE("declarations -> declarations declaration\n")
        $$ = ast_allocate(NONE, DECLARATIONS, $1, $2, NULL);
      }
  |
      { yTRACE("declarations -> \n")
        $$ = NULL;
      }
  ;

statements
  : statements statement
      { yTRACE("statements -> statements statement\n")
        $$ = ast_allocate(NONE, STATEMENTS, $1, $2, NULL);
      }
  |
      { yTRACE("statements -> \n")
        $$ = NULL;
      }
  ;

declaration
  : type ID ';'
      { yTRACE("declaration -> type ID ;\n")
        yylval.as_str = $2;
        $$ = ast_allocate(ID, DECLARATION, $1,NULL,NULL);//$2 sends ID's string name
      }
  | type ID '=' expression ';'
      { yTRACE("declaration -> type ID = expression ;\n")        
        yylval.as_str = $2;
        $$ = ast_allocate(ID,DECLARATION, $1,$4,NULL);//$2
      }
  | CONST type ID '=' expression ';'
      { yTRACE("declaration -> CONST type ID = expression ;\n")
        yylval.as_str = $3;
        $$ = ast_allocate(ID,CONST_DECLARATION, $2, $5,NULL);//$3
      }
  ;

statement
  : variable '=' expression ';'
      { yTRACE("statement -> variable = expression ;\n")
        $$ = ast_allocate(NONE,ASSIGN_STMT, $1, $3, NULL);
      }
  | IF '(' expression ')' statement ELSE statement %prec WITH_ELSE
      { yTRACE("statement -> IF ( expression ) statement ELSE statement \n")
        $$ = ast_allocate(NONE,IF_ELSE_STMT, $3, $5, $7);
      }
  | IF '(' expression ')' statement %prec WITHOUT_ELSE
      { yTRACE("statement -> IF ( expression ) statement \n")
        $$ = ast_allocate(NONE,IF_STMT, $3, $5, NULL);
      }
  | scope
      { yTRACE("statement -> scope \n")
        $$ = ast_allocate(NONE,NESTED_SCOPE, $1,NULL,NULL);
      }
  | ';'
      { yTRACE("statement -> ; \n")
        $$ = NULL;
      }
  ;

type
  : INT_T
      { yTRACE("type -> INT_T \n")
        $$ = ast_allocate(INT,TYPE_NODE,NULL,NULL,NULL);
      }
  | IVEC_T
      { yTRACE("type -> IVEC_T \n")
        $$ = ast_allocate($1-1+IVEC2,IVEC_NODE,NULL,NULL,NULL);//detects ivec2, ivec3, ivec4
      }
  | BOOL_T
      { yTRACE("type -> BOOL_T \n")
        $$ = ast_allocate(BOOL,TYPE_NODE,NULL,NULL,NULL);
      }
  | BVEC_T
      { yTRACE("type -> BVEC_T \n")
        $$ = ast_allocate($1-1+BVEC2,BVEC_NODE,NULL,NULL,NULL);
      }
  | FLOAT_T
      { yTRACE("type -> FLOAT_T \n")
        $$ = ast_allocate(FLOAT,TYPE_NODE,NULL,NULL,NULL);
      }
  | VEC_T
      { yTRACE("type -> VEC_T \n")
        $$ = ast_allocate($1-1+VEC2,VEC_NODE,NULL,NULL,NULL);
      }
  ;

expression

  /* function-like operators */
  : type '(' arguments_opt ')' %prec '('
      { yTRACE("expression -> type ( arguments_opt ) \n")
        $$ = ast_allocate(NONE,CONSTRUCTOR, $1, $3,NULL);
      }
  | FUNC '(' arguments_opt ')' %prec '('
      { yTRACE("expression -> FUNC ( arguments_opt ) \n")
        yylval.as_func = $1;
        $$ = ast_allocate(FUNC,FUNCTION, $3,NULL,NULL);//$1 sends the name of the function
      }

  /* unary opterators */
  | '-' expression %prec UMINUS
      { yTRACE("expression -> - expression \n")
        $$ = ast_allocate(MINUS,UNARY_EXP, $2,NULL,NULL);
      }
  | '!' expression %prec '!'
      { yTRACE("expression -> ! expression \n")
        $$ = ast_allocate(NOT,UNARY_EXP, $2,NULL,NULL);
      }

  /* binary operators */
  | expression AND expression %prec AND
      { yTRACE("expression -> expression AND expression \n")
        $$ = ast_allocate(AND_OP, BINARY_EXP, $1, $3,NULL);
      }
  | expression OR expression %prec OR
      { yTRACE("expression -> expression OR expression \n")
        $$ = ast_allocate(OR_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression EQ expression %prec EQ
      { yTRACE("expression -> expression EQ expression \n")
        $$ = ast_allocate(EQ_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression NEQ expression %prec NEQ
      { yTRACE("expression -> expression NEQ expression \n")            
        $$ = ast_allocate(NEQ_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '<' expression %prec '<'
      { yTRACE("expression -> expression < expression \n")
        $$ = ast_allocate(LT_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression LEQ expression %prec LEQ
      { yTRACE("expression -> expression LEQ expression \n")
        $$ = ast_allocate(LEQ_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '>' expression %prec '>'
      { yTRACE("expression -> expression > expression \n")
        $$ = ast_allocate(GT_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression GEQ expression %prec GEQ
      { yTRACE("expression -> expression GEQ expression \n")
        $$ = ast_allocate(GEQ_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '+' expression %prec '+'
      { yTRACE("expression -> expression + expression \n")
        $$ = ast_allocate(ADD_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '-' expression %prec '-'
      { yTRACE("expression -> expression - expression \n")
        $$ = ast_allocate(SUB_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '*' expression %prec '*'
      { yTRACE("expression -> expression * expression \n")
        $$ = ast_allocate(MULT_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '/' expression %prec '/'
      { yTRACE("expression -> expression / expression \n")
        $$ = ast_allocate(DIV_OP,BINARY_EXP, $1, $3, NULL);
      }
  | expression '^' expression %prec '^'
      { yTRACE("expression -> expression ^ expression \n")
        $$ = ast_allocate(POW_OP,BINARY_EXP, $1, $3, NULL);
      }

  /* literals */
  | TRUE_C
      { yTRACE("expression -> TRUE_C \n")
        $$ = ast_allocate(TRUE_C,BOOL_NODE,NULL,NULL,NULL);
      }
  | FALSE_C
      { yTRACE("expression -> FALSE_C \n")
        $$ = ast_allocate(FALSE_C,BOOL_NODE,NULL,NULL,NULL);
      }
  | INT_C
      { yTRACE("expression -> INT_C \n")
        yylval.as_int = $1;
        $$ = ast_allocate(INT_C,INT_NODE,NULL,NULL,NULL);//$1
      }
  | FLOAT_C
      { yTRACE("expression -> FLOAT_C \n")
        yylval.as_float = $1;
        $$ = ast_allocate(FLOAT_C,FLOAT_NODE,NULL,NULL,NULL);//$1
      }

  /* misc */
  | '(' expression ')'
      { yTRACE("expression -> ( expression ) \n")
        $$ = ast_allocate(NONE,NESTED_EXP, $2,NULL,NULL);
      }
  | variable
    { yTRACE("expression -> variable \n")
      $$ = ast_allocate(NONE,VAR_TO_EXP, $1,NULL,NULL);
    }
  ;

variable
  : ID
      { yTRACE("variable -> ID \n")
        yylval.as_str = $1;
        $$ = ast_allocate(ID,VAR_NODE,NULL,NULL,NULL);//$1
      }
  | ID '[' INT_C ']' %prec '['
      { yTRACE("variable -> ID [ INT_C ] \n")
        yylval.as_str = $1;
        parser_idx = $3;
        $$ = ast_allocate(ID,ARRAY_NODE,NULL,NULL,NULL);//$1
      }
  ;

arguments
  : arguments ',' expression
      { yTRACE("arguments -> arguments , expression \n")
        $$ = ast_allocate(NONE,MULTI_ARG, $1, $3,NULL);
      }
  | expression
      { yTRACE("arguments -> expression \n")
        $$ = ast_allocate(NONE,ARGUMENTS_EXP, $1,NULL,NULL);
      }
  ;

arguments_opt
  : arguments
      { yTRACE("arguments_opt -> arguments \n")
        $$ = ast_allocate(NONE,ARGUMENTS_EXP, $1,NULL,NULL);
      }
  |
      { yTRACE("arguments_opt -> \n")
        $$ = NULL;
      }
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
