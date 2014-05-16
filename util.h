// Requries stdarg, stdio.

#define NORETURN __attribute__((__noreturn__))
#define ARRAY_SZ(x) (sizeof(x)/sizeof(*(x)))
#define EXPECT(condition) \
  if (condition); else fprintf(stderr, "%s:%d: FAIL\n", __FILE__, __LINE__)

extern void die(const char *err, ...) NORETURN
    __attribute__((format (printf, 1, 2)));
extern void die_perror(const char *err, ...) NORETURN
    __attribute__((format (printf, 1, 2)));
