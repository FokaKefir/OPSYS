/*
 * 3. labor 6. feladat sablon
 *
 * egy paramétere van, a fájl neve
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[])
{

    int fd;              //fájl azonosító
    pid_t pid;           //folyamatazonosító
    char buff[] = "abc"; //ezt írjuk a fájlba

    //TODO: egy paraméter ellenőrzése, kilépés ha nincs
    if (argc != 2) {
        syserr("argc");
    }

    //TODO: fájl megnyitása írásra, csonkolás és új fájl
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        syserr("open");
    }

    //TODO:  fiú indítása a fork() függvénnyel
    pid = fork();
    if (pid < 0) {
        syserr("fork error");
    }

    //fiú kódja
    if (pid==0)
    {
        //TODO: a fiú beírja a 3 karaktert a fájlba
        //a buff címről
        if (write(fd, buff, 3) != 3) {
            syserr("write");
        }

        //zárja a fájlt és kilép
        if (close(fd)<0){
            syserr("close");
        }
        exit(EXIT_SUCCESS); //fiú kilép
    }
    //apa kódja 
    //
    //TODO: apa megvárja  a fiát
    //
    wait(NULL);

    off_t L;
    //TODO: lekérdezzük az L változóba, hol van az offset a 
    //fájlban: a SEEK_CUR-hoz képest 0 pozícióba visszük
    //tk. az offset nem fog elmozdulni
    L = lseek(fd, 0, SEEK_CUR);

    //kiírjuk, hol van az offset
    printf ("poz: %ld\n",L);

    //TODO: az apa is beír 3 karaktert
    if (write(fd, buff, 3) != 3) {
        syserr("write");
    }

    close(fd);
    exit(EXIT_SUCCESS);
}      
