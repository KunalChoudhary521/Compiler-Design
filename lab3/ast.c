//top-down (preorder), bottom-up (postorder)
//(./compiler467 -Da) < test_ast.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"


#define DEBUG_PRINT_TREE 0

char* unary_op_array[] = {"MINUS", "NOT"};

char* binary_op_array[] = {"AND", "OR",
                         "EQ", "NEQ", "LT", "LEQ", "GT", "GEQ",
                         "ADD", "SUB", "MULT", "DIV", "POW_OP"};

char* type_array[] = { "INT", "IVEC2", "IVEC3", "IVEC4",
                     "BOOL", "BVEC2", "BVEC3", "BVEC4",
                     "FLOAT", "VEC2", "VEC3", "VEC4", "FUNCT" };

char* func_array[] = {"dp3", "lit", "rsq"};


node *ast = NULL;
//AST construction happens bottom-up because the bison operates bottom-up (postorder)
node *ast_allocate(node_kind kind, ...) {
    va_list args;//defined in stdio.h

    // make the node
    node *ast = (node *) malloc(sizeof(node));
    memset(ast, 0, sizeof *ast);
    ast->kind = kind;

    va_start(args, kind);

    switch(kind) {
        case INSIDE_SCOPE:
            printf("alloc INSIDE_SCOPE\n");
            ast->inside_scope.scope = va_arg(args, node*);
            break;

        case SCOPE_NODE:
            printf("alloc SCOPE_NODE\n");
            ast->scope.declarations = va_arg(args, node*);
            ast->scope.statements = va_arg(args, node *);
            break;

        case DECLARATIONS_NODE:
            printf("alloc DECLARATIONS_NODE\n");
            ast->declarations.declarations = va_arg(args, node*);
            ast->declarations.declaration = va_arg(args, node*);
            break;

        case DECLARATION_NODE:
            printf("alloc DECLARATION_NODE\n");
            ast->declaration.type = va_arg(args, node*);
            ast->declaration.id = va_arg(args, char*);
            break;

        case STATEMENTS_NODE:
            printf("alloc STATEMENTS_NODE\n");
            ast->statements.statements = va_arg(args, node*);
            ast->statements.statement = va_arg(args, node*);
            break;

        case NESTED_EXP_NODE:
            printf("alloc NESTED_EXP_NODE\n");
            ast->nested_exp.expression = va_arg(args, node*);
            break;

        case UNARY_EXPRESION_NODE:
            printf("alloc UNARY_EXPRESION_NODE\n");
            ast->unary_expr.op = va_arg(args, int);
            ast->unary_expr.right = va_arg(args, node *);
            break;

        case BINARY_EXPRESSION_NODE:
            printf("alloc EXPRESSION_NODE\n");
            ast->binary_expr.op = va_arg(args, int);
            ast->binary_expr.left = va_arg(args, node *);
            ast->binary_expr.right = va_arg(args, node *);
            break;

        case ASSIGNMENT_NODE:
            printf("alloc ASSIGNMENT_NODE\n");
            ast->assignment.left = va_arg(args, node *);//variable
            ast->assignment.right = va_arg(args, node *);//expression
            break;

        case IF_STATEMENT_NODE:
            printf("alloc IF_STATEMENT_NODE\n");
            ast->if_statement.condition = va_arg(args, node *);
            ast->if_statement.then_st =  va_arg(args, node *);
            break;

        case IF_ELSE_STATEMENT_NODE:
            printf("alloc IF_ELSE_STATEMENT_NODE\n");
            ast->if_else_statement.condition = va_arg(args, node *);
            ast->if_else_statement.then_st =  va_arg(args, node *);
            ast->if_else_statement.else_st =  va_arg(args, node *);
            break;

        case CONSTRUCTOR_NODE:
            printf("alloc CONSTRUCTOR_NODE\n");
            ast->constructor.type = va_arg(args, node *);
            ast->constructor.arguments = va_arg(args, node *);
            break;

        case FUNCTION_NODE:
            printf("alloc FUNCTION_NODE\n");
            ast->function.name = va_arg(args, int);
            ast->function.arguments = va_arg(args, node *);
            break;

        case DECLARATION_ASSIGN_NODE:
            printf("alloc DECLARATION_ASSIGN_NODE\n");
            ast->declaration_assignment.type = va_arg(args, node*);
            ast->declaration_assignment.id = va_arg(args, char*);
            ast->declaration_assignment.value = va_arg(args, node*);
            break;

        case CONST_DECLARATION_NODE:
            printf("alloc CONST_DECLARATION_NODE\n");
            ast->const_declaration.type = va_arg(args, node*);
            ast->const_declaration.id = va_arg(args, char*);
            ast->const_declaration.value = va_arg(args, node*);
            break;

        case TYPE_NODE:
            printf("alloc TYPE_NODE\n");
            ast->type.name = va_arg(args, int);
            break;

        case INT_NODE:
            printf("alloc INT_NODE\n");
            ast->int_const.right = va_arg(args, int);
            break;

        case FLOAT_NODE:
            printf("alloc FLOAT_NODE\n");
            ast->float_const.right = va_arg(args, double);
            break;

        case BOOL_NODE:
            printf("alloc BOOL_NODE\n");
            ast->bool_const.right = va_arg(args, int);
            break;

        case VAR_NODE:
            printf("alloc VAR_NODE\n");
            ast->variable.id = va_arg(args, char*);
            break;

        case ARRAY_NODE:
            printf("alloc ARRAY_NODE\n");
            ast->array.id = va_arg(args, char*);
            ast->array.index = va_arg(args, int);
            break;

        case MULTI_ARGUMENT_NODE:
            printf("alloc MULTI_ARGUMENT_NODE\n");
            ast->multi_argument.arguments = va_arg(args, node*);
            ast->multi_argument.expression = va_arg(args, node*);
            break;

        case ARGUMENTS_EXP_NODE:
            printf("alloc ARGUMENTS_EXP_NODE\n");
            ast->arguments_exp.expression = va_arg(args, node*);
            break;

        default: break;
    }

    va_end(args);

    return ast;
}

