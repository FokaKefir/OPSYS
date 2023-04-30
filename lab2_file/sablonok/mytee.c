#include "myinclude.h"
#include <string.h>

#define SIZE 80

int main ( int argc, char * argv[])
{

    //TODO: mennyi az argc?
    if (argc != 2 && argc != 3) {
        printf ("használat: %s fájlnév [-a] FILE\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd;
    if (argc == 3) {
        if (strcmp(argv[1], "-a") != 0) {
            printf("Hibas opcio\n");
            exit(EXIT_FAILURE);
        }
        fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    } else if (argc == 2) {
        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    if (fd == -1) {
        syserr("open");
        exit(EXIT_FAILURE);
    }

    char buff[SIZE]; 
    int n;
    while ( (n=read (STDIN_FILENO, buff, SIZE)) > 0){ // a STDIN_FILENO erteke 0
        write (STDOUT_FILENO, buff, n);            // a STDOUT_FILENO erteke 1
        if ((write(fd, buff, n) != n)) {
            syserr("write");
            exit(EXIT_FAILURE);
        }
    }


    exit(EXIT_SUCCESS);

}
