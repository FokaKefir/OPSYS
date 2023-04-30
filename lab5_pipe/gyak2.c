#include "myinclude.h"

void fel1(int pfd11, int pfd21) {


    close(pfd11);
    close(pfd21);
}

void fel2(int pfd10) {

    close(pfd10);
}

void fel3(int pfd20) {

    close(pfd20);
}

int main(int argc, char const *argv[]) {
    int pfd1[2], pfd2[2];

    if (pipe(pfd1) < 0) {
        syserr("pipe");
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        syserr("fork");
    }
    if(pid1 == 0) {
        // feladat2
        if (close(pfd1[1]) < 0) {
            syserr("close");
        }
        fel2(pfd1[0]);
        exit(EXIT_SUCCESS);
    }

    if (pipe(pfd2) < 0) {
        syserr("pipe");
    }

    pid_t pid2 = fork();
    if (pfd2 < 0) {
        syserr("fork");
    }
    if(pfd2 == 0) {
        // feladat3
        if (close(pfd1[1]) < 0) {
            syserr("close");
        }
        if (close(pfd2[1]) < 0) {
            syserr("close");
        }
        fel3(pfd2[0]);


        exit(EXIT_SUCCESS);
    }

    close(pfd1[0]);
    close(pfd2[0]);

    fel1(pfd1[1], pfd2[1]);

    wait(NULL);
    wait(NULL);


    return 0;
}
