#make allomany a recv es send allomanyok forditasahoz
CFLAGS=-g -Wall

all: circbuff.o pufferfun.o circbuff

pufferfun.o: pufferfun.c pufferfun.h
	gcc  $(CFLAGS) -c pufferfun.c

circbuff.o: circbuff.c myinclude.h pufferfun.h
	gcc  $(CFLAGS) -c circbuff.c

circbuff: circbuff.o pufferfun.o 
	gcc  $(CFLAGS) circbuff.o pufferfun.o -o circbuff

clean:  
	rm -f *.o circbuff

