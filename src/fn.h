#ifndef FN_H
#define FN_H

#include "../lib/ast.h"

#define N_IF                    2

struct ast_node* n_if(struct ast_node* cond, struct ast_node* then, struct ast_node* _else);

#endif /* FN_H */