#include "myinclude.h"
#define SEEK_DATA 3
#define SEEK_HOLE 4

#define BUFFER_SIZE 4096
 
int main(int argc, char* argv[]) {
 
    if(argc != 3) {
        fprintf(stderr,"használat: %s fájlnév1  faljnev2\n", argv[0]); 
        exit(EXIT_FAILURE);
    }
 
    int fd1 = open(argv[1], O_RDONLY);
 
    if(fd1 == -1) {
        syserr("open");
    }
 
    int offset = 0;
    int hole_start, data_start;
 
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
 
    if(fd2 == -1) {
        syserr("open");
    }
 
    while(1) {
        char buffer[BUFFER_SIZE];
        int num_read = pread(fd1, buffer, BUFFER_SIZE, offset);
 
        if(num_read == -1) {
            perror("pread");
            exit(EXIT_FAILURE);
        }
 
        if(num_read == 0) {
            break;
        }
 
        int end_offset = offset + num_read;
        while(offset < end_offset) {
            hole_start = lseek(fd1, offset, SEEK_HOLE);
            if(hole_start == -1) {
                if(errno == ENXIO) {
                    hole_start = end_offset;
                    data_start = lseek(fd1, offset, SEEK_DATA);
                    break;
                }
            }
            if(hole_start >= end_offset) {
                break;
            }
            data_start = lseek(fd1, hole_start, SEEK_DATA);
            if(data_start >= end_offset) {
                break;
            }
            if(data_start > hole_start) {
                int num_copied = data_start - offset;
                if(pwrite(fd2, buffer, num_copied, offset) == -1) {
                    exit(EXIT_FAILURE);
                }
                offset = data_start;
                break;
            }
            offset = hole_start;
        }
 
        if(hole_start < end_offset) {
            int num_copied = hole_start - offset;
            if(lseek(fd2, num_copied, SEEK_CUR) == -1) {
                syserr("lseek");
            }
            offset = hole_start;
        }
    }
}