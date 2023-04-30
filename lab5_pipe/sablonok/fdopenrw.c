#include "myinclude.h"

void father(int pfd1) {
    FILE *fpfd1 = fdopen(pfd1, "w");
    if (fpfd1 == NULL) {
        syserr("open");
    }

    FILE *fin = fopen("be12.txt", "r");
    if (fin == NULL) {
        syserr("open");
    }

    double a, b;
    while(fscanf(fin, "%lf %lf", &a, &b) != EOF) {
        if (fprintf(fpfd1, "%lf %lf", a, b) < 0) {
            syserr("write");
        } 
    }

    fclose(fin);
    fclose(fpfd1);
}

void child(int pfd0) {
    FILE *fpfd0 = fdopen(pfd0, "r");
    if (fpfd0 == NULL) {
        syserr("open");
    }

    double a, b;
    while (fscanf(fpfd0, "%lf %lf", &a, &b) != EOF) {
        if (a > b) {
            printf("%lf\n", a);
        } else {
            printf("%lf\n", b);
        }
    }

    fclose(fpfd0);
}


int main(int argc, char const *argv[]) {
    int pfd[2];

    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    pid_t pid = fork();
    if (pid < 0) {
        syserr("fork");
    }
    if (pid == 0) {
        // fiu folyamat
        if (close(pfd[1]) < 0) {
            syserr("close");
        }
        child(pfd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // apa folyamat
        if (close(pfd[0] < 0)) {
            syserr("close");
        }
        father(pfd[1]);
        wait(NULL);
    }

    return 0;
}
