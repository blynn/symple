#include <stdio.h>
#include <string.h>
#include "val.h"

// Prints a parse tree in pre-order.
void val_print_pre(struct val_s *v) {
  fputs(v->s, stdout);
  if (v->nkid) {
    putchar('(');
    for (int i = 0; i < v->nkid; i++) {
      if (i) printf(", ");
      val_print_pre(v->kid[i]);
    }
    putchar(')');
  }
}

// Prints a parse tree in the style of 'pstree'.
// Uses UTF-8 line-drawing characters.
void val_print_tree(const char *prefix, struct val_s *v) {
  fputs(v->s, stdout);
  if (v->nkid) {
    for (int i = 0; i < v->nkid; i++) {
      char s[strlen(prefix) + 3 + strlen(v->s) + 1];
      strcpy(s, prefix);
      for (int i = strlen(v->s); i; i--) strcat(s, " ");
      if (!i) {
        if (v->nkid == 1) {
          fputs("───", stdout); strcat(s, "   ");
        } else {
          fputs("─┬─", stdout); strcat(s, " │ ");
        }
      } else {
        fputs(s, stdout);
        if (v->nkid == i + 1) {
          fputs(" └─", stdout); strcat(s, "   ");
        } else {
          fputs(" ├─", stdout); strcat(s, " │ ");
        }
      }
      val_print_tree(s, v->kid[i]);
    }
  } else {
    putchar('\n');
  }
}

int main(int argc, char *argv[]) {
  int f(val_ptr v) {
    val_print_pre(v);
    putchar('\n');
    val_print_tree("", v);
    val_free(v);
    return 0;
  }
  val_parse_forall(argc > 1 ? argv[1] : 0, f);
  return 0;
}
