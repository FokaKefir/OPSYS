#include "../myinclude.h"

int main(int argc, char const *argv[]){

    pid_t pid = fork();
    if (pid == 0) {
        // fiu folyamat
        int fdin = open("szam.txt", O_RDONLY);
        if (fdin < 0) {
            syserr("open");
        }

        if (dup2(fdin, STDIN_FILENO) < 0) {
            syserr("dup2");
        }

        execlp("sort", "sort", "-nr", "-o ki.txt", (char *) NULL);
        syserr("sort");

    } else {
        // apa folyamat
        wait(NULL);
        if (access("ki.txt", F_OK) == 0) {
            execlp("head", "head", "-1", "ki.txt", (char *) NULL);
            syserr("head");
        }

    }


    return EXIT_SUCCESS;
}
