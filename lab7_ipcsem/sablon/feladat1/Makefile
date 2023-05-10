# make all mindkét programot lefordítja
# make f1 a f1-et fordítja
# make f2 a f2-et fordítja
# make clean törli a végrehajtható fájlokat

CFLAGS = -Wall
DEBUG = -ggdb

f1: f1.c myinclude.h f12.h
	gcc $(DEBUG) $(CFLAGS) f1.c -o f1

f2: f2.c myinclude.h f12.h
	gcc $(DEBUG)  $(CFLAGS) f2.c -o f2

all: f1 f2

clean: 
	rm -f f1
	rm -f f2
