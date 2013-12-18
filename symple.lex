%option outfile="flex.c" header-file="flex.h"
%option reentrant
%option bison-bridge
%option noyywrap nounput noinput

%{
#include "bison.h"
#define OP(PREC) *yylval = val_new_s(yytext); return PREC
%}

%%

[0-9]*                    return *yylval = val_new_num(yytext), NUM;
[[:alpha:]_][[:alnum:]_]* return *yylval = val_new_s(yytext), ID;
\+  OP(OP_ADD);
-   OP(OP_ADD);
\*  OP(OP_MUL);
\/  OP(OP_MUL);
\^  OP(OP_EXP);
\(  return LPAR;
\)  return RPAR;
,   return COMMA;
#.*$      // Comment.
[ \t\r\n]*  // Whitespace.

%%
