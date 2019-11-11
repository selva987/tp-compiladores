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
    fprintf(yyout,"\nError léxico en la línea %d: '%s'\n", line_number, lexem);
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
    fprintf(yyout,"\nError sintáctico en la línea %d (%s).\n", line_number, s);
    return -2;
}
/**
 * Manejador de errores semanticos
 *
 * @param  line_number
 * @param  s
 * @return
 */
int semantic_error(int line_number, char *s)
{
    fprintf(yyout,"\nError semántico en la línea %d (Ya existe una funcion %s).\n", line_number, s);
    exit(1);
}

void print_blocks(struct ast_node* node)
{
    struct ast_node* n;
    int i = 0;

    while (NULL != (n = ast_get_nth_child(node, i++))) {
        print_node(n);
        if(n->type != N_WHILE
            && n->type != N_IF
            && n->type != N_FUNCTION
            && n->type != N_FOREACH
            && n->type != N_LOAD_ARRAY
            && n->type != N_FOR) {
            fprintf(yyout,";");
        }
        fprintf(yyout,"\n");
    }
}

void print_id(struct ast_node* node)
{
    struct ast_node* n;

    fprintf(yyout,"$%s", node->data.id.id);
    
    //busco si tengo posicion de array, si la tengo imprimo corchetes
    n = ast_get_nth_child(node, 0);
    if(n != NULL) {
        fprintf(yyout,"[");
        print_node(n);
        fprintf(yyout,"]");
    }

}

void print_if(struct ast_node* node)
{
    struct ast_node* n;

    fprintf(yyout,"if(");
    // condicion
    n = ast_get_nth_child(node, 0);
    print_node(n);
    fprintf(yyout,") {\n");

    // then
    n = ast_get_nth_child(node, 1);
    print_node(n);
    // else?
    n = ast_get_nth_child(node, 2);
    if (n != NULL) {
        fprintf(yyout,"} else {\n");
        print_node(n);
    }
    fprintf(yyout,"}");
}

void print_while(struct ast_node* node)
{
    struct ast_node* n;

    fprintf(yyout,"while(");
    // condicion
    n = ast_get_nth_child(node, 0);
    print_node(n);
    fprintf(yyout,") {\n");

    // then
    n = ast_get_nth_child(node, 1);
    print_node(n);
    fprintf(yyout,"}");
}

void print_for(struct ast_node* node) 
{
    struct ast_node* n, *var;

    fprintf(yyout,"for(");

    //variable
    var =  ast_get_nth_child(node, 0);
    print_node(var);

    fprintf(yyout,"=");

    //condicion inicio
    n =  ast_get_nth_child(node, 1);
    print_node(n);

    fprintf(yyout,";");

    //condicion fin
    print_node(var);
    fprintf(yyout,"<=");
    n =  ast_get_nth_child(node, 2);
    print_node(n);

    fprintf(yyout,";");

    //incremento (por ahora solo suma de 1 en 1)
    print_node(var);
    fprintf(yyout,"++");

    fprintf(yyout,") {\n");

    //then
    n =  ast_get_nth_child(node, 3);
    print_node(n);

    fprintf(yyout,"}\n");
}

void print_foreach(struct ast_node* node) 
{
    struct ast_node* n;
    fprintf(yyout,"foreach(");

    //iterable
    n = ast_get_nth_child(node, 1);
    print_node(n);

    fprintf(yyout," as ");

    //iterador
    n = ast_get_nth_child(node, 0);
    print_node(n);

    fprintf(yyout,"){\n");

    //then
    n = ast_get_nth_child(node, 2);
    print_node(n);

    fprintf(yyout,"}");
}

void print_echo(struct ast_node* node) 
{
    struct ast_node* n;
    n = ast_get_nth_child(node, 0);

    fprintf(yyout,"echo ");
    print_node(n);
}

void print_input(struct ast_node* node) 
{
    struct ast_node* n;
    n = ast_get_nth_child(node, 0);

    print_node(n);

    fprintf(yyout," = trim(fgets(STDIN))");
}

void print_load_array(struct ast_node* node) 
{
    struct ast_node* var, *file;

    var = ast_get_nth_child(node,0);
    file = ast_get_nth_child(node,1);
    

    fprintf(yyout,"if(is_readable(");
    print_node(file);
    fprintf(yyout,")){\n");

    fprintf(yyout,"if($___nuestroComp_handle = fopen(");
    print_node(file);
    fprintf(yyout,",'r')){\n");

    fprintf(yyout,"while($___nuestroComp_str = fgets($___nuestroComp_handle)){\n");
    print_node(var);
    fprintf(yyout,"[] = $___nuestroComp_str;\n");

    fprintf(yyout,"}\nfclose($___nuestroComp_handle);\n}\n}");
    
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
            fprintf(yyout,"==");
            break;
        case T_OP_DISTINCT:
            fprintf(yyout,"!=");
            break;
        case T_OP_LESSER:
            fprintf(yyout,"<");
            break;
        case T_OP_GREATER:
            fprintf(yyout,">");
            break;
        case T_OP_LESSER_EQ:
            fprintf(yyout,"<=");
            break;
        case T_OP_GREATER_EQ:
            fprintf(yyout,">=");
            break;
        case T_OP_AND:
            fprintf(yyout,"&&");
            break;
        case T_OP_OR:
            fprintf(yyout,"||");
            break;
        case T_OP_ASSIGN:
            fprintf(yyout,"=");
            break;
        case T_OP_ACUMULATE_PLUS:
            fprintf(yyout,"+=");
            break;
        case T_OP_ACUMULATE_MINUS:
            fprintf(yyout,"-=");
            break;
        case T_OP_CONCAT:
            fprintf(yyout,".");
            break;
        case T_AOP_PLUS:
            fprintf(yyout,"+");
            break;
        case T_AOP_MINUS:
            fprintf(yyout,"-");
            break;
        case T_AOP_MUL:
            fprintf(yyout,"*");
            break;
        case T_AOP_DIV:
            fprintf(yyout,"/");
            break;
        case T_AOP_POW:
            fprintf(yyout,"^");
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
        fprintf(yyout," ");
        print_node(n);
    }

    n = ast_get_nth_child(node, 2);
    if(n != NULL) {
        fprintf(yyout," ");
        print_node(n);
    }
    
}

void print_const_number(struct ast_node* node)
{
    fprintf(yyout,"%f", node->data.number);
}

void print_const_integer(struct ast_node* node)
{
    fprintf(yyout,"%i", node->data.integer);
}

void print_const_string(struct ast_node* node)
{
    fprintf(yyout,"%s", node->data.string);
}

void print_exit(struct ast_node* node)
{
    fprintf(yyout,"exit(%d)", node->data.integer);
}

void print_function(struct ast_node* node) 
{
    
    struct ast_node* n;

    fprintf(yyout, "function %s(", node->data.string);

    //vars
    n = ast_get_nth_child(node, 0);
    if(n->type == N_VARS || n->type == N_ID) {
        print_node(n);
        n = ast_get_nth_child(node, 1);
    }

    fprintf(yyout, ") {\n");

    //cuerpo
    print_node(n);

    fprintf(yyout, "}");

}

void print_vars(struct ast_node* node) 
{
    struct ast_node* n;
    int i = 0;

    while(NULL != (n = ast_get_nth_child(node, i))) {
        if(i != 0) {
            fprintf(yyout, ",");
        }
        print_node(n);
        i++;
    }

}

void print_return(struct ast_node* node) 
{
    struct ast_node* n = ast_get_nth_child(node, 0);

    fprintf(yyout, "return ");
    print_node(n);

}

void print_function_call(struct ast_node* node) 
{
    struct ast_node* n = ast_get_nth_child(node, 0);

    fprintf(yyout, "%s(", node->data.string);

    if(n != NULL) {
        print_node(n);
    }

    fprintf(yyout, ")");
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
        case N_FOR:
            print_for(node);
            break;
        case N_FOREACH:
            print_foreach(node);
            break;
        case N_ECHO:
            print_echo(node);
            break;
        case N_INPUT:
            print_input(node);
            break;
        case N_LOAD_ARRAY:
            print_load_array(node);
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
        case N_EXIT:
            print_exit(node);
            break;
        case N_FUNCTION:
            print_function(node);
            break;
        case N_VARS:
            print_vars(node);
            break;
        case N_RETURN:
            print_return(node);
            break;
        case N_FUNCTION_CALL:
            print_function_call(node);
            break;
        default:
            fprintf(yyout,"nodo(%d) ??\n", node->type);
    }
}

void print_ast(struct ast_node* ast)
{
    print_node(ast);
}

void print_st(int id, struct usr_st_data* data)
{
    // if (data->type == 0) {}
    fprintf(yyout,"double %s = 0;\n", data->name);
}


int main(int argc, char **argv)
{
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    }
    if(argc > 2) {
        yyout = fopen(argv[2], "w");
    }

    if (yyparse()) {
        return 1;
    }

    fprintf(yyout,"<?php\nini_set('error_reporting', E_ALL & ~E_NOTICE);\n");

    //st_print_symbols(print_st);
    //fprintf(yyout,"\n\n");

    print_ast(ast);

    fprintf(yyout,"exit;\n ?>");

    return 0;
}
