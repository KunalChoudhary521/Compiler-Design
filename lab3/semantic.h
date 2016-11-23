#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "symbol.h"

void semantic_check( node *ast);//previously, int

//checks
void implicit_conv_check(struct symbol* curr, int expected_type);

#endif
