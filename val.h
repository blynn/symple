enum {
  T_INT,
  T_STRING,
};

struct val_s {
  int type;
  union {
    int i;
    struct {
      char *s;
      struct val_s **kid;
      int nkid;
    };
  };
};
typedef struct val_s *val_ptr;
typedef int (*val_callback_t)(struct val_s *);

val_ptr val_new_num(char *s);
val_ptr val_new_s(char *s);
val_ptr val_new_op(val_ptr op, ...);
void val_append(val_ptr list, val_ptr v);
void val_free(val_ptr v);
