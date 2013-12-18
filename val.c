#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "val.h"

val_ptr val_new_num(char *s) {
  val_ptr r = malloc(sizeof(*r));
  r->type = T_INT;
  r->i = atoi(s);
  return r;
}

val_ptr val_new_s(char *s) {
  val_ptr r = malloc(sizeof(*r));
  r->type = T_STRING;
  if (s) r->s = strdup(s);
  r->nkid = 0;
  r->kid = 0;
  return r;
}

val_ptr val_new_op(val_ptr op, ...) {
  assert(op->type == T_STRING);
  val_ptr r = malloc(sizeof(*r));
  r->type = T_STRING;
  r->s = strdup(op->s);
  val_free(op);
  r->kid = 0;
  r->nkid = 0;
  va_list params;
  va_start(params, op);
  for(;;) {
    val_ptr v = va_arg(params, val_ptr);
    if (!v) break;
    r->nkid++;
    r->kid = realloc(r->kid, r->nkid * sizeof(struct val_s));
    r->kid[r->nkid - 1] = v;
  }
  va_end(params);
  return r;
}

void val_append(val_ptr list, val_ptr v) {
  list->nkid++;
  list->kid = realloc(list->kid, list->nkid * sizeof(struct val_s));
  list->kid[list->nkid - 1] = v;
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
