#ifndef FN_H
#define FN_H

#include "../lib/ast.h"

#define N_IF                    2

struct ast_node* n_if(struct ast_node* cond, struct ast_node* then, struct ast_node* _else);

#define N_WHILE		3

struct ast_node* n_while(struct ast_node* cond, struct ast_node* body);

#define N_CONST_INTEGER 	4

struct ast_node* n_const_integer(int number);

#define N_FOR	5

struct ast_node* n_for(struct ast_node* var, struct ast_node* from, struct ast_node* to, struct ast_node* body);

#define N_ECHO	6

struct ast_node* n_echo(struct ast_node* arg);

#define N_INPUT	7

struct ast_node* n_input(struct ast_node* var);

#define N_LOAD_ARRAY	8

struct ast_node* n_loadArray(struct ast_node* var, struct ast_node* file);

#define N_FOREACH	9

struct ast_node* n_foreach(struct ast_node* var, struct ast_node* array, struct ast_node* then);

#define N_EXIT	10

struct ast_node* n_exit(int code);

#endif /* FN_H */