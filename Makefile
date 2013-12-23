target:
	bison symple.y
	flex symple.lex
	$(CC) -Wall -std=gnu99 -o main main.c flex.c bison.c val.c
	$(CC) -Wall -std=gnu99 -o zed zed.c flex.c bison.c val.c -lreadline -lgmp
