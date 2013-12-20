%option outfile="flex.c" header-file="flex.h"
%option reentrant bison-bridge
%option noyywrap nounput noinput

%{
#include "bison.h"
#define OP(PREC) *yylval = val_new_s(yytext); return PREC
%}

%%

[0-9]*                    *yylval = val_new_int(yytext); return NUM;
[[:alpha:]_][[:alnum:]_]* *yylval = val_new_s(yytext); return ID;
\+  OP(OP_ADD);
-   OP(OP_ADD);
\*  OP(OP_MUL);
\/  OP(OP_MUL);
\^  OP(OP_EXP);
\(  return LPAR;
\)  return RPAR;
,   return COMMA;
#.*$      // Comment.
[ \t\r]*  // Whitespace.

%%
