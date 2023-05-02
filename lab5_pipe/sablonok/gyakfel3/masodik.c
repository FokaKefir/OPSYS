#include "myinclude.h"

void fel3(int pfd1) {
    if (access("a.fifo", F_OK)) {
        if (mkfifo("a.fifo", 0644) < 0) {
            syserr("fifo");
        }
    }

    int fifo = open("a.fifo", O_RDONLY);
    if (fifo < 0) {
        syserr("open fifo");
    }

    FILE *fout = fdopen(pfd1, "w");
    if (fout == NULL) {
        syserr("open");
    }

    float k;
    while(read(fifo, &k, sizeof(float)) > 0) {
        if (k >= 0.7) {
            fprintf(fout, "%f\n", k);
        }
    }


    if (close(fifo) < 0) {
        syserr("close fifo");
    }

    if (unlink("a.fifo") < 0){
        syserr("unlink");
    }

    if (fclose(fout) != 0) {
        syserr("close pipe");
    }

}

void fel4(int pfd0) {
    if (dup2(pfd0, STDIN_FILENO) < 0) {
        syserr("dup2");
    }

    int fd = open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        syserr("open");
    } 
    if (dup2(fd, STDOUT_FILENO) < 0) {
        syserr("dup2");
    }

    execlp("sort", "sort", "-nr", (char *) 0);
    syserr("exec sort");
}

int main(int argc, char const *argv[]){
    int pfd[2];
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    pid_t pid4 = fork();
    if (pid4 < 0) {
        syserr("fork");
    } 
    if (pid4 == 0) {
        // fiu folyamat (4)
        if (close(pfd[1]) < 0) {
            syserr("close pipe");
        }
        fel4(pfd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // apa folyamat (3)
        if (close(pfd[0]) < 0) {
            syserr("close pipe");
        }
        fel3(pfd[1]);
        wait(NULL);
    }

    pid_t pid5 = fork();
    if (pid5 < 0) {
        syserr("fork");
    } 
    if (pid5 == 0) {
        // fiu folyamat (5)
        execlp("head", "head", "-1", "ki.txt", (char *) 0);
        exit(EXIT_SUCCESS);
    } 
    // apa folyamat (3)
    wait(NULL);
    
    return 0;
}
