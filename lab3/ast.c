//top-down (preorder), bottom-up (postorder)
//(./compiler467 -Da) < test_ast.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"
#include "float.h"
#include "limits.h"

#define DEBUG_PRINT_TREE 0

const char* unary_op_array[] = {"MINUS", "NOT"};

const char* binary_op_array[] = {"AND", "OR",
                                "EQ", "NEQ", "LT", "LEQ", "GT", "GEQ",
                                "ADD", "SUB", "MULT", "DIV", "POW_OP"};

const char* type_array[] = { "INT", "IVEC2", "IVEC3", "IVEC4",
                            "BOOL", "BVEC2", "BVEC3", "BVEC4",
                            "FLOAT", "VEC2", "VEC3", "VEC4", "FUNCT" };

const char* func_array[] = {"dp3", "lit", "rsq"};

const char* nd_kind_array[] = {"PROGRAM", "SCOPE", "DECLARATIONS", "DECLARATION",
                               "CONST_DECLARATION","STATEMENTS", "NESTED_EXP",
                               "UNARY_EXP", "BINARY_EXP", "INT_NODE","FLOAT_NODE",
                               "BOOL_NODE", "IVEC_NODE", "VEC_NODE", "BVEC_NODE",
                               "VAR_NODE", "ARRAY_NODE", "FUNCTION", "CONSTRUCTOR",
                               "MULTI_ARG", "ARGUMENTS_EXP", "IF_ELSE_STMT", "IF_STMT",
                               "ASSIGN_STMT","NESTED_SCOPE","VAR_TO_EXP","TYPE_NODE"};


node *ast = NULL;
//AST construction happens bottom-up because the bison operates bottom-up (postorder)
struct node *ast_allocate(int lex_token, int kind,struct node* f, struct node* s, struct node* t)
{
    printf("allocated: %s\n", nd_kind_array[kind]);
    struct node* ast_node;
    ast_node = (struct node*)malloc(sizeof(struct node));
    ast_node->node_kind = kind;
    ast_node->first = f;
    ast_node->second = s;
    ast_node->third = t;

    /*--setting default values--*/
    ast_node->val.b_val = false;
    ast_node->val.f_val = FLT_MAX;
    ast_node->val.i_val = INT_MAX;
    ast_node->val.id = NULL;
    /*------------------------*/

    ast_node->token = lex_token;
    if(lex_token == ID)
    {
        ast_node->val.id = (char*)malloc(sizeof(char) * (strlen(yylval.as_str)+1));//+1 for '\0'
        strcpy(ast_node->val.id,yylval.as_str);
        //printf("ID: %s\n",yylval.as_str);
    }
    else if(lex_token == TRUE_C)
    {
        ast_node->val.b_val = true;
        ast_node->typ = BOOL;
    }
    else if(lex_token == FALSE_C)
    {
        ast_node->val.b_val = false;
        ast_node->typ = BOOL;
    }
    else if(lex_token == INT_C)
    {
        ast_node->val.i_val = yylval.as_int;
        ast_node->typ = INT;
    }
    else if(lex_token == FLOAT_C)
    {
        ast_node->val.f_val = yylval.as_float;
        ast_node->typ = FLOAT;
    }        

    if(kind == IVEC_NODE)
    {
        ast_node->typ = lex_token;//IVEC types-enum
    }
    else if(kind == VEC_NODE)
    {
        ast_node->typ = lex_token;//VEC types-enum
    }
    else if(kind == BVEC_NODE)
    {
        ast_node->typ = lex_token;//BVEC types-enum
    }
    else if(kind == FLOAT_NODE)
    {
        ast_node->typ = FLOAT;//BVEC types-enum
    }
    else if(kind == FUNCTION)
    {
        ast_node->typ = yylval.as_func;
    }
    else if(kind == TYPE_NODE)
    {
        ast_node->typ = lex_token;//INT, FLOAT or BOOL types-enum
    }
    else if(kind == ARRAY_NODE)
    {
        ast_node->index = parser_idx;
    }

