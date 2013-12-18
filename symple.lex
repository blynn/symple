%option outfile="flex.c" header-file="flex.h"
%option reentrant
%option bison-bridge
%option noyywrap nounput noinput

%{
#include "bison.h"
%}

%%

[0-9]*  { *yylval = val_new_num(yytext); return NUM; }
\n

%%
