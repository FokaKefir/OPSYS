/* 
 * FELADAT: kiírja, hányszor van meg egy karakter egy fájlban
 * futtatás:
 *     ./times a.txt a
 */

#include "myinclude.h"
#include <string.h>

#define BUFFSIZE 128 
int main(int argc, char * argv[])
{

    char buff[BUFFSIZE]; //ebbe olvasunk
    int fd; //fájl azonosító
    int count;

    //TODO: mennyi az argc?
    if (argc != 3) {
        fprintf(stderr,"használat: %s fájlnév karakter\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[2]) > 1) {
        printf("Csak egy karaktert kell magadni masodik parameterkent\n");
        exit(EXIT_FAILURE);
    }
    char c = argv[2][0];


    //TODO: fájl nyitás olvasásra és hiba ellenőrzés
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        syserr("open");
    }

    count=0;
    int n; 

    //TODO: a read paramétereit megadni
    while ((n = read(fd, buff, BUFFSIZE)) > 0) {
        if (n<0){
            syserr("read");
        }
        for (int i = 0; i < n; ++i)
            if (buff[i] == c)
                count++;
    }

    printf("%c előfordulási száma: %d\n", *argv[2], count);

    if (close(fd)<0){
        syserr("close");
    }

    exit(EXIT_SUCCESS);
}
