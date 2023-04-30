#include "myinclude.h"

#define BUFFSIZE 8

int main(int argc, char * argv[]){
    if (argc<3) {
        fprintf(stderr,"használat: %s fájlnév\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0644);

    if (fd1 < 0){
        syserr("open");
    }

    if (fd2 < 0) {
        syserr("open");
    }

    char buff[BUFFSIZE];
    int n;

    while ((n = read(fd1, buff, BUFFSIZE)) > 0) {
        for (int i = 0; i < n; i++) {
            if (buff[i] != 0) {
                int num = buff[i];
                lseek(fd2, num * sizeof(int), SEEK_SET);
                if (write(fd2, &num, 1) != 1) {
                    syserr("write");
                }
            }
        } 
    }

    if (close(fd1) == -1) {
        syserr("close");
    }
    if (close(fd2) == -1) {
        syserr("close");
    }
}

