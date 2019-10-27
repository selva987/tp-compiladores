%{
    #include "y.tab.h"
    #include <stdlib.h>
    void yyerror(char *);

    int st_newline();
    int lexer_error(int line_number, char *lexem);
    extern int line_number;
%}

%option noyywrap

%%
number              { return T_TYPE_NUMBER; }
\if                 { return T_IF; }
\else               { return T_ELSE; }
\{                  { return T_LLA_I; }
\}                  { return T_LLA_D; }
\(                  { return T_PAR_I; }
\)                  { return T_PAR_D; }
,                   { return T_COMMA; }

[a-zA-Z][a-zA-Z0-9]* {
                        yylval.id = strdup(yytext);
                        return T_ID;
                    }
[\n \t]             ;
.                   {   return lexer_error(line_number, yytext);  }

%%