    return ast_node;
}

void ast_free(node *ast_node)//needs to be bottom-up (postorder)
{

    if(ast_node == NULL)
    {
        return;
    }
    ast_free(ast_node->first);
    ast_free(ast_node->second);
    ast_free(ast_node->third);

    //printf("freed: %s\n", nd_kind_array[ast->node_kind]);
    if(ast_node->token == ID)
    {
        free(ast_node->val.id);
        ast_node->val.id = NULL;
    }
    free(ast_node);
    ast_node = NULL;
}

void ast_print(node* curr)
{
    if(curr == NULL)
    {
        return;
    }
    //printf("\ntype: %d\n", curr->typ);

    switch(curr->node_kind)
    {
        case PROGRAM:
            ast_print(curr->first);
            break;

        case SCOPE:
            printf("( SCOPE ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" )\n");
            break;

        case DECLARATIONS:
            printf("( DECLARATIONS ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" )\n");
            break;

        case DECLARATION:
            printf("( DECLARATION %s ", curr->val.id);
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" )\n");
            break;

        case CONST_DECLARATION:
            printf("(CONST DECLARATION %s ", curr->val.id);
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" )\n");
            break;

        case STATEMENTS:
            printf("( STATEMENTS ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" )\n");
            break;

        case ASSIGN_STMT:
            //printf("( ASSIGN %s ", type_array[curr->typ]);
            printf("( ASSIGN ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(")");
            break;

        case INT_NODE:
            printf("%s ", type_array[curr->typ]);
            if(curr->val.i_val != INT_MAX)
            {printf("%d ", curr->val.i_val);}
            break;

        case FLOAT_NODE:
            printf("%s ", type_array[curr->typ]);
            if(curr->val.i_val != FLT_MAX)
            {printf("%.3f ", curr->val.f_val);}
            break;

        case BOOL_NODE:
            printf("%s ", type_array[curr->typ]);
            printf("%s ", curr->val.b_val? "true" : "false");
            break;

        case IVEC_NODE:
            printf("%s ",type_array[curr->typ]);
            break;

        case VEC_NODE:
            printf("%s ",type_array[curr->typ]);
            break;

        case BVEC_NODE:
            printf("%s ",type_array[curr->typ]);
            break;

        case NESTED_EXP:
            ast_print(curr->first);
            break;

        case VAR_TO_EXP:
            ast_print(curr->first);
            break;

        case VAR_NODE:
            printf("%s ", curr->val.id);
            break;

        case TYPE_NODE:
            //printf("%s ", type_array[curr->token]);
            break;

        case UNARY_EXP:
            printf("( UNARY %s ",unary_op_array[curr->token]);//, type_array[curr->typ]);
            ast_print(curr->first);
            printf(")");
            break;

        case BINARY_EXP:
            printf("( BINARY %s ",binary_op_array[curr->token]);//type_array[curr->typ]
            ast_print(curr->first);
            ast_print(curr->second);
            printf(")");
            break;

        case ARGUMENTS_EXP:
            ast_print(curr->first);
            break;

        case FUNCTION:
            printf("(CALL %s ", func_array[curr->typ]);
            ast_print(curr->first);
            printf(")");
            break;

        case MULTI_ARG:
            ast_print(curr->first);
            ast_print(curr->second);
            break;

        case CONSTRUCTOR:
            printf("(CALL ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(")");
            break;

        case IF_STMT:
            printf("(IF ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(")");
            break;

        case IF_ELSE_STMT:
            printf("(IF ");
            ast_print(curr->first);
            ast_print(curr->second);
            printf(" ELSE ");
            ast_print(curr->third);
            printf(")");
            break;

        case NESTED_SCOPE:
            ast_print(curr->first);
            break;

        case ARRAY_NODE:
            printf("( INDEX %s [%d] )",curr->val.id, curr->index);
            break;


        default: break;
    }
}

