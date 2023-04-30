#include "myinclude.h"

#define BUFFSIZE 4

void printBetweenPos(int fd, int start, int end, char *buff) {
    lseek(fd, start, SEEK_SET);
    int n;
    while (end - start >= BUFFSIZE){
        if ((n = read(fd, buff, BUFFSIZE)) > 0) {
            if (write(STDOUT_FILENO, buff, n) < 0) {
                syserr("write");
            }
        }
        start += BUFFSIZE;
    }
    if ((n = read(fd, buff, end - start + 1)) > 0) {
        if (write(STDOUT_FILENO, buff, n) < 0) {
            syserr("write");
        }
    }
    
}

int main(int argc, char* argv[]) {
    char buff[BUFFSIZE];
    int endls[100];
    int db = 0;

    if (argc != 2) {
        printf("használat: %s fájlnév sztring\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        syserr("open");
    }

    int n;
    int size = 0;
    while ((n = read(fd, buff, BUFFSIZE)) > 0) {
        for (int i = 0; i < n; i++){
            if (buff[i] == '\n') {
                endls[db++] = size + i;
            }
        }
        size += n;
    }

    for (int i = db - 1; i >= 1; --i) {
        printBetweenPos(fd, endls[i - 1] + 1, endls[i], buff);
    }
    printBetweenPos(fd, 0, endls[0], buff);

    if (close(fd) == -1){
        syserr("close");
    }

    exit(EXIT_SUCCESS);
}