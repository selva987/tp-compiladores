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

struct ast_node* n_for(struct ast_node* var, struct ast_node* from, struct ast_node* to, struct ast_node* body)
{
	struct ast_node* _for = ast_new_node(N_FOR);
    ast_add_child(_for, var);
    ast_add_child(_for, from);
    ast_add_child(_for, to);
    ast_add_child(_for, body);

    return _for;
}

struct ast_node* n_echo(struct ast_node* arg) 
{
	struct ast_node* _echo = ast_new_node(N_ECHO);
    ast_add_child(_echo, arg);

    return _echo;
}

struct ast_node* n_input(struct ast_node* var) 
{
	struct ast_node* _input = ast_new_node(N_INPUT);
    ast_add_child(_input, var);

    return _input;
}


struct ast_node* n_loadArray(struct ast_node* var, struct ast_node* file) 
{
	struct ast_node* node = ast_new_node(N_LOAD_ARRAY);

	ast_add_child(node, var);
	ast_add_child(node, file);
	return node;
}

struct ast_node* n_function(char* name, struct ast_node* vars, struct ast_node* body)
{
	struct ast_node* node = ast_new_node(N_FUNCTION);
	node->data.string = strdup(name);

	st_define_symbol(name, 1);

	if(vars != NULL) {
		ast_add_child(node, vars);
	}
	ast_add_child(node, body);

	return node;
}


struct ast_node* n_foreach(struct ast_node* var, struct ast_node* array, struct ast_node* then) 
{
	struct ast_node* node = ast_new_node(N_FOREACH);

	ast_add_child(node, var);
	ast_add_child(node, array);
	ast_add_child(node, then);

	return node;
}

struct ast_node* n_exit(int code) 
{
	struct ast_node* node = ast_new_node(N_EXIT);
    node->data.integer = code;
    return node;
}

struct ast_node* n_vars(struct ast_node* var, struct ast_node* vars)
{
	int i = 0;

	if(vars == NULL) {
		vars = ast_new_node(N_VARS);
	}

	ast_add_child_first(vars, var);


	return vars;
}

struct ast_node* n_return(struct ast_node* arg) {
	struct ast_node* node = ast_new_node(N_RETURN);

	ast_add_child(node, arg);

	return node;
}

struct ast_node* n_function_call(char* name, struct ast_node* params) {
	struct ast_node* node = ast_new_node(N_FUNCTION_CALL);
	node->data.string = strdup(name);

	if(params != NULL) {
		ast_add_child(node, params);
	}

	return node;
}