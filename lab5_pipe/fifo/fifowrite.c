
/*
 * példaprogram a fájlrendszerbeli névvel 
 * rendelkező csővezeték használatára
 *
 * ez a program ír a csőbe
 * fordítás:
      gcc -Wall fifowrite.c -o fifowrite
 * használat 
      ./fifowrite fifo1
 *   fifo1 a pipe neve a fájlrendszerben
 */

#include "myinclude.h"

#define N 256

int main(int argc, char **argv)
{
    char buff[N];
    int fd;

    if (argc != 2)
    {
        printf("Használat: %s fifo_név\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //van-e ilyen nevű fájl
    if (access(argv[1], F_OK))
    { //ha nem létezik
        //létrehozza a névvel rendelkező csövet
        //lásd man 3 mkfifo
        if (mkfifo(argv[1], 0644) < 0)
            syserr("mkfifo");
    }

    //megnyitja  a fifo-t írásra
    if ((fd = open(argv[1], O_WRONLY)) < 0)
    {
        syserr("open");
    }

    //az stdin-t masolja  a fifo-ba
    while (fgets(buff, N, stdin))
    {
        if (write(fd, buff, strlen(buff)) < 0)
            syserr("write");
    }

    close(fd);
    exit(0);
}