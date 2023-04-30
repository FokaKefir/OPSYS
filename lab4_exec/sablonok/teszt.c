#include "../myinclude.h"

int main(int argc, char const *argv[]) {

    int fdin;
    int fdout;

    fdin = open("szam.txt", O_RDONLY);
    if (fdin < 0) {
        syserr("open");
    }

    if (dup2(fdin, STDIN_FILENO) < 0) {
        syserr("dup2");
    }

    fdout = open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) {
        syserr("open");
    }

    if (dup2(fdout, STDOUT_FILENO) < 0) {
        syserr("dup2");
    }

    execlp("sort", "sort", "-nr", (char *) NULL);
    syserr("sort");

    return 0;
}
