#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "symbol.h"

void semantic_check( node *ast);//previously, int

//checks

/* assignment_check does the following checks:
 * 1    Implict type conversion
 * 2    Wrong assignment
 * 3    Constructor call
*/
void assignment_check(struct symbol* curr, int expected_type);




#endif
