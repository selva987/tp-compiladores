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
    struct ast_node* node;
}

%token <id> T_ID
%token T_IF T_LLA_I T_LLA_D T_PAR_I T_PAR_D T_ELSE T_TYPE_NUMBER T_COMMA
%type <node> linea lineas sigma id if cond then else decl type vars var

%precedence T_ID

%%

sigma: lineas                               { $$ = $1; ast = $1; }
     ;

lineas: linea lineas                        { $$ = n_blocks($1, $2); }
      | /* lamda */                         { $$ = n_blocks(NULL, NULL); }
      ;

linea: if                                   { $$ = $1; }
     | id                                   { $$ = $1; }
     | decl                                 { $$ = NULL; }
     ;

id: T_ID                                    { $$ = n_id($1, 0); }
  ;

if: T_IF cond then else                     { $$ = n_if($2, $3, $4); }
  ;

cond: T_PAR_I T_ID T_PAR_D                  { $$ = n_id($2, 0); }
    ;

then: T_LLA_I lineas T_LLA_D               { $$ = $2; }
    ;

else: T_ELSE T_LLA_I lineas T_LLA_D        { $$ = $3; }
    | /* lambda */                         { $$ = NULL; }
    ;

decl: type vars                            { $$ = NULL; }
    ;

type: T_TYPE_NUMBER                        { $$ = NULL; }
    ;

vars: id T_COMMA vars                      { $$ = NULL; }
    | id                                   { $$ = $1; }
    ;



%%

void yyerror(char *s)
{
    syntax_error(st_line_number(), s);
}
