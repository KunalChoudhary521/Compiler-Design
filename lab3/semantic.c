#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "common.h"

int curr_type = -1;

void semantic_check( node *ast)//semantic check happens bottom-up(postorder)
{
    if(ast == NULL)
    {
        return;
    }

    switch(ast->kind)
    {
        case INSIDE_SCOPE:
            semantic_check(ast->inside_scope.scope);

            delete_scope(get_head_scope());//delete the scope that the head of scope list
            break;

        case SCOPE_NODE:
            semantic_check(ast->scope.declarations);
            semantic_check(ast->scope.statements);
            break;

        case DECLARATIONS_NODE:
            semantic_check(ast->declarations.declarations);
            semantic_check(ast->declarations.declaration);
            break;

        case STATEMENTS_NODE:
            semantic_check(ast->statements.statements);
            semantic_check(ast->statements.statement);
            break;

        case NESTED_EXP_NODE:
            semantic_check(ast->nested_exp.expression);
            break;

        case UNARY_EXPRESION_NODE:
            //semantic_check(ast->unary_expr.op);//no need to call semantic_check on non-node* types
            semantic_check(ast->unary_expr.right);
            break;

        case BINARY_EXPRESSION_NODE:
            //semantic_check(ast->binary_expr.op);
            semantic_check(ast->binary_expr.left);
            semantic_check(ast->binary_expr.right);
            break;

        case ASSIGNMENT_NODE:
            semantic_check(ast->assignment.left);//variable
            semantic_check(ast->assignment.right);//expression
            break;

        case IF_STATEMENT_NODE:
            semantic_check(ast->if_statement.condition);
            semantic_check(ast->if_statement.then_st);
            break;

        case IF_ELSE_STATEMENT_NODE:
            semantic_check(ast->if_else_statement.condition);
            semantic_check(ast->if_else_statement.then_st);
            semantic_check(ast->if_else_statement.else_st);
            break;

        case CONSTRUCTOR_NODE:
            semantic_check(ast->constructor.type);
            semantic_check(ast->constructor.arguments);
            break;

        case FUNCTION_NODE:
            //semantic_check(ast->function.name);
            semantic_check(ast->function.arguments);
            break;

        case DECLARATION_NODE:
            semantic_check(ast->declaration.type);
            //semantic_check(ast->declaration.id);

            insert_scope();
            insert_symbol(get_head_scope(), create_symbol(ast->declaration_assignment.id, curr_type));
            break;

        case DECLARATION_ASSIGN_NODE:
            semantic_check(ast->declaration_assignment.type);
            //semantic_check(ast->declaration_assignment.id);
            semantic_check(ast->declaration_assignment.value);

            insert_scope();
            insert_symbol(get_head_scope(), create_symbol(ast->declaration_assignment.id, curr_type));
            break;

        case CONST_DECLARATION_NODE:
            semantic_check(ast->const_declaration.type);
            //semantic_check(ast->const_declaration.id);
            semantic_check(ast->const_declaration.value);
            break;

        case TYPE_NODE:
            //semantic_check(ast->type.name);
            curr_type = -1;//prevents propogation of previous curr_type value
            curr_type = ast->type.name;
            break;

        case INT_NODE:
            //semantic_check(ast->int_const.right);
            //assumes that variable being checked is currently at the top of symbol table
            implicit_conv_check(get_head_scope()->table_list, INT);//enum from ast.h
            break;

        case FLOAT_NODE:
            //semantic_check(ast->float_const.right);
            implicit_conv_check(get_head_scope()->table_list, FLOAT);
            break;

        case BOOL_NODE:
            //semantic_check(ast->bool_const.right);
            //assumes that variable being checked is currently at the top of symbol table
            implicit_conv_check(get_head_scope()->table_list, BOOL);//enum from ast.h
            break;

        case VAR_NODE:
            //semantic_check(ast->variable.id);
            break;

        case ARRAY_NODE:
            //semantic_check(ast->array.id);
            //semantic_check(ast->array.index);
            break;

        case MULTI_ARGUMENT_NODE:
            semantic_check(ast->multi_argument.arguments);
            semantic_check(ast->multi_argument.expression);
            break;

        case ARGUMENTS_EXP_NODE:
            semantic_check(ast->arguments_exp.expression);
            break;

        default: break;
    }
}


void implicit_conv_check(struct symbol* curr, int expected_type)
{
    if(curr->type != FLOAT)//allows int to be assigned to float, but not a bool
    {
        if(curr->type != expected_type)
        {
            fprintf(errorFile,"error: implicit type conversion from %s to %s\n",
                    type_array[curr->type], type_array[expected_type]);
        }
    }
    else
    {
        if(expected_type == BOOL)
        {
            fprintf(errorFile,"error: implicit type conversion from %s to %s\n",
                    type_array[curr->type], type_array[BOOL]);
        }
    }

}
