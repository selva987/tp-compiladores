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
    printf("$%s", node->data.id.id);
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

void print_while(struct ast_node* node)
{
    struct ast_node* n;

    printf("while(");
    // condicion
    n = ast_get_nth_child(node, 0);
    print_node(n);
    printf(") {\n");

    // then
    n = ast_get_nth_child(node, 1);
    print_node(n);
    printf("}\n");
}

void print_expr(struct ast_node* node)
{
    struct ast_node* n;

///////////////////////////SI EN EL PROGRAMA DESTINO FALTAN ELEMENTOS VER ACA
    n = ast_get_nth_child(node, 0);
    if(n != NULL) {
        print_node(n);
    }
}

void print_operand(struct ast_node* node)
{
    switch(node->data.operand) {
        case T_OP_EQUAL:
            printf("==");
            break;
        case T_OP_DISTINCT:
            printf("!=");
            break;
        case T_OP_LESSER:
            printf("<");
            break;
        case T_OP_GREATER:
            printf(">");
            break;
        case T_OP_LESSER_EQ:
            printf("<=");
            break;
        case T_OP_GREATER_EQ:
            printf(">=");
            break;
        case T_OP_AND:
            printf("&&");
            break;
        case T_OP_OR:
            printf("||");
            break;
        case T_OP_ASSIGN:
            printf("=");
            break;
        case T_AOP_PLUS:
            printf("+");
            break;
        case T_AOP_MINUS:
            printf("-");
            break;
        case T_AOP_MUL:
            printf("*");
            break;
        case T_AOP_DIV:
            printf("/");
            break;
        case T_AOP_POW:
            printf("^");
            break;
    }
}

void print_operation(struct ast_node* node)
{
    //operando operador operando
    struct ast_node* n;

    n = ast_get_nth_child(node, 0);
    print_node(n);

    n = ast_get_nth_child(node, 1);
    if(n != NULL) {
        printf(" ");
        print_node(n);
    }

    n = ast_get_nth_child(node, 2);
    if(n != NULL) {
        printf(" ");
        print_node(n);
    }
    
}

void print_const_number(struct ast_node* node)
{
    printf("%f", node->data.number);
}

void print_const_integer(struct ast_node* node)
{
    printf("%i", node->data.integer);
}

void print_const_string(struct ast_node* node)
{
    printf("\'%s", node->data.string);
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
        case N_WHILE:
            print_while(node);
            break;
        case N_EXPR:
            print_expr(node);
            break;
        case N_OPERAND:
            print_operand(node);
            break;
        case N_OPERATION:
            print_operation(node);
            break;
        case N_CONST_INTEGER:
            print_const_integer(node);
            break;
        case N_CONST_NUMBER:
            print_const_number(node);
            break;
        case N_CONST_STRING:
            print_const_string(node);
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

    printf("<?php\n");

    //st_print_symbols(print_st);
    //printf("\n\n");

    print_ast(ast);

    printf("?>");

    return 0;
}
