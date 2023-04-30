#include "myinclude.h"

void apa(int pfd1, char *filename) {
    FILE *fin;
    fin = fopen(filename, "r");
    if (fin == NULL) {
        syserr("open");
    }

    FILE *fpipe1 = fdopen(pfd1, "w");
    if (fpipe1 == NULL) {
        syserr("open");
    }

    double num;
    while (fscanf(fin, "%lf", &num) != EOF) {
        if (fprintf(fpipe1, "%lf", num) < 0) {
            syserr("write");
        }
    }

    fclose(fpipe1);
    fclose(fin);

}

void fiu(int pfd0, char *filename) {
    FILE *fpipe0 = fdopen(pfd0, "r");
    if (fpipe0 == NULL) {
        syserr("open");
    }

    double maxi, mini;
    double s = 0;
    double num;
    int ok = -1;

    while (fscanf(fpipe0, "%lf", &num) != EOF) {
        if (ok == -1) {
            maxi = mini = num;
            ok = 0;
        } else {
            if (num > maxi) 
                maxi = num;
            if (num < mini) 
                mini = num;
        }
        s += num;
    }
    fclose(fpipe0);

    FILE *fout = fopen(filename, "w");
    if (fout == NULL) {
        syserr("open");
    }

    fprintf(fout, "Osszeg: %lf\n", s);
    fprintf(fout, "LN: %lf\n", maxi);
    fprintf(fout, "LK: %lf\n", mini);

    fclose(fout);

}

int main(int argc, char *argv[]){
    if (argc != 3) {
        syserr("arg");
    }

    if (!strcmp(argv[1], argv[2])) {
        printf("A ket file neve nem egyezhet\n");
        exit(EXIT_FAILURE);
    }
    
    int pfd[2];

    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    pid_t pid;
    pid = fork();
    if (pid == -1) {
        syserr("fork");
    }

    if (pid == 0) {
        // fiu
        if (close(pfd[1]) < 0) {
            syserr("close");
        }
        fiu(pfd[0], argv[2]);
        exit(EXIT_SUCCESS);
    } else {
        // apa
        if (close(pfd[0]) < 0) {
            syserr("close");
        }
        apa(pfd[1], argv[1]);
        wait(NULL);
    }

    return 0;
}
