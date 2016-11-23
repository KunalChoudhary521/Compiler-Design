
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum{
    INT, IVEC2, IVEC3, IVEC4,
    BOOL, BVEC2, BVEC3, BVEC4,
    FLOAT, VEC2, VEC3, VEC4,
    FUNCT
}types;

typedef enum{
    MINUS, NOT
} unary_ops;

typedef enum{
    AND_OP, OR_OP,
    EQ_OP, NEQ_OP, LT_OP, LEQ_OP, GT_OP, GEQ_OP,
    ADD_OP, SUB_OP, MULT_OP, DIV_OP, POW_OP
} binary_ops;


typedef enum {
  UNKNOWN               ,
  INSIDE_SCOPE          ,
  SCOPE_NODE            ,
  DECLARATIONS_NODE     ,
  STATEMENTS_NODE       ,

  EXPRESSION_NODE       ,//don't use it anyway currently
  NESTED_EXP_NODE       ,
  UNARY_EXPRESION_NODE  ,
  BINARY_EXPRESSION_NODE,

  INT_NODE              ,
  FLOAT_NODE            ,
  BOOL_NODE             ,
  IDENT_NODE            ,
  VAR_NODE              ,
  ARRAY_NODE            ,

  FUNCTION_NODE         ,
  CONSTRUCTOR_NODE      ,
  TYPE_NODE             ,
  MULTI_ARGUMENT_NODE   ,
  ARGUMENTS_EXP_NODE    ,

  IF_ELSE_STATEMENT_NODE,
  IF_STATEMENT_NODE     ,
//  WHILE_STATEMENT_NODE,
  ASSIGNMENT_NODE       ,
  NESTED_SCOPE_NODE     ,

  DECLARATION_NODE      ,
  DECLARATION_ASSIGN_NODE,
  CONST_DECLARATION_NODE,
} node_kind;


struct node_ {

  // an example of tagging each node with a type
  node_kind kind; 

  union {
    struct {
      node *declarations;
      node *statements;
      //int code_line;
    } scope;

    struct {
      node *scope;      
    } inside_scope;

    struct {
      node *declarations;
      node *declaration;
    } declarations;

    struct {
      node *statements;
      node *statement;
    } statements;

    struct {
      int op;
      node *right;
    } unary_expr;

    struct {
      int op;
      node *left;
      node *right;
    } binary_expr;

    struct
    {
      node *left;//variable
      node *right;//expression
    } assignment;

    struct {
      node *condition;
      node *then_st;
    } if_statement;

    struct {
      node *condition;
      node *then_st;
      node *else_st;
    } if_else_statement;

    struct {
      node * type;
      node *arguments;
    } constructor;

    struct {
      int name;//dp3=0, lit=1, rsq=2
      node* arguments;
    } function;

    struct{
      char* id;
    } variable;

    struct {
      char *id;
      int index;
    } array;

    struct {
      node *expression;
    } nested_exp;//,arguments_exp;

    struct {
      node *type;
      char *id;
    } declaration;

    struct {
      node *type;
      char *id;
      node *value;
    } declaration_assignment;//dec_and_assign

    struct {
      node *type;
      char *id;
      node *value;
    } const_declaration;

    struct {
      int name;
    } type;

    struct {
      int right;
    } int_const;

    struct {
      double right;
    }float_const;

    struct {
      int right;
    } bool_const;

    struct {
      node *arguments;
      node *expression;
    }multi_argument;

    struct {
      node *expression;
    }arguments_exp;

  };
};

extern const char* unary_op_array[2];
extern const char* binary_op_array[13];
extern const char* type_array[13];
extern const char* func_array[3];

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);
char* get_type(int type);

#endif /* AST_H_ */
