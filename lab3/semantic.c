#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
//semantic check happens bottom-up(postorder)
void semantic_check( node *ast)
{
    if(ast == NULL)
    {
        return;
    }

    switch(ast->kind)
    {
        case INSIDE_SCOPE:
            semantic_check(ast->inside_scope.scope);
            break;
        case SCOPE_NODE:
            semantic_check(ast->scope.declarations);
            semantic_check(ast->scope.statements);
            insert_scope();
            break;
        case DECLARATIONS_NODE:
            /*semantic_check(ast->declarations.declarations);
            semantic_check(ast->declarations.declaration);*/            
            break;
        default: break;
    }
}
