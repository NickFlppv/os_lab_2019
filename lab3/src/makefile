CC=gcc
CFLAGS=-I.

all: utils.o find_min_max.o utils.h find_min_max.h parallel_min_max sequential_min_max

exec_seq_min_max: sequential_min_max
	$(CC) -o exec_seq_min_max -c exec_seq_min_max.c
	$(CC) exec_seq_min_max -o exec_seq_min_max.out
	./exec_seq_min_max.out 1 10

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c -std=c99 find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max
