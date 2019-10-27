#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "main.h"
#include "../lib/ast.h"
#include "fn.h"

int yyparse (void);
void print_node(struct ast_node* node);

/**
 * Manejador de errores léxicos
 *
 * @param  line_number
 * @param  lexem
 * @return
 */
int lexer_error(int line_number, char *lexem)
{
    printf("\nError léxico en la línea %d: '%s'\n", line_number, lexem);
    return -1;
}

/**
 * Manejador de errores sinácticos
 *
 * @param  line_number
 * @param  s
 * @return
 */
int syntax_error(int line_number, char *s)
{
    printf("\nError sintáctico en la línea %d (%s).\n", line_number, s);
    return -2;
}

void print_blocks(struct ast_node* node)
{
    struct ast_node* n;
    int i = 0;

    while (NULL != (n = ast_get_nth_child(node, i++))) {
        print_node(n);
        printf(";\n");
    }
}

void print_id(struct ast_node* node)
{
    printf("%s", node->data.id.id);
}

void print_if(struct ast_node* node)
{
    struct ast_node* n;

    printf("if(");
    // condicion
    n = ast_get_nth_child(node, 0);
    print_node(n);
    printf(") {\n");

    // then
    n = ast_get_nth_child(node, 1);
    print_node(n);
    // else?
    n = ast_get_nth_child(node, 2);
    if (n != NULL) {
        printf("} else {\n");
        print_node(n);
    }
    printf("}\n");
}

void print_node(struct ast_node* node)
{
    switch (node->type) {
        case N_BLOCKS:
            print_blocks(node);
            break;
        case N_ID:
            print_id(node);
            break;
        case N_IF:
            print_if(node);
            break;
        default:
            printf("nodo(%d) ??\n", node->type);
    }
}

void print_ast(struct ast_node* ast)
{
    print_node(ast);
}

void print_st(int id, struct usr_st_data* data)
{
    // if (data->type == 0) {}
    printf("double %s = 0;\n", data->name);
}


int main(int argc, char **argv)
{
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    }

    if (yyparse()) {
        return 1;
    }

    printf("int main() {\n");

    st_print_symbols(print_st);
    printf("\n\n");

    print_ast(ast);

    printf("}\n");

    return 0;
}
