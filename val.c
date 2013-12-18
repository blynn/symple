#include <stdlib.h>
#include "val.h"

val_ptr val_new_num(char *s) {
  val_ptr r = malloc(sizeof(*r));
  r->i = atoi(s);
  return r;
}
