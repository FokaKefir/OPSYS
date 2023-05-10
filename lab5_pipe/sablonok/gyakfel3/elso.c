#include "myinclude.h"
#include <math.h>

// fiu folyamat (2)
void fel2(int pfd1) {
    FILE *fin = fopen("d2.txt", "r");
    if (fin == NULL) {
        syserr("open");
    }
    float k = 0;
    while (fscanf(fin, "%f", &k) != EOF) {
        float buff = k;
        if (write(pfd1, (void *) &buff, sizeof(float)) != sizeof(float)) {
            syserr("write");
        }
    }

    if (fclose(fin) != 0) {
        syserr("close");
    }
    if (close(pfd1) < 0) {
        syserr("close pipe");
    }
}

// apa folyamat (1)
void fel1(int pfd0) {
    if (access("a.fifo", F_OK)) {
        if (mkfifo("a.fifo", 0644) < 0) {
            syserr("fifo");
        }
    }

    int fifo = open("a.fifo", O_WRONLY);
    if (fifo < 0) {
        syserr("open fifo");
    }

    FILE *fin = fopen("d1.txt", "r");
    if (fin == NULL) {
        syserr("open");
    }

    float k1, k2;
    while(read(pfd0, (void *) &k1, sizeof(float)) > 0 && fscanf(fin, "%f", &k2) != EOF) {
        //printf("foly1: %f\n", k);
        float res = fabsf(k1 - k2);
        if (write(fifo, &res, sizeof(float)) != sizeof(float)) {
            syserr("write fifo");
        }
    }
    if (close(pfd0) < 0) {
        syserr("close pipe");
    }
    if(fclose(fin) != 0) {
        syserr("close");
    }
    if (close(fifo) < 0) {
        syserr("close fifo");
    }

}

int main(int argc, char const *argv[]) {
    int pfd[2];
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        // fiu folyamat (2)
        if (close(pfd[0]) < 0) {
            syserr("close pipe");
        }
        fel2(pfd[1]);
        exit(EXIT_SUCCESS);
    } else {
        // apa folyamat (1)
        if (close(pfd[1]) < 0) {
            syserr("close pipe");
        }
        fel1(pfd[0]);
        wait(NULL);
    }
    return 0;
}
