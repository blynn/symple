#include <stdio.h>

#include "bison.h"
#include "flex.h"

int yyerror(yyscan_t scanner, const char *msg) {
  return 0;
}

void val_print(struct val_s *v) {
  switch(v->type) {
  case T_INT: printf("%d", v->i); break;
  case T_STRING:
    fputs(v->s, stdout);
    if (v->nkid) {
      putchar('(');
      for (int i = 0; i < v->nkid; i++) {
        if (i) printf(", ");
        val_print(v->kid[i]);
      }
      putchar(')');
    }
    break;
  }
}

int f(struct val_s *v) {
  val_print(v);
  putchar('\n');
  val_free(v);
  return 0;
}

int main() {
  yyscan_t scanner;
  if (yylex_init(&scanner)) exit(1);
  //YY_BUFFER_STATE buf = yy_scan_string("42*31", scanner);
  if (yyparse(scanner, f)) exit(1);
  //yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return 0;
}
