#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
// bison.h includes val.h
#include "bison.h"
#include "flex.h"

val_ptr val_new(int type, char *s) {
  val_ptr r = malloc(sizeof(*r));
  r->type = type;
  if (s) r->s = strdup(s);
  r->nkid = 0;
  r->kid = 0;
  return r;
}

val_ptr val_append(val_ptr op, val_ptr v) {
  op->nkid++;
  op->kid = realloc(op->kid, op->nkid * sizeof(struct val_s));
  op->kid[op->nkid - 1] = v;
  return op;
}

val_ptr val_append0(val_ptr op, ...) {
  assert(op->type == T_STRING);
  va_list params;
  va_start(params, op);
  for(;;) {
    val_ptr v = va_arg(params, val_ptr);
    if (!v) break;
    val_append(op, v);
  }
  va_end(params);
  return op;
}

val_ptr val_fun(val_ptr fun, val_ptr args) {
  assert(fun->type == T_STRING);
  assert(!fun->nkid);
  args->s = fun->s;
  free(fun);
  return args;
}

void val_free(val_ptr v) {
  switch(v->type) {
  case T_STRING:
    for (int i = 0; i < v->nkid; i++) {
      val_free(v->kid[i]);
    }
    free(v->s);
    free(v->kid);
    break;
  case T_INT:
    break;
  }
  free(v);
}

void val_parse_forall(char *s, val_callback_t callback) {
  yyscan_t scanner;
  if (yylex_init(&scanner)) exit(1);
  YY_BUFFER_STATE buf = s ? yy_scan_string(s, scanner) : 0;
  if (yyparse(scanner, callback)) exit(1);
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
}
