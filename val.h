enum {
  T_INT,
  T_STRING,
};

struct val_s {
  int type;
  struct {
    char *s;
    struct val_s **kid;
    int nkid;
  };
};
typedef struct val_s *val_ptr;
typedef int (*val_callback_t)(val_ptr);

val_ptr val_new(int type, char *s);
void val_free(val_ptr v);
val_ptr val_fun(val_ptr fun, val_ptr args);

// Adds a child 'v' to 'op'.
val_ptr val_append(val_ptr op, val_ptr v);

// Adds a NULL-terminated list of children to 'op'.
val_ptr val_append0(val_ptr op, ...);

// Parses expressions from a given string, and calls a given callback on each.
// If 's' is NULL, then parses from standard input.
void val_parse_forall(char *s, val_callback_t callback);
