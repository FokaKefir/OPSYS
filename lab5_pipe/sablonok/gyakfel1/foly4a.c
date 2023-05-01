#include "myinclude.h"

#define BUFF_SIZE 1024
#define FIFO_NAME "a.fifo"

/**
 * mkfifo a.fifo
*/

int main(int argc, char const *argv[]) {
    
    // vizsgalja hogy a fifo letezik-e
    if (access(FIFO_NAME, F_OK)) {
        //syserr("fifo");
        if (mkfifo(FIFO_NAME, 0644) < 0) {
            syserr("mkfifo");
        }
    }

    // megnyitja a fifo-t
    int fifo = open(FIFO_NAME, O_RDONLY);
    if (fifo < 0) {
        syserr("open fifo");
    }

    // atiranyitjuk a standard kimenetet fifo-ra
    if (dup2(fifo, STDIN_FILENO) < 0) {
        syserr("dup2 fifo");
    }

    // elinditjuk a sort-ot
    //execlp("sort", "sort", "-nr", (char *) 0);
    if (system("sort -nr") < 0) {
        syserr("system");
    }

    // zarja a fifo-t
    if (close(fifo) < 0) {
        syserr("close fifo");
    } 

    // torli a fifo-t
    if (unlink(FIFO_NAME) < 0) {
        syserr("unlink");
    }

    return 0;
}
