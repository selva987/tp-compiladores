%{
    /**
     * Analizador Sintáctico - Parser
     
     */

  	#include <stdio.h>
    #include "../lib/cya.h"
    #include "fn.h"

    void function_exists_error(char *name)
    {
        semantic_error(st_line_number(), name);
    }
    void unknown_function_error(char *name)
    {
        semantic_error(st_line_number(), name);
    }
%}

//%define parse.error verbose

%union {
    char* id;
    char* string;
    double number;
    int integer;
    struct ast_node* node;
}

%token <id> T_ID

%token T_IF T_WHILE T_LLA_I T_LLA_D T_PAR_I T_PAR_D T_BRA_I T_BRA_D T_ELSE T_TYPE_NUMBER T_TYPE_STRING T_COMMA T_OP_ASSIGN T_OP_ACUMULATE_PLUS T_OP_ACUMULATE_MINUS T_OP_CONCAT T_OP_EQUAL T_OP_DISTINCT T_OP_LESSER T_OP_GREATER T_OP_LESSER_EQ T_OP_GREATER_EQ T_OP_AND T_OP_OR T_AOP_PLUS T_AOP_MINUS T_AOP_MUL T_AOP_DIV T_AOP_POW T_NUMBER T_STRING T_INTEGER T_SEMICOLON T_FOR T_FROM T_TO T_ECHO T_INPUT T_LOAD_ARRAY T_FOREACH T_IN T_EXIT T_FUNCTION T_RETURN
%type <node> linea lineas sigma if cond then else /*decl type*/ while operation operand operator for echo input id loadArray foreach exit function_decl vars return function_call params

%left T_OP_ASSIGN
%left T_AOP_PLUS T_AOP_MINUS
%left T_AOP_MUL T_AOP_DIV


%%

sigma: lineas                               { $$ = $1; ast = $1; }
     ;

lineas: linea lineas                        { $$ = n_blocks($1, $2); }
      | /* lamda */                         { $$ = n_blocks(NULL, NULL); }
      ;

linea: if                                   { $$ = $1; }
     | while                                { $$ = $1; }
     | for                                  { $$ = $1; }
     | foreach                              { $$ = $1; }
     | operation T_SEMICOLON                { $$ = $1; }
     | loadArray T_SEMICOLON                { $$ = $1; }
     | exit T_SEMICOLON                     { $$ = $1; }
     | function_decl                        { $$ = $1; }
     | function_call T_SEMICOLON            { $$ = $1; }
     | return T_SEMICOLON                   { $$ = $1; }
     ;


if: T_IF cond then else                     { $$ = n_if($2, $3, $4); }
  ;

cond: T_PAR_I operation T_PAR_D             { $$ = $2; }
    ;

operation: operand operator operand         { $$ = n_operation($1, $2, $3); }
    | echo                                  { $$ = $1; }
    | input                                 { $$ = $1; }
    | id                                    { $$ = $1; }
    ;

operand: id                                 { $$ = $1; }
    | T_STRING                              { $$ = n_const_string(yylval.string); }
    | T_NUMBER                              { $$ = n_const_number(yylval.number); }
    | T_INTEGER                             { $$ = n_const_integer(yylval.integer); }
    | cond                                  { $$ = $1; }
    | function_call                         { $$ = $1; }
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
    | T_OP_ACUMULATE_PLUS                   { $$ = n_operand(T_OP_ACUMULATE_PLUS); }
    | T_OP_ACUMULATE_MINUS                  { $$ = n_operand(T_OP_ACUMULATE_MINUS); }
    | T_OP_CONCAT                           { $$ = n_operand(T_OP_CONCAT); }
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

for: T_FOR T_PAR_I id T_FROM operand T_TO operand T_PAR_D then         { $$ =  n_for($3, $5, $7, $9); }

foreach: T_FOREACH T_PAR_I T_ID T_IN id T_PAR_D then    { $$ = n_foreach(n_id($3,NULL), $5, $7); }
    ;

echo: T_ECHO T_PAR_I operand T_PAR_D       { $$ = n_echo($3); }
    ;

input: T_INPUT T_PAR_I id T_PAR_D          { $$ = n_input($3); }
    ;

id: T_ID T_BRA_I operand T_BRA_D           { $$ = n_id($1, $3); }
    | T_ID                                 { $$ = n_id($1, NULL); } 
    ;


loadArray: T_LOAD_ARRAY T_PAR_I id T_COMMA operand T_PAR_D    { $$ = n_loadArray($3, $5); }
    ;

function_decl: T_FUNCTION T_ID T_PAR_I vars T_PAR_D then    { if(st_get_symbol($2, 1) != NULL) function_exists_error($2); $$ = n_function($2, $4, $6); }
    ;

vars: T_ID T_COMMA vars                      { $$ = n_vars(n_id($1,NULL), $3); }
    | T_ID                                   { $$ = n_vars(n_id($1,NULL),NULL); }
    | /* lambda */                           { $$ = NULL; }   
    ;

function_call: T_ID T_PAR_I params T_PAR_D  { if(st_get_symbol($1, 1) == NULL) unknown_function_error($1); $$ = n_function_call($1, $3); }
    ;


params: id T_COMMA vars                      { $$ = n_vars($1, $3); }
    | id                                     { $$ = n_vars($1,NULL); }
    | /* lambda */                           { $$ = NULL; }   
    ;


exit: T_EXIT T_PAR_I T_INTEGER T_PAR_D      { $$ = n_exit(yylval.integer); }
    ;

return: T_RETURN T_PAR_I operand T_PAR_D    { $$ = n_return($3); }
    ;

    

%%

void yyerror(char *s)
{
    syntax_error(st_line_number(), s);
}
