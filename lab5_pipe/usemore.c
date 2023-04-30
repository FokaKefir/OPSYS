/*
 * 
 * az apa folyamat egy nagy szoveges fajlt akar megjeleniteni
 * atirja csovezeteken a fianak, amelyben a more nevu
 * listazo programot futtatja
 *
 * teszteles:
 *    gcc -Wall usemore.c -o usemore
 *    ./usemore 1.txt
 * */

#include "myinclude.h"

/* a puffer hossza */
#define BUFF 80

int main(int argc, char *argv[]) {
    pid_t pid;
    int pfd[2];
    int fd, n, status;
    char buff[BUFF];

    if (argc != 2) {
        printf("használat: %s fájlnév\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    //csovezetek letrehozas
    if (pipe(pfd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {  //fiu
        syserr("fork");
    }

    //apa folyamat, ez ir a csovezetekbe, peldaul az 1.txt nevu
    //nagy meretu allomanyt masolja
    if (pid > 0) {
        // lezarja a cso olvashato veget
        if (close(pfd[0]) < 0) {
            syserr("close1");
        }
        //bemeneti fajl
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            syserr("open");
        }

        // atirja a fajlt a csovezetekbe
        while ((n = read(fd, buff, BUFF)) > 0) {
            if (write(pfd[1], buff, n) < 0) {
                syserr("pipe iras:");
            }
        }

        if (n < 0) {
            syserr("read1");
        }

        close(fd);

        //a cso lezarasa fontos!
        close(pfd[1]);
        // megvarja, hogy fia is befejezze
        if (wait(&status) < 0)
            syserr("wait");

        exit(EXIT_SUCCESS);
    }
    /* fiu folyamat kodja */
    else {
        close(pfd[1]);  // ez a folyamat olvasni fog, az irhato veget lezarja

        // hozzarendelem a pipe olvashato veget az stdin-hez
        if (dup2(pfd[0], STDIN_FILENO) < 0) {
            syserr("dup2");
        }

        close(pfd[0]); /* tobbet nincs ra szukseg */

        // elinditom a more programot, ez a standard bemenetrol olvassa
        // a szoveget
        execlp("more", "more", (char *)NULL);

        /* ide mar nem erhetek el */
        /* a more ki fog lepni, ha lezarjuk a pipe masik veget */
        syserr("execlp");
        exit(EXIT_SUCCESS);
    }
}
