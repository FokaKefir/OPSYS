# make all mindkét programot lefordítja
# make filer a filer-et fordítja
# make filew a filew-et fordítja
# make clean törli a végrehajtható fájlokat

CFLAGS = -Wall
DEBUG = -ggdb

filer: filer.c myinclude.h file.h
	gcc $(DEBUG) $(CFLAGS) filer.c -o filer

filew: filew.c myinclude.h file.h
	gcc $(DEBUG)  $(CFLAGS) filew.c -o filew

all: filer filew

clean: 
	rm -f filer
	rm -f filew
