#include "myinclude.h"

#define BUFF_SIZE 1024
#define FIFO_NAME "a.fifo"

/**
 * mkfifo a.fifo
*/

int main(int argc, char const *argv[]) {
    
    // vizsgalja hogy a fifo letezik-e
    if (access(FIFO_NAME, F_OK)) {
        syserr("fifo");
    }

    // megnyitja a fifo-t
    int fifo = open(FIFO_NAME, O_RDONLY);
    if (fifo < 0) {
        syserr("open fifo");
    }

    // olvasunk a fifo-rol
    int n = 0;
    char buff[BUFF_SIZE];
    while((n = read(fifo, buff, BUFF_SIZE)) > 0) {
        // ir a standard output-ra (printf nem jo)
        if (write(STDOUT_FILENO, buff, n) != n) {
            syserr("write");
        }
    }
    if (n != 0) {
        syserr("read");
    }

    // zarja a fifo-t
    if (close(fifo) < 0) {
        syserr("close fifo");
    } 

    return 0;
}
