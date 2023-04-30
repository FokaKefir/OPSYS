#include "myinclude.h"
#include <string.h>

#define SIZE 1024

void feladat2(int pfd1) {
    char buff[SIZE];
    while (1) {
        if (fgets(buff, SIZE, stdin) == NULL) {
            // vege
            close(pfd1);
            return;
        }
        if (write(pfd1, buff, strlen(buff)) < 0) {
            syserr("write");
        }
    }
}

void feladat3(int pfd0) {
    if (dup2(pfd0, STDIN_FILENO) < 0) {
        syserr("dup2");
    }
    execlp("sort", "sort", "-o", "ki.txt", (char *) NULL);
    syserr("sort");
}


int main(int argc, char const *argv[]) {

    pid_t pid2, pid3;
    int pfd[2];

    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid2 = fork()) < 0) {
        syserr("fork 2");
    }
    if (pid2 == 0) {
        // 2. folyamat
        if (close(pfd[0]) < 0) {
            syserr("close");
        }
        feladat2(pfd[1]);
        exit(EXIT_SUCCESS);
    }

    if ((pid3 = fork()) < 0) {
        syserr("fork 3");
    }
    if (pid3 == 0) {
        // 3. folyamat
        if (close(pfd[1]) < 0) {
            syserr("close");
        }
        feladat3(pfd[0]);
        exit(EXIT_SUCCESS);
    }

    close(pfd[0]);
    close(pfd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}



