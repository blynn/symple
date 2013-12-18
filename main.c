#include <stdio.h>

#include "bison.h"
#include "flex.h"

int yyerror(yyscan_t scanner, const char *msg) {
  return 0;
}

int f(struct val_s *v) {
  printf("num: %d\n", v->i);
  return 0;
}

int main() {
  yyscan_t scanner;
  if (yylex_init(&scanner)) exit(1);
  YY_BUFFER_STATE buf = yy_scan_string("42", scanner);
  if (yyparse(scanner, f)) exit(1);
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return 0;
}
