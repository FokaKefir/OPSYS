#include "myinclude.h"
#include <string.h>

void bfeladat() {
    // b feladat
    while (access("ki.txt", F_OK) == -1) {
        usleep(300000);
    } 
    usleep(200000);

    int fd = open("ki.txt", O_RDONLY);
    if (fd < 0) {
        syserr("open");
    }

    char buff[100];
    int n;
    if ((n = read(fd, buff, 100)) < 0) {
        syserr("read");
    }

    if ((write(STDOUT_FILENO, buff, n)) != n) {
        syserr("write");
    }

    close(fd);
}

void dfeladat(char *buff) {
    // d feladat
    char nevemD[] = "A nevem D\n";

    strcat(buff, nevemD);

    int fd = open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        syserr("open");
    }

    int fsize = strlen(buff);
    if((write(fd, buff, fsize)) != fsize) {
        syserr("write");
    }
    close(fd);
}

void cfeladat(char *buff) {
    char nevemC[] = "A nevem C\n";
    // masolas
    strcat(buff, nevemC);

    pid_t pidD;
    if ((pidD = fork()) < 0) {
        syserr("D");
    }
    if (pidD == 0) {
        // D folyamat
        dfeladat(buff);
        exit(EXIT_SUCCESS);
    }
    wait(NULL);
}

int main(int argc, char const *argv[]) {
    pid_t pidB, pidC;
    char buff[100];
    char nevemA[] = "A nevem A\n";

    // A folyamat eleje
    strcpy(buff, nevemA);

    if ((pidC = fork()) < 0) {
        syserr("C");
    }
    if (pidC == 0) {
        // C folyamat
        cfeladat(buff);
        exit(EXIT_SUCCESS);
    }

    
    if ((pidB = fork()) < 0) {
        syserr("B");
    }
    if (pidB == 0) {
        // B folyamat
        bfeladat();
        exit(EXIT_SUCCESS);
    }

    // A folyamat folytatodik
    wait(NULL);
    wait(NULL);

    return 0;
}
