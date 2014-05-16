.PHONY: target clean all

target: all

CFLAGS := -Wall -std=gnu99
RANLIB := ranlib

flex.c flex.h: symple.lex ; flex symple.lex

bison.c bison.h: symple.y ; bison symple.y

libsym_srcs := $(addsuffix .c,flex bison util val)
libsym_objs := $(libsym_srcs:.c=.o)
libsym.a: flex.h bison.h libsym.a($(libsym_objs)) ; $(RANLIB) $@

all: libsym.a
	$(CC) -Wall -std=gnu99 -o main main.c libsym.a
	$(CC) -Wall -std=gnu99 -o zed zed.c libsym.a -lreadline -lgmp
	$(CC) -Wall -std=gnu99 -o mon mon.c libsym.a ~/pro/blt/blt.c -lreadline -lgmp -I ~/pro/blt

clean: ; -rm flex.[ch] bison.[ch] *.o libsym.a mon zed main
