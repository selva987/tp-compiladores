#ifndef FN_H
#define FN_H

#include "../lib/ast.h"

#define N_IF                    2

struct ast_node* n_if(struct ast_node* cond, struct ast_node* then, struct ast_node* _else);

#define N_WHILE		3

struct ast_node* n_while(struct ast_node* cond, struct ast_node* body);

#define N_CONST_INTEGER 	4

struct ast_node* n_const_integer(int number);

#

#endif /* FN_H */