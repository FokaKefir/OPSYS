DEBUG = -ggdb -Wall            

dump.o: dump.c dump.h
	gcc $(DEBUG) -c dump.c
hex.o: hex.c
	gcc $(DEBUG) -c hex.c
hex: dump.o hex.o dump.h
	gcc $(DEBUG) dump.o hex.o -o hex

all: dump.o hex.o hex

clean: 
	[ -f hex ] && rm hex	
	rm *.o
