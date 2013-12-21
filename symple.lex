%option outfile="flex.c" header-file="flex.h"
%option reentrant bison-bridge
%option noyywrap nounput noinput

%{
#include "bison.h"
#define RET(TOKEN) return yyextra = TOKEN
#define S(TOKEN) *yylval = val_new(T_STRING, yytext); RET(TOKEN)
static int can_end_line(int n) {
  return n == NUM || n == ID || n == ')';
}
%}

%%

[0-9]*                    *yylval = val_new(T_INT, yytext); RET(NUM);
[[:alpha:]_][[:alnum:]_]* S(ID);
\+  S(OP_ADD);
-   S(OP_ADD);
\*  S(OP_MUL);
\/  S(OP_MUL);
\^  S(OP_EXP);
#.*$      // Comment.
[ \t\r]*  // Whitespace.
\n  if (can_end_line(yyextra)) return ';';
. return yytext[0];

%%
