# make all mindkét programot lefordítja

CFLAGS = -Wall

fiforead: fiforead.c myinclude.h
	gcc $(DEBUG) $(CFLAGS) fiforead.c -o fiforead

fifowrite: fifowrite.c myinclude.h
	gcc $(DEBUG)  $(CFLAGS) fifowrite.c -o fifowrite

all: fifowrite fiforead

clean: 
	[ -f fiforead ] && rm fiforead
	[ -f fifowrite ] && rm fifowrite
