/* 
 * Sablon: Labor 3, feladat 7.
 *
 * futtatás méréshez:
 *
 * ./kiazelso 100000
 * ./kiazelso 20000
 *
 * awk 'BEGIN{FS=""} {if (NF!=1) print "hibás sor", NR }' ki.txt
 * 
 * awk 'BEGIN{ca=0; cf=0}{a=$0; getline; f=$0; if (a=="1") {ca++;} else{ cf++;}; } END {print ca,cf,cf/(ca+cf)}' ki.txt
 * 
 */

#include "myinclude.h"

int main(int argc, char *argv[])
{
    int fd;
    pid_t pid; 
    long long ciklus,i;

    //ezt a két változót írjuk a fájlba
    char one[] = "1\n";
    char zero[] = "0\n";

    setbuf  (stdout,NULL);

    //TODO: ha nincs parancssori paraméter, ciklus=1000,
    //ha van, akkor atoll() függvénnyel átalakítjuk
    //és beírjuk a ciklus-ba
    //ha 0-t kapunk hiba és kilépünk
    if (argc == 1) {
        ciklus = 1000;
    } else if (argc == 2) {
        ciklus = atoll(argv[1]);
        if (ciklus <= 0) {
            printf("hibas parameter\n");
            exit(EXIT_FAILURE);
        }
    } else {
        syserr("arg");
    }


    //TODO: megnyitjuk ki.txt fájlt írásra, csonkolás, új fájl
    fd = open("ki.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    //TODO: ellenőrzés, megnyílt-e a fájl
    if (fd < 0) {
        syserr("open");
    }

    //elvégezzük többször a kiírásokat
    for (i=0; i<ciklus;i++)
    {

        //TODO: új folyamat, pid szám a pid változóba
        //ellenőrzés, hogy sikerült
        pid = fork();
        if (pid < 0) {
            syserr("fork error");
        }

        //fiú:
        if (pid == 0){

            //TODO: beírja a zero változó tartalmát a fájlba, 2 karaktert
            if (write(fd, zero, 2) != 2) {
                syserr("write zero");
            }

            exit(EXIT_SUCCESS);
        }else
        { //apa:
            
            //TODO: beírja a one változó tartalmát a fájlba, 2 karaktert
            if (write(fd, one, 2) != 2) {
                syserr("write one");
            }

            //TODO: apa megvárja a fiát
            wait(NULL);
        }
    }

    // bezarja a file-t
    if (close(fd)<0){
            syserr("close");
        }

    exit(EXIT_SUCCESS);
}      

