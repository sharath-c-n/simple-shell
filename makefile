CC=gcc
CFLAGS=-I .

shell: shell.c fileHandler.c
	$(CC) -o shell shell.c fileHandler.c  $(CFLAGS)
