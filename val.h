struct val_s {
  char *s;
  int i;
};
typedef struct val_s *val_ptr;
typedef int (*val_callback_t)(struct val_s *);

val_ptr val_new_num(char *s);
