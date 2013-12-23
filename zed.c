#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <gmp.h>
#include "val.h"

static void eval(val_ptr v, mpz_t z) {
  if (v->type == T_INT) {
    mpz_set_str(z, v->s, 0);
    return;
  }
  if (v->nkid == 2) {
    mpz_t x, y;
    mpz_init(x), mpz_init(y);
    eval(v->kid[0], x);
    eval(v->kid[1], y);
    switch(v->s[0]) {
    case '+': mpz_add(z, x, y); break;
    case '-': mpz_sub(z, x, y); break;
    case '*': mpz_mul(z, x, y); break;
    case '/': mpz_tdiv_q(z, x, y); break;
    case '^':
      // TODO: Handle y < 0, large y.
      mpz_pow_ui(z, x, mpz_get_ui(y));
      break;
    default:
      exit(1);
    }
    mpz_clear(x), mpz_clear(y);
  } else if (v->nkid == 1) {
    eval(v->kid[0], z);
    if (v->s[0] == '-') mpz_neg(z, z);
  }
}

int main(int argc, char *argv[]) {
  int f(val_ptr v) {
    mpz_t z;
    mpz_init(z);
    eval(v, z);
    mpz_out_str(stdout, 0, z);
    putchar('\n');
    mpz_clear(z);
    return 0;
  }
  for(char *line; (line = readline(">")); free(line)) {
    if (*line) {
      add_history(line);
      val_parse_forall(line, f);
    }
  }
  return 0;
}
