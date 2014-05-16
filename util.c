#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void die(const char *err, ...) {
  va_list params;

  va_start(params, err);
  vfprintf(stderr, err, params);
  fputc('\n', stderr);
  va_end(params);
  exit(1);
}

void die_perror(const char *err, ...) {
  va_list params;

  va_start(params, err);
  vfprintf(stderr, err, params);
  fputc('\n', stderr);
  va_end(params);
  perror("errmsg");
  exit(1);
}
