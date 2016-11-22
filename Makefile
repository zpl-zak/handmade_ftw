CC=gcc
CFLAGS=-std=c99 -Wall

all:
	$(CC) $(CFLAGS) test.cpp -o test.exe