CC=gcc
CFLAGS=-I.
all : client server

client : 
	$(CC) -o client tcp_udp_client.c $(CFLAGS)

server : 
	$(CC) -o server tcp_udp_server.c $(CFLAGS)
	
clean :
	rm -f client server