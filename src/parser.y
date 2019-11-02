%{
    /**
     * Analizador Sintáctico - Parser
     */

  	#include <stdio.h>
    #include "../lib/cya.h"
    #include "fn.h"
%}

%union {
    char* id;
    char* string;
    double number;
    int integer;
    struct ast_node* node;
}

%token <id> T_ID
%token T_IF T_WHILE T_LLA_I T_LLA_D T_PAR_I T_PAR_D T_ELSE T_TYPE_NUMBER T_TYPE_STRING T_COMMA T_OP_ASSIGN T_OP_EQUAL T_OP_DISTINCT T_OP_LESSER T_OP_GREATER T_OP_LESSER_EQ T_OP_GREATER_EQ T_OP_AND T_OP_OR T_AOP_PLUS T_AOP_MINUS T_AOP_MUL T_AOP_DIV T_AOP_POW T_NUMBER T_STRING T_INTEGER T_SEMICOLON
%type <node> linea lineas sigma if cond then else /*decl type*/ while operation operand operator



%%

sigma: lineas                               { $$ = $1; ast = $1; }
     ;

lineas: linea lineas                        { $$ = n_blocks($1, $2); }
      | /* lamda */                         { $$ = n_blocks(NULL, NULL); }
      ;

linea: if                                   { $$ = $1; }
     | while                                { $$ = $1; }
     | operation T_SEMICOLON                { $$ = $1; }
     /*| decl                               { $$ = NULL; }*/
     ;


if: T_IF cond then else                     { $$ = n_if($2, $3, $4); }
  ;

cond: T_PAR_I operation T_PAR_D             { $$ = $2; }
    ;

operation: operand operator operand         { $$ = n_operation($1, $2, $3); }
    | T_ID                                  { $$ = n_operation(n_id($1, 0), NULL, NULL); }
    ;

operand: T_ID                               { $$ = n_id($1,0); }
    | T_STRING                              { $$ = n_const_string(yylval.string); }
    | T_NUMBER                              { $$ = n_const_number(yylval.number); }
    | T_INTEGER                             { $$ = n_const_integer(yylval.integer); }
    | cond                                  { $$ = $1; }
    ;

operator: T_OP_EQUAL                        { $$ = n_operand(T_OP_EQUAL); }
    | T_OP_DISTINCT                         { $$ = n_operand(T_OP_DISTINCT); }
    | T_OP_LESSER                           { $$ = n_operand(T_OP_LESSER); }
    | T_OP_GREATER                          { $$ = n_operand(T_OP_GREATER); }
    | T_OP_LESSER_EQ                        { $$ = n_operand(T_OP_LESSER_EQ); }
    | T_OP_GREATER_EQ                       { $$ = n_operand(T_OP_GREATER_EQ); }
    | T_OP_AND                              { $$ = n_operand(T_OP_AND); }
    | T_OP_OR                               { $$ = n_operand(T_OP_OR); }
    | T_OP_ASSIGN                           { $$ = n_operand(T_OP_ASSIGN); }
    | T_AOP_PLUS                            { $$ = n_operand(T_AOP_PLUS); }
    | T_AOP_MINUS                           { $$ = n_operand(T_AOP_MINUS); }
    | T_AOP_MUL                             { $$ = n_operand(T_AOP_MUL); }
    | T_AOP_DIV                             { $$ = n_operand(T_AOP_DIV); }
    ;


then: T_LLA_I lineas T_LLA_D               { $$ = $2; }
    ;

else: T_ELSE T_LLA_I lineas T_LLA_D        { $$ = $3; }
    | /* lambda */                         { $$ = NULL; }
    ;

while: T_WHILE cond then                   { $$ = n_while($2, $3); }
    ;
    /*
decl: type vars                            { $$ = NULL; }
    ;

type: T_TYPE_NUMBER                        { $$ = NULL; }
    | T_TYPE_STRING                        { $$ =  }
    ;

vars: id T_COMMA vars                      { $$ = NULL; }
    | id                                   { $$ = $1; }
    ;

*/

%%

void yyerror(char *s)
{
    syntax_error(st_line_number(), s);
}
