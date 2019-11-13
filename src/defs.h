#ifndef DEFS_H
#define DEFS_H

#include "y.tab.h"

/**
 * Estructura de los elementos de la Tabla de Simbolos (ST)
 * 
 * Como mínimo debe tener un campo name (que no puede ser renombrado) en el que se
 * almacena el nombre del símbolo. Luego, los demás campos deben definirse conforme
 * a la necesidad de cada desarrollo.
 * function define si es o no una funcion
 */
struct usr_st_data {
    char* name;
    int type;
    int function;
};


/**
 * Estructura de los nodos del Árbol de Análisis Sintáctico (AST)
 *
 * Los campos que se definen deben comprender los posibles valores que requieran
 * para conformar los diversos tipos de nodos del AST.
 */
union usr_ast_data {
    struct ast_node* node;
    struct {
        char* id;
        int type;
    } id;
    char* string;
    double number;
    int boolean;
    int operand;
    int integer;
};

struct ast_node *ast;

#include "../lib/cya.h"

#endif /* DEFS_H */