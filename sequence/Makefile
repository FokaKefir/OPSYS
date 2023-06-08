CFLAGS=-Wall

all: 
server: server.c myinclude.h sequence.h
	gcc $(CFLAGS) server.c -o server
client: client.c myinclude.h sequence.h
	gcc $(CFLAGS) client.c -o client
all:	server client
clean:   
	rm -f server 
	rm -f client
