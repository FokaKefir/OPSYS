#include "myinclude.h"


typedef void (*signalhandler_t)(int);
void usr1_handler(int sig) {
    int fd1 = open("ki.txt", O_RDONLY);
    if (fd1 < 0) {
        syserr("open");
    }

    if (dup2(fd1, STDIN_FILENO) < 0) {
        syserr("dup2");
    }

    int fd2 = open("rend.txt", O_WRONLY | O_CREAT | O_EXCL, 0664);
    if (fd2 < 0) {
        syserr("open");
    }

    if (dup2(fd2, STDOUT_FILENO) < 0) {
        syserr("dup2");
    }

    execlp("sort", "sort", "-n", (void *) 0);

    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {

    if (signal(SIGUSR1, usr1_handler) == SIG_ERR) {
        syserr("signal");
    }
    
    FILE *fout = fopen("4.pid", "w");
    if (!fout) {
        syserr("fopen");
    }

    fprintf(fout, "%d", getpid());

    if (fclose(fout) != 0) {
        syserr("fclose");
    }

    while(1) {
        usleep(10000);
    }

    return 0;
}