void ast_free(node *ast)//needs to be bottom-up (postorder)
{
    if(ast == NULL)
    {
        return;
    }

    switch(ast->kind){
        case INSIDE_SCOPE:
            ast_free(ast->inside_scope.scope);
            free(ast);
            ast = NULL;
            break;

        case SCOPE_NODE:
            ast_free(ast->scope.declarations);
            ast_free(ast->scope.statements);
            free(ast);
            ast = NULL;
            break;

        case DECLARATIONS_NODE:
            ast_free(ast->declarations.declarations);
            ast_free(ast->declarations.declaration);
            free(ast);
            ast = NULL;
            break;

        case DECLARATION_NODE:
            ast_free(ast->declaration.type);
            //ast_free(ast->declaration.id);//it doesn't make sense to traverse a non-node* type
            break;

        case STATEMENTS_NODE:
            ast_free(ast->statements.statements);
            ast_free(ast->statements.statement);
            free(ast);
            ast = NULL;
            break;

        case EXPRESSION_NODE:
            break;

        case NESTED_EXP_NODE:
            ast_free(ast->nested_exp.expression);
            free(ast);
            ast = NULL;
            break;

        case UNARY_EXPRESION_NODE:
            //ast_free(ast->unary_expr.op);
            ast_free(ast->unary_expr.right);
            free(ast);
            ast = NULL;
            break;

        case BINARY_EXPRESSION_NODE:
            //ast_free(ast->binary_expr.op);
            ast_free(ast->binary_expr.left);
            ast_free(ast->binary_expr.right);
            free(ast);
            ast = NULL;
            break;

        case ASSIGNMENT_NODE:
            ast_free(ast->assignment.left);//variable
            ast_free(ast->assignment.right);//expression
            free(ast);
            ast = NULL;
            break;

        case IF_STATEMENT_NODE:
            ast_free(ast->if_statement.condition);
            ast_free(ast->if_statement.then_st);
            free(ast);
            ast = NULL;
            break;

        case IF_ELSE_STATEMENT_NODE:
            ast_free(ast->if_else_statement.condition);
            ast_free(ast->if_else_statement.then_st);
            ast_free(ast->if_else_statement.else_st);
            free(ast);
            ast = NULL;
            break;

        case CONSTRUCTOR_NODE:
            //ast_free(ast->constructor.type);
            ast_free(ast->constructor.arguments);
            free(ast);
            ast = NULL;
            break;

        case FUNCTION_NODE:
            //ast_free(ast->function.name);
            ast_free(ast->function.arguments);
            free(ast);
            ast = NULL;
            break;

        case DECLARATION_ASSIGN_NODE:
            ast_free(ast->declaration_assignment.type);
            //ast_free(ast->declaration_assignment.id);
            ast_free(ast->declaration_assignment.value);
            free(ast);
            ast = NULL;
            break;

        case CONST_DECLARATION_NODE:
            ast_free(ast->const_declaration.type);
            //ast_free(ast->const_declaration.id);
            ast_free(ast->const_declaration.value);
            free(ast);
            ast = NULL;
            break;

        case TYPE_NODE:
            //ast_free(ast->type.name);
            free(ast);
            ast = NULL;
            break;

        case INT_NODE:
            //ast_free(ast->int_const.right);
            free(ast);
            ast = NULL;
            break;

        case FLOAT_NODE:
            //ast_free(ast->float_const.right);
            free(ast);
            ast = NULL;
            break;

        case BOOL_NODE:
            //ast_free(ast->bool_const.right);
            free(ast);
            ast = NULL;
            break;

        case VAR_NODE:
            //ast_free(ast->variable.id);//not sure if to traverse
            free(ast);
            ast = NULL;
            break;

        case ARRAY_NODE:
            //ast_free(ast->array.id);
            //ast_free(ast->array.index);
            free(ast);
            ast = NULL;
            break;

        case MULTI_ARGUMENT_NODE:
            ast_free(ast->multi_argument.arguments);
            ast_free(ast->multi_argument.expression);
            free(ast);
            ast = NULL;
            break;

        case ARGUMENTS_EXP_NODE:
            ast_free(ast->arguments_exp.expression);
            free(ast);
            ast = NULL;
            break;

        default: break;
    }
}

