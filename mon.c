// Monomial calculator.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <gmp.h>
#include "blt.h"
#include "util.h"
#include "val.h"

struct mon_s {
  mpq_t q;
  BLT *deg;
};
typedef struct mon_s *mon_t;

mon_t mon_new() {
  mon_t r = malloc(sizeof(*r));
  mpq_init(r->q);
  r->deg = blt_new();
  return r;
}

int mon_deg_cmp(mon_t x, mon_t y) {
  if (blt_size(x->deg) != blt_size(y->deg)) return 1;
  return !blt_allprefixed(x->deg, "", ({int _(BLT_IT *it) {
    BLT_IT *yit = blt_get(y->deg, it->key);
    if (!yit) return 0;
    return yit->data == it->data;
  }_;}));
}

BLT* global_var;
intptr_t global_n;

static void eval(val_ptr v, mon_t x) {
  if (v->type == T_INT) {
    mpz_t z;
    mpz_init(z);
    mpz_set_str(z, v->s, 0);
    mpq_set_z(x->q, z);
    mpz_clear(z);
    return;
  }
  if (v->nkid == 2) {
    mon_t y = mon_new();
    eval(v->kid[0], x);
    eval(v->kid[1], y);
    switch(v->s[0]) {
    case '-':
      mpq_neg(y->q, y->q);
      // FALLTHROUGH
    case '+':
      if (mon_deg_cmp(x, y)) die("mismatch");
      mpq_add(x->q, x->q, y->q);
      break;
    case '*':
      mpq_mul(x->q, x->q, y->q);
      blt_forall(y->deg, ({void _(BLT_IT *it) {
        BLT_IT *rit = blt_get(x->deg, it->key);
        if (!rit) {
          blt_put(x->deg, it->key, it->data);
        } else {
          intptr_t sum = (intptr_t) rit->data + (intptr_t) it->data;
          if (!sum) {
            blt_delete(x->deg, it->key);
          } else {
            blt_put(x->deg, it->key, (void *) sum);
          }
        }
      }_;}));
      break;
    case '/':
      mpq_div(x->q, x->q, y->q);
      blt_forall(y->deg, ({void _(BLT_IT *it) {
        BLT_IT *rit = blt_get(x->deg, it->key);
        if (!rit) {
          blt_put(x->deg, it->key, (void *) -((intptr_t) it->data));
        } else {
          intptr_t diff = (intptr_t) rit->data - (intptr_t) it->data;
          if (!diff) {
            blt_delete(x->deg, it->key);
          } else {
            blt_put(x->deg, it->key, (void *) diff);
          }
        }
      }_;}));
      break;
    case '^':
      if (!blt_empty(y->deg)) die("nonconstant exponent");
      if (mpz_cmp_ui(mpq_denref(y->q), 1)) die("fractional exponent");
      int sgn = mpq_sgn(y->q);
      if (sgn < 0) mpq_neg(y->q, y->q);
      unsigned int pwr = mpz_get_ui(mpq_numref(y->q));
      mpz_ptr p = mpq_numref(x->q);
      mpz_pow_ui(p, p, pwr);
      p = mpq_denref(x->q);
      mpz_pow_ui(p, p, pwr);
      if (sgn < 0) {
        mpz_swap(mpq_numref(x->q), mpq_denref(x->q));
        if (mpz_sgn(mpq_denref(x->q)) < 0) {
          mpz_neg(mpq_numref(x->q), mpq_numref(x->q));
          mpz_neg(mpq_denref(x->q), mpq_denref(x->q));
        }
      }
      blt_forall(x->deg, ({void _(BLT_IT *it) {
        it->data = (void *) (sgn * pwr * (intptr_t) it->data);
      }_;}));
      break;
    default: die("bad op:%s", v->s);
    }
    free(y);
  } else if (v->nkid == 1) {
    eval(v->kid[0], x);
    if (v->s[0] == '-') mpq_neg(x->q, x->q);
  } else {
    EXPECT(!v->nkid);
    if (!blt_put_if_absent(global_var, v->s, (void *) global_n)) global_n++;
    mpq_set_ui(x->q, 1, 1);
    blt_put(x->deg, v->s, (void *) 1);
  }
}

void mon_out_str(FILE* fp, mon_t r) {
  if (mpq_cmp_ui(r->q, 1, 1) || blt_empty(r->deg)) {
    mpq_out_str(fp, 0, r->q);
  }
  blt_forall(r->deg, ({void _(BLT_IT *it) {
    fputs(it->key, fp);
    int d = (intptr_t) it->data;
    if (d != 1) fprintf(fp, "^%d", d);
  }_;}));
}

int main(int argc, char *argv[]) {
  global_var = blt_new();
  int f(val_ptr v) {
    mon_t r = mon_new();
    eval(v, r);
    mon_out_str(stdout, r);
    putchar('\n');
    free(r);
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
