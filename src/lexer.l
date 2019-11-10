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
entero              { return T_TYPE_NUMBER; }
cadena				{ return T_TYPE_STRING; }
(\'.*\'|\".*\")     { 
						yylval.string = strdup(yytext); 
						return T_STRING; 
					}
\-?[0-9]+				{ 
						yylval.integer = atoi(yytext); 
						return T_INTEGER; 
					}
\-?[0-9]*\.[0-9]+		{ 
						yylval.number = (double)*yytext; 
						return T_NUMBER; 
					}
si                 	{ return T_IF; }
sino               	{ return T_ELSE; }
mientras			{ return T_WHILE; }
para				{ return T_FOR; }
desde				{ return T_FROM; }
hasta				{ return T_TO; }
mostrar				{ return T_ECHO; }
pedir				{ return T_INPUT; }
cargardesdearchivo	{ return T_LOAD_ARRAY; }
cada				{ return T_FOREACH; }
en					{ return T_IN; }
terminar			{ return T_EXIT; }
funcion				{ return T_FUNCTION; }
retornar			{ return T_RETURN; }
\{                  { return T_LLA_I; }
\}                  { return T_LLA_D; }
\(                  { return T_PAR_I; }
\)                  { return T_PAR_D; }
\[                  { return T_BRA_I; }
\]                  { return T_BRA_D; }
,                   { return T_COMMA; }
\<=					{ return T_OP_ASSIGN; }
\<\+				{ return T_OP_ACUMULATE_PLUS; }
\<\-				{ return T_OP_ACUMULATE_MINUS; }
\>\<				{ return T_OP_CONCAT; }
igual		        { return T_OP_EQUAL; }
distinto	        { return T_OP_DISTINCT; }
menor		        { return T_OP_LESSER; }
mayor  	            { return T_OP_GREATER; }
menoroigual         { return T_OP_LESSER_EQ; }
mayoroigual         { return T_OP_GREATER_EQ; }
y 			        { return T_OP_AND; }
o 			        { return T_OP_OR ; }
\+                  { return T_AOP_PLUS; }
\-                  { return T_AOP_MINUS; }
\*                  { return T_AOP_MUL; }
\/                  { return T_AOP_DIV; }
potencia	        { return T_AOP_POW; }
;					{ return T_SEMICOLON; }
[a-zA-Z][a-zA-Z0-9]* {
                        yylval.id = strdup(yytext);
                        return T_ID;
                    }
[\n]             { st_newline(); }
[\t ]	;
.                   {   return lexer_error(line_number, yytext);  }

%%

