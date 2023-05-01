#include "myinclude.h"
#include <string.h>

#define MAX_LINE_CHAR 1024
#define FIFO_NAME "a.fifo"

void fel2(int pfd1) {
    // olvassa a sorokat terminalrol
    char line[MAX_LINE_CHAR];
    while(fgets(line, MAX_LINE_CHAR, stdin) != NULL) {
        // lekeri a sor meretet
        int len = strlen(line);

        //kuldi tovabb a pipe-on
        if (write(pfd1, line, len) != len) {
            syserr("write");
        }
    }

    // zarja a pipe-t
    if (close(pfd1) < 0) {
        syserr("close pipe");
    }
}

void fel3(int pfd0) {
    // vizsgalja hogy a fifo elerheto-e
    if (access(FIFO_NAME, F_OK)) {
        syserr("fifo");
    }
    // megnyitja a fifo-t
    int fifo = open(FIFO_NAME, O_WRONLY);
    if (fifo < 0) {
        syserr("open fifo");
    }

    // a standard bemenetet atiranyitja a pipe-ra
    if (dup2(pfd0, STDIN_FILENO) < 0) {
        syserr("dup2 pipe");
    }

    // a standard kimenetet atiranyitja a fifo-ra
    if (dup2(fifo, STDOUT_FILENO) < 0) {
        syserr("dup2 fifo");
    }

    // elinditja az egrep-et
    execlp("egrep", "egrep", "^[0-9]+$", (char *) 0);

    // zarja a pipe-ot
    if (close(pfd0) < 0) {
        syserr("close pipe");
    }

    // zarja a fifo-t
    if (close(fifo) < 0) {
        syserr("close fifo");
    } 
}

int main(int argc, char const *argv[]) {
    
    // pipe letrehozasa
    int pfd[2];
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    // 2. folyamat letrehozasa
    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        if (close(pfd[0]) < 0) {
            syserr("close pipe");
        }
        fel2(pfd[1]);
        exit(EXIT_SUCCESS);
    }

    // 3. folyamat letrehozasa
    pid_t pid3 = fork();
    if (pid3 < 0) {
        syserr("fork");
    }
    if (pid3 == 0) {
        if (close(pfd[1]) < 0) {
            syserr("close pipe");
        }
        fel3(pfd[0]);
        exit(EXIT_SUCCESS);
    }

    // (1) apa folyamat 
    // zarja a pipe-okat
    if (close(pfd[0]) < 0) {
        syserr("close pipe");
    }
    if (close(pfd[1]) < 0) {
        syserr("close pipe");
    }

    // bevarja a fiu folyamatokat 
    wait(NULL);
    wait(NULL);

    return 0;
}
