/* 
 *  Labor 3, Feladat 1 sablon
 */

#include "myinclude.h"

//n darab c karaktert ír a name nevű fájlba
int makefile(char * name, char c, int n)
{
    int fd;
    //TODO: nyissuk meg írásra, új fájl, csonkolással
    if ((fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        syserr("open");
    }

    //TODO: for ciklus n-szer, minden ciklus egyszer 
    //kiírja a c karaktert a fájlba
    for (int i = 0; i < n; i++) {
        if ((write(fd, &c, 1)) != 1) {
            syserr("write");
        }
    }

    //fájl zárás
    close(fd);
    return 0;
}


int main(int argc, char *argv[])
{
    pid_t pid1, pid2; //folyamat azonosítók
    int status1, status2; //kilépési állapotok átvételéhez

    //első fiú indítása
    if ( (pid1=fork()) < 0) {
        syserr("f1");
    }
    if (pid1==0) {
        //TODO: első fiú kódja
        makefile("b.txt", 'b', 100000);

        exit(EXIT_SUCCESS);//a fiú itt mindenképp kilép, így
                           //nem futhat rá a következő kódokra  
    }

    //TODO: második fi
    //második fiú indítása
    if ((pid2 = fork()) < 0) {
        syserr("f2");
    }

    //TODO: második fiú kódja
    if (pid2 == 0) {
        makefile("c.txt", 'c', 100000);
        exit(EXIT_SUCCESS);
    }

    //TODO: apa kódja, itt ír a fájlba
    makefile("a.txt", 'a', 100000);
   
    //apa megvárja mindkét fiát 
    wait(&status1);
    if (WIFEXITED(status1)) {
        printf("Elso exit: %d\n", WEXITSTATUS(status1));
    }

    wait(&status2);
    if (WIFEXITED(status2)) {
        printf("Masodik exit: %d\n", WEXITSTATUS(status2));
    }

    exit(EXIT_SUCCESS);
}      
