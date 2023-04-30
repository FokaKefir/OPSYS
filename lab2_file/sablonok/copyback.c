#include "myinclude.h"

#define BUFFSIZE 1024

void swapBuff(char *buff, int bs) {
    int i = 0;
    int j = bs - 1;
    while (i <= j){
        int tmp = buff[i];
        buff[i] = buff[j];
        buff[j] = tmp;
        i++;
        j--;
    }
}

int main(int argc, char *argv[]) {
    char buff[BUFFSIZE];
    int size;

    if (argc != 3) {
        printf("használat: %s fájlnév sztring\n", argv[0]); 
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0644);

    if (fd<0){
        syserr("open");
    }

    if (fd2 < 0) {
        syserr("open");
    }

    size = lseek(fd, 0, SEEK_END);
    int pos = size - BUFFSIZE;
    int n;
    while (pos >= 0){
        
        lseek(fd, pos, SEEK_SET);
        
        if ((n = read(fd, buff, BUFFSIZE)) < 0) {
            syserr("read");
            exit(EXIT_FAILURE);
        }
        swapBuff(buff, BUFFSIZE);
        if (write(fd2, buff, n) != n) {
            syserr("write");
            exit(EXIT_FAILURE);
        }

        pos -= BUFFSIZE;
    }


    lseek(fd, 0, SEEK_SET);
    int m = size % BUFFSIZE;
    if ((n = read(fd, buff, m)) < 0) {
        syserr("read");
        exit(EXIT_FAILURE);
    }
    swapBuff(buff, m);
    if (write(fd2, buff, n) != n) {
        syserr("write");
        exit(EXIT_FAILURE);
    }
    
    if (close(fd) == -1 ){
            perror("write");
            exit(1);
    }
    if (close(fd2) == -1 ){
            perror("write");
            exit(1);
    }

    exit(EXIT_SUCCESS);
}