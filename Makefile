CC=gcc
TESTS=$(wildcard *_test)
CFLAGS= -Wall -g -O2

all: kr_skiplist_test kr_conhash_test

kr_skiplist.c : kr_skiplist.h

kr_skiplist_test.c: kr_skiplist.h kr_skiplist.c

kr_conhash.c: kr_skiplist.h kr_skiplist.c kr_conhash.h

%.o : %.c
	$(CC) $(CFLAGS) -c $<

kr_skiplist_test: kr_skiplist_test.c kr_skiplist.o
	$(CC) $(CFLAGS) $^ -o $@	

kr_conhash_test: kr_conhash_test.c kr_skiplist.o kr_conhash.o
	$(CC) $(CFLAGS) $^ -o $@

test: kr_skiplist_test kr_conhash_test
	./kr_skiplist_test
	./kr_conhash_test

clean:
	rm -rf *.o *_test
