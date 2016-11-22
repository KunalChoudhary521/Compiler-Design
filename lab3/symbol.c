#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"

struct scope* head_scope;//always points to first scope

void table_init()//initialize symbol table using head_scope
{
    head_scope = NULL;
}
void table_dtor()//delete all symbols in all scopes using head_scope
{
    struct scope* temp = head_scope;
    while(temp != NULL)
    {
        delete_scope(temp);
        head_scope = temp->next;
        free(temp);
        temp = head_scope;
    }

    head_scope = NULL;
}
void insert_scope()
{
    if(head_scope == NULL)
    {
        head_scope = (struct scope*)malloc(sizeof(struct scope));
        head_scope->prev = NULL;
        head_scope->next = NULL;
        head_scope->table_list = NULL;
    }
    else//place this scope @ head of the scope_list
    {
        struct scope* temp = (struct scope*)malloc(sizeof(struct scope));
        temp->prev = NULL;
        temp->next = head_scope;
        head_scope->prev = temp;
        head_scope = temp;
    }
}
void delete_scope(struct scope* curr)
{
    struct symbol* sym_itr = curr->table_list;
    while(sym_itr != NULL)
    {
        if(sym_itr->id != NULL)
        {
            free(sym_itr->id);//also free string ids
        }
        sym_itr = sym_itr->next;
    }
    curr->table_list = NULL;
}

void insert_symbol(struct scope* curr, char* id, int typ)
{
    //insert symbol in current scope @ head of the list
    struct symbol* temp = (struct symbol*)malloc(sizeof(struct symbol));
    temp->type = typ;
    temp->id = (char*)malloc(sizeof(char) * (strlen(id) + 1));//+1 for '\0' character
    strcpy(temp->id,id);
    temp->next = curr->table_list;
    curr->table_list = temp;
}
void delete_symbol(struct scope* curr)//delete symbol from current scope
{/*deleting a specific symbol is unecessary*/}

//this function prevents multiple declarations
struct symbol* find_symbol(struct scope* curr, char* id)//find symbol from current scope
{
    //char* id is the name/identifier of the variable
    struct symbol* sym_itr = curr->table_list;
    while(sym_itr != NULL)
    {
        if(strcmp(sym_itr->id, id) == 0)//id found
        {
            break;
        }
        sym_itr = sym_itr->next;
    }
    return sym_itr;
}
void print_table(struct scope* curr)//print symbol table from a scope
{
    printf("---------Symbol Table (type|id)---------------\n");
    struct symbol* sym_itr = curr->table_list;
    while(sym_itr != NULL)
    {
        printf("%d | %s", sym_itr->type, sym_itr->id);
        sym_itr = sym_itr->next;
    }

    printf("------------------------------------\n");
}
