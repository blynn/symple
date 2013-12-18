%output  "bison.c"
%defines "bison.h"
%define api.pure full
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { val_callback_t callback }

%code requires {
#include "val.h"
#define YYSTYPE val_ptr
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
int yylex();
int yyerror();
}

%token NUM

%%

stmt: foo {
    callback($1);
    $$ = $1;
  }

foo: NUM

%%
