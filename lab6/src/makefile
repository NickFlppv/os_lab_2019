CC=gcc
CFLAGS=-I.
all : cli serv
cli : modul.o modul.h
	$(CC) -o cli modul.o client.c -lpthread $(CFLAGS)

serv : modul.o modul.h
	$(CC) -std=c99 -o serv modul.o server.c -lpthread $(CFLAGS)
	
modul.o : modul.h
	$(CC) -o modul.o -c modul.c $(CFLAGS)

clean :
	rm -f cli serv modul.o