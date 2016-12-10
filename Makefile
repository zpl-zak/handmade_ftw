CC=clang
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -Wno-unused-function -Wno-missing-field-initializers -Wno-missing-braces

all:
	$(CC) $(CFLAGS) test.c -o test