/* How ast_print works?
 * 1    detect what is the "kind" of current node
 * 2    print appropriate info about that node
 * 3    call ast_print on that "kind" of node
 */
void ast_print(node *ast)//needs to be top-down (preorder)
{
    if(ast == NULL)
    {
        return;
    }

    switch(ast->kind){
        case INSIDE_SCOPE:
            //printf("\n(SCOPE");
            ast_print(ast->inside_scope.scope);
            //printf(")\n");
            break;

        case SCOPE_NODE:
            printf("\n(SCOPE ");
            ast_print(ast->scope.declarations);
            ast_print(ast->scope.statements);
            printf(")\n");
            break;

        case DECLARATIONS_NODE:
            printf("( DECLARATIONS ");
            ast_print(ast->declarations.declarations);
            ast_print(ast->declarations.declaration);
            printf(")\n");
            break;

        case STATEMENTS_NODE:
            printf("( STATEMENTS ");
            ast_print(ast->statements.statements);
            ast_print(ast->statements.statement);
            printf(")\n");
            break;

        case EXPRESSION_NODE:
            break;

        case NESTED_EXP_NODE:
            ast_print(ast->nested_exp.expression);
            break;

        case UNARY_EXPRESION_NODE:
            printf("( UNARY %s ",unary_op_array[ast->unary_expr.op]);
            ast_print(ast->unary_expr.right);
            printf(")\n");
            break;

        case BINARY_EXPRESSION_NODE:
            printf("( BINARY %s ",binary_op_array[ast->binary_expr.op]);
            ast_print(ast->binary_expr.left);
            ast_print(ast->binary_expr.right);
            printf(")\n");
            break;

        case INT_NODE:
            printf("%d ", ast->int_const.right);
            break;

        case FLOAT_NODE:
            printf("%.3f ", ast->float_const.right);
            break;

        case BOOL_NODE:
            printf("%s ", (ast->bool_const.right) ? "true" : "false");
            break;

        case IDENT_NODE:
            break;

        case VAR_NODE:
            printf("%s ", ast->variable.id);//check if variable type also prints
            break;

        case ARRAY_NODE:
            printf("( INDEX %s %d ", ast->array.id, ast->array.index);//( INDEX type id index)
            //need to print type
            break;

        case FUNCTION_NODE:
            printf("( CALL %s ", func_array[ast->function.name]);
            ast_print(ast->function.arguments);
            printf(")\n");
            break;

        case CONSTRUCTOR_NODE:
            printf("\n( CONSTRUCTOR " );//need to print constructor type (ivec2,int, bool, ...)
            ast_print(ast->constructor.type);
            ast_print(ast->constructor.arguments);
            printf(")\n");
            break;

        case TYPE_NODE:
            printf("%s ", type_array[ast->type.name]);
            break;

        case IF_STATEMENT_NODE:
            /*
             * cond is an expression form, then-stmt is a statement form,
             * and else-stmt? is an optional statement form.
             */
            printf("( IF ");
            ast_print(ast->if_statement.condition);
            ast_print(ast->if_statement.then_st);
            printf(")\n");
            break;

        case IF_ELSE_STATEMENT_NODE:
            printf("( IF ");
            ast_print(ast->if_else_statement.condition);
            ast_print(ast->if_else_statement.then_st);
            ast_print(ast->if_else_statement.else_st);
            printf(")\n");
            break;

        case ASSIGNMENT_NODE:
            printf("\n( ASSIGN ");
            ast_print(ast->assignment.left);
            ast_print(ast->assignment.right);
            printf(")\n");
            break;

        case NESTED_SCOPE_NODE:
            break;

        case DECLARATION_NODE:
            printf("\n( DECLARE_ONLY %s ",ast->declaration.id);//check if variable type prints
            ast_print(ast->declaration.type);
            printf(")\n");
            break;

        case DECLARATION_ASSIGN_NODE:
            printf("\n( ASSIGN_&_DECLARE %s ", ast->declaration_assignment.id);
            ast_print(ast->declaration_assignment.type);
            ast_print(ast->declaration_assignment.value);
            printf(")\n");
            break;

        case CONST_DECLARATION_NODE:
            printf("\n( DECLARE_CONST_ASSIGN %s ", ast->const_declaration.id);
            ast_print(ast->const_declaration.type);
            ast_print(ast->const_declaration.value);
            printf(")\n");
            break;

        case MULTI_ARGUMENT_NODE:
            ast_print(ast->multi_argument.arguments);
            ast_print(ast->multi_argument.expression);
            break;

        case ARGUMENTS_EXP_NODE:
            ast_print(ast->arguments_exp.expression);
            break;

        default: break;
    }

}
