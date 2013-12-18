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

%token NUM ID
%token LPAR RPAR COMMA

%left OP_ADD
%left OP_MUL
%right OP_EXP

%%

stmt: expr { callback($1); $$ = $1; }

expr: unary
    | expr OP_ADD expr { $$ = val_new_op($2, $1, $3, 0); }
    | expr OP_MUL expr { $$ = val_new_op($2, $1, $3, 0); }
    | expr OP_EXP expr { $$ = val_new_op($2, $1, $3, 0); }

unary : atom | OP_ADD atom { $$ = val_new_op($1, $2, 0); }

atom: NUM | ID | ID LPAR expr_list RPAR { $$ = $3; $$->s = $1->s; free($1); }
    | LPAR expr RPAR { $$ = $2; }

expr_list: expr { $$ = val_new_s(0); val_append($$, $1); }
    | expr_list COMMA expr { $$ = $1; val_append($$, $3); }

%%
