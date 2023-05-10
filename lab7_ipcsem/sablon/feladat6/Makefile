# mindent lefordít:
#    make all

CFLAGS = -Wall
DEBUG = -ggdb

randlimit.o: randlimit.c randlimit.h
	gcc $(DEBUG) $(CFLAGS) -c randlimit.c 

autok: autok.c myinclude.h randlimit.o
	gcc $(DEBUG)  $(CFLAGS) randlimit.o  autok.c -o autok

all: randlimit.o autok

clean: 
	rm -f autok randlimit.o
