
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

#define NONE -1
typedef enum {
  PROGRAM           ,//0
  SCOPE             ,
  DECLARATIONS      ,
  DECLARATION       ,
  CONST_DECLARATION ,
  STATEMENTS        ,//5

  NESTED_EXP        ,
  UNARY_EXP         ,
  BINARY_EXP        ,

  INT_NODE          ,
  FLOAT_NODE        ,//10
  BOOL_NODE         ,
  IVEC_NODE         ,
  VEC_NODE          ,
  BVEC_NODE         ,

  VAR_NODE          ,//15
  ARRAY_NODE        ,

  FUNCTION          ,
  CONSTRUCTOR       ,
  MULTI_ARG         ,
  ARGUMENTS_EXP     ,//20

  IF_ELSE_STMT      ,
  IF_STMT           ,
//WHILE_STMT_NODE   ,
  ASSIGN_STMT       ,
  NESTED_SCOPE      ,//24
  VAR_TO_EXP        ,//to handle: <type> <var1> = <var2>; cases
  TYPE_NODE
} node_kind;

union token_val{
    int i_val;//value stored in INT_C value
    float f_val;//value stored in FLOAT_C value
    bool b_val;//value stored in BOOL_C value
    char *id;//to store ID string
};


typedef struct node {
    int token;
    int typ;
    int index;
    union token_val val;
    int node_kind;//refers to node_kind enums
    struct node* first;//left
    struct node* second;//middle
    struct node* third;//right
}my_node;

extern node *ast;
extern int parser_idx;

extern const char* unary_op_array[2];
extern const char* binary_op_array[13];
extern const char* type_array[13];
extern const char* func_array[3];
extern const char* nd_kind_array[27];

struct node *ast_allocate(int lex_token, int kind,struct node* f, struct node* s, struct node* t);
void ast_free(node *ast);
void ast_print(node * ast);
char* get_type(int type);

#endif /* AST_H_ */
