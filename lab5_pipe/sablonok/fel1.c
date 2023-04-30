/*
 * 1. feladat sablonja 
 * apa átmásol egy bármilyen fájlt a fiának egy csövön,
 * akár szöveges, akár bináris a fájl
 *
 *  binárisan másolnak, nem nézik mi a tartalom
 */

#include "myinclude.h"

#define MAXBUFF 80

//a fiú feladata
//be: pfd0: cső olvasható vége, nev: kimeneti fájl
void fiu_feladat(int pfd0, char* nev) {
    int fd, n;  //fájl beolvasni
    char buff[MAXBUFF];
    int count = 0;

    //TODO: megnyitjuk írásra a fájlt, név a nev-ben
    //      O_WRONLY | O_TRUNC | O_CREAT
    fd = open(nev, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    //TODO: -olvasunk MAXBUFF byte-ot a csőből: pfd0
    //      -írunk n byte-ot a fájlba
    //      -számoljuk hány byte-ot olvastunk összesen a count-ban
    //      -minden hibát ellenőrzünk
    while ((n = read(pfd0, buff, MAXBUFF)) > 0) {
        if (write(fd, buff, n) != n) {
            syserr("write fd");
        }
        count += n;
    }

    //TODO: lezárjuk a csövet meg a fájlt is
    close(fd);
    close(pfd0);

    printf("másolás összesen: %d byte\n", count);
}

//az apa feladata
//be: pfd1: cső írható vége, nev: bemeneti fájl
void apa_feladat(int pfd1, char* nev) {
    int fd, n;
    char buff[MAXBUFF];

    //TODO: megnyitjuk olvasásra a fájlt, név a nev-ben
    //      O_RDONLY
    fd = open(nev, O_RDONLY);
    if (fd < 0)
        syserr("open");

    //TODO: olvasunk MAXBUFF byte-ot a fájlból
    //      írunk n byte-ot a csőbe
    //      minden hibát ellenőrzünk
    while ((n = read(fd, buff, MAXBUFF)) > 0) {
        if (write(pfd1, buff, n) != n) {
            syserr("write pipe");
        }
    }

    //TODO: lezárjuk a csövet és a fájlt
    close(fd);
    close(pfd1);
}

int main(int argc, char* argv[]) {
    pid_t pid;   //fiú pid
    int pfd[2];  // cső
    int status;

    //két különböző paraméter
    if (argc != 3) {
        printf("Használat: %s fájl-be fájl-ki\n", argv[0]);
        exit(1);
    }
    if (!strcmp(argv[1], argv[2])) {
        printf("a két fájlnév név ugyanaz!\n");
        exit(1);
    }

    //megnyitjuk a csövet
    //pfd változóban lesz a két azonosító
    //hiba ellenőrzés!
    if (pipe(pfd) < 0) syserr("pipe");

    //elindítjuk a fiút, pid-ben a folyamat azonosító
    if ((pid = fork()) < 0) syserr("fork");
    //fiú:
    if (pid == 0) {
        //lezárjuk a cső írható végét
        if (close(pfd[1]) < 0) syserr("close");
        //elindítjuk a fiú feladatot
        fiu_feladat(pfd[0], argv[2]);  //feladat
        exit(EXIT_SUCCESS);
    }
    //apa feladatai:

    //lezárjuk a cső olvasható véget
    if (close(pfd[0]) < 0) syserr("cls");

    //elindítjuk az apa feladatot
    apa_feladat(pfd[1], argv[1]);  //feladat

    wait(&status);
    printf("Apa ok\n");

    // a feladat 2. részéhez ezt a megjegyzést kivenni

    //ha hiba nélkül lépett ki a fiú
    if (WIFEXITED(status) && WEXITSTATUS(status)==0){
        printf("exit fiú ok\n");
        
        //második fiút indítunk
        if ( (pid=fork()) < 0) {
            syserr("fork");
        }
        

        //a második fiúban elindítjuk a cmp-t
        if (pid==0){
            //TODO: execlp-vel indítjuk a cmp-t, argv[1] és argv[2] összehasonlításra
            execlp("cmp", "cmp", argv[1], argv[2], (char *) NULL);
            syserr("execlp");
        }     

        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status)==0){
            printf("a két fájl egyforma\n"); 
        }else
        {
            printf("összehasonlítás nem sikerült\n"); 
        }
    }
    
    exit(EXIT_SUCCESS);
}
