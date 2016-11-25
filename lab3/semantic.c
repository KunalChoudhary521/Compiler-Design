#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "common.h"

int curr_type = -2;
int exp_type = -1;//expected type
struct symbol* temp;

void semantic_check( node *curr)//semantic check happens bottom-up(postorder)
{
    if(curr == NULL)
    {
        return ;
    }
    //printf("\ntype: %d\n", curr->typ);

    switch(curr->node_kind)
    {
        case PROGRAM:
            semantic_check(curr->first);
            break;

        case SCOPE:
            insert_scope();

            semantic_check(curr->first);
            semantic_check(curr->second);

            delete_scope(get_head_scope());
            break;

        case DECLARATIONS:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case DECLARATION:
            semantic_check(curr->first);
            semantic_check(curr->second);

            //printf("DECLARATION: id: %s | %d\n",curr->val.id, curr_type);
            temp = create_symbol(curr->val.id,curr_type);
            insert_symbol(get_head_scope(),temp);
            implicit_conv_check(temp,exp_type);
            break;

        case CONST_DECLARATION:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case STATEMENTS:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case ASSIGN_STMT:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case INT_NODE:
            exp_type = INT;
            break;

        case FLOAT_NODE:
            //printf("FLOAT_NODE: typ: %d \n",curr->typ);
            exp_type = FLOAT;
            break;

        case BOOL_NODE:
            exp_type = BOOL;
            break;

        case IVEC_NODE:
            break;

        case VEC_NODE:
            break;

        case BVEC_NODE:
            break;

        case NESTED_EXP:
            semantic_check(curr->first);
            break;

        case VAR_TO_EXP:
            semantic_check(curr->first);
            break;

        case VAR_NODE:
            break;

        case TYPE_NODE:
            //printf("In TYPE_NODE: typ %d\n", curr->typ);
            curr_type = curr->typ;
            break;

        case UNARY_EXP:
            semantic_check(curr->first);
            break;

        case BINARY_EXP:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case ARGUMENTS_EXP:
            semantic_check(curr->first);
            break;

        case FUNCTION:
            semantic_check(curr->first);
            break;

        case MULTI_ARG:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case CONSTRUCTOR:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case IF_STMT:
            semantic_check(curr->first);
            semantic_check(curr->second);
            break;

        case IF_ELSE_STMT:
            semantic_check(curr->first);
            semantic_check(curr->second);
            semantic_check(curr->third);
            break;

        case NESTED_SCOPE:
            semantic_check(curr->first);
            break;

        case ARRAY_NODE:
            break;

        default: break;
    }
}

void implicit_conv_check(struct symbol* sym, int expected_type)
{
    if(sym->type != FLOAT)//allows int to be assigned to float, but not a bool
    {
        if(sym->type != expected_type)
        {
            fprintf(errorFile,"error: implicit type conversion from %s to %s\n",
                    type_array[sym->type], type_array[expected_type]);
        }
    }
    else
    {
        if(expected_type == BOOL)
        {
            fprintf(errorFile,"error: implicit type conversion from %s to %s\n",
                    type_array[sym->type], type_array[BOOL]);
        }
    }

}
