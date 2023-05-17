CFLAGS = -Wall
DEBUG = -ggdb

termfogy: termfogy.c myinclude.h  colors.h
	gcc $(DEBUG) $(CFLAGS) termfogy.c -o termfogy

all: termfogy 

clean: 
	rm -f termfogy
