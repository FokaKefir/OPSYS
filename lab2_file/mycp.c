/* Operacios rendszer II Labor
 * egyszeru allomanymasolo program
 * forditas:
 *      gcc -Wall mycp.c -o mycp
 * inditas:
 *      ./mycp file1 file2
 *
 * A program nem ellenorzi, hogy a ket fajlnev megegyezik.
 * Miert gond ez? mi fog tortenni, ha ugyanazt a nevet adjuk meg
 * forras es cel fajlnak?

 * javitsuk ki: ha ugyanazt a nevet adjuk ki es bementnek, ne fusson le
 * a program
*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#define BUFFSIZE 1024  // a masolasi puffer merete

int main(int argc, char* argv[]) {
    int fd1, fd2, n;
    char buf[BUFFSIZE];  //,asolashoz hasznalt puffer

    // van-e harom argumentum
    if (argc != 3) {
        fprintf(stderr, "használat: %s f1 f2\n", argv[0]);
        exit(1);
    }

    // bemeneti fajl nyitas
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror("open1");
        exit(1);
    }
    // kimeneti fajl letrehozas, ha letezik torli a regit
    if ((fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1) {
        perror("open2");
        exit(1);
    }

    // masolas amig a read nem 0-t ad vissza
    while ((n = read(fd1, buf, BUFFSIZE)) > 0) {
        if ((write(fd2, buf, n) != n)) {
            perror("write");
            exit(1);
        }
    }
    
    if (n < 0) { //ha a reda hibazik
        perror("read");
        exit(1);
    }
    // az allomayokat zarni kell kilepes elott
    if (close(fd1) == -1 ){
            perror("write");
            exit(1);
    }
    if (close(fd2) == -1 ){
            perror("write");
            exit(1);
    }

    exit(0);
}
