%output  "bison.c"
%defines "bison.h"
%define api.pure full
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { val_callback_t callback }

%code requires {
#include "val.h"
#define YYSTYPE val_ptr
#define YY_EXTRA_TYPE int
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif
}

%code {
#include "flex.h"
int yyerror(yyscan_t scanner, val_callback_t callback, const char *msg) {
  return 0;
}
}

%initial-action { yyset_extra(0, scanner); }

%token NUM ID
%left OP_ADD
%left OP_MUL
%right OP_EXP

%%

input:  // Empty
    | input expr ';' { callback($$ = $2); }

expr: unary
    | expr OP_ADD expr { $$ = val_append0($2, $1, $3, 0); }
    | expr OP_MUL expr { $$ = val_append0($2, $1, $3, 0); }
    | expr OP_EXP expr { $$ = val_append0($2, $1, $3, 0); }

unary: atom | OP_ADD atom { $$ = val_append0($1, $2, 0); }

atom: NUM | ID | '(' expr ')' { $$ = $2; }
    | ID '(' expr_list ')' { $$ = val_fun($1, $3); }

expr_list: expr { $$ = val_append(val_new(T_STRING, 0), $1); }
    | expr_list ',' expr { $$ = $1; val_append($$, $3); }

%%
