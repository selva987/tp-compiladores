#include "fn.h"

struct ast_node* n_if(struct ast_node* cond, struct ast_node* then, struct ast_node* _else)
{
    struct ast_node* _if = ast_new_node(N_IF);
    ast_add_child(_if, cond);
    ast_add_child(_if, then);
    if (_else != NULL) {
        ast_add_child(_if, _else);
    }

    return _if;
}


struct ast_node* n_while(struct ast_node* cond, struct ast_node* body)
{
    struct ast_node* _while = ast_new_node(N_WHILE);
    ast_add_child(_while, cond);
    ast_add_child(_while, body);

    return _while;
}

struct ast_node* n_const_integer(int number)
{
    struct ast_node* node = ast_new_node(N_CONST_INTEGER);
    node->data.integer = number;
    return node;
}