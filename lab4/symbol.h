#ifndef _SYMBOL_H
#define _SYMBOL_H 1

struct symbol{
    int type;//use enum
    char* id;
    //int value;//not needed
    struct symbol* next;
};

struct scope {
    struct scope* prev;
    struct scope* next;
    struct symbol* table_list;
};

void table_init();//initialize symbol table using head_scope
void table_dtor();//delete all symbols in all scopes using head_scope
void insert_scope();
void delete_scope(struct scope* curr);
struct symbol* create_symbol(char* var_name, int typ);
void insert_symbol(struct scope* curr, struct symbol* temp);//insert symbol in current scope @ head of the list
//void delete_symbol(struct scope* curr);//delete symbol from current scope
struct symbol* find_symbol(struct scope* curr, char* id);//find symbol from current scope
void print_table(struct scope* curr);//print symbol table from a scope

struct scope* get_head_scope();
#endif
