#include "myinclude.h"

// apa folyamat
void fel1(int pfd11, int pfd21) {
    FILE *fin = fopen("be.txt", "r");
    if (fin == NULL) {
        syserr("open");
    }

    FILE *fpfd21 = fdopen(pfd21, "w");
    if (fpfd21 == NULL) {
        syserr("open");
    }

    int k;
    while (fscanf(fin, "%d", &k) != EOF) {
        int k1, k2;
        k1 = k;
        k2 = k;

        if (write(pfd11, (void *) &k1, sizeof(k1)) != sizeof(k1)) {
            syserr("write");
        }
        //if (write(pfd21, (void *) &k2, sizeof(k2)) != sizeof(k2)) {
        //    syserr("write");
        //}
        if (fprintf(fpfd21, "%d\n", k2) < 0) {
            syserr("print");
        }
    }

    if (fclose(fin)){
        syserr("close");
    }

    close(pfd11);
    fclose(fpfd21);
}

// fiu folyamat (ki.txt)
void fel2(int pfd10) {
    // letrehozzuk a kimeneti file-t
    FILE *fout = fopen("ki.txt", "w");
    if (fout == NULL) {
        syserr("open");
    }

    // olvassuk k-ba a szamot
    int k;
    while(read(pfd10, (void *) &k, sizeof(k)) != 0) {
        // kiirjuk a file-ba
        fprintf(fout, "%d ", k);
    }

    // zarjuk a file-t
    if (fclose(fout)) {
        syserr("close");
    }

    // zarjuk a csovezeteket
    close(pfd10);
}

// fiu folyamat (awk)
void fel3(int pfd20) {
    // bemenetet atiranyitjuk a pipe2-re
    if (dup2(pfd20, STDIN_FILENO) < 0) {
        syserr("dup2");
    }

    // letrehozzuk a kimeneti sum1.txt file-t
    int fd = open("sum1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        syserr("open");
    }

    // a kimenetet atiranyitjuk a sum1.txt file-ra
    if (dup2(fd, STDOUT_FILENO) < 0) {
        syserr("dup2");
    }

    // awk futtatasa a exec segitsegevel
    //execlp("awk", "awk", "'{s+=$1}END{print s}'", (char *) 0);
    execlp("sort", "sort", "-n", (char *) 0);

    // zarjuk a file-t
    if (close(fd) < 0) {
        syserr("close");
    }

    // zarjuk a csovezeteket
    close(pfd20);
}

int main(int argc, char const *argv[]) {
    int pfd1[2], pfd2[2];

    if (pipe(pfd1) < 0) {
        syserr("pipe");
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if(pid2 == 0) {
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

    pid_t pid3 = fork();
    if (pid3 < 0) {
        syserr("fork");
    }
    if(pid3 == 0) {
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
