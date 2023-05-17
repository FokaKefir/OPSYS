CFLAGS = -Wall
DEBUG = -ggdb

fogyaszto: fogyaszto.c myinclude.h shm.h
	gcc $(DEBUG) $(CFLAGS) fogyaszto.c -o fogyaszto

termelo: termelo.c myinclude.h shm.h
	gcc $(DEBUG)  $(CFLAGS) termelo.c -o termelo

all: fogyaszto termelo

clean: 
	rm -f fogyaszto termelo