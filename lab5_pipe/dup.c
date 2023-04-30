/* pelda a dup2() hasznalatara 
 * az apa folyamat atir nehany sort egy pipe-on keresztul
 * a pipe a masik vegen a sort varja es elrendezi a sorokat 
 *
 * az 
 *              int dup2(int oldfd, int newfd);
 *
 *              masolatot keszit az oldfd fajl azonositorol,
 *              es azt a newfd -be irja be
 *
 *              ha newfd elotte nyitva volt egy mas fajlra, lezarja 
 * 
 * teszteles: 
 *   gcc -Wall dup.c -o dup
 *   ./dup 
 */

#include "myinclude.h"

int main()
{
    FILE *fp;  // 2. szintu mutato
    int fd[2]; // pipe
    pid_t pid;

    setbuf(stdout, NULL);

    if (pipe(fd) < 0)
    { // pipe letrehozas
        syserr("pipe");
    }

    if ((pid = fork()) < 0)
    {
        syserr("fork");
    }

    //fiu folyamat:
    if (pid == 0)
    {

        close(fd[1]); // bezarom a pipe irhato felet

        // a pipe olvashato vege a standard input lesz
        // ha elvegzem a deszkriptor duplikalast
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            syserr("dup2");
        }

        execlp("sort", "sort", (char *)NULL); // elinditom a sort-ot
        //ez a stdin - en varja bemenetet mert
        //                 nincs fajl argumentuma
        syserr("execlp");
        //a fiu legfennebb eddig fut, ha exec hiba van
    }
    else //apa:
    {
        close(fd[0]); // itt nincs szukseg az olvashato felere

        fp = fdopen(fd[1], "w"); // atalakitom fd-t fp-ve
                                 // igy fp-t ugy hasznalhatom mint FILE *
        if (fp == NULL)
        {
            syserr("fdopen");
        }

        //ezek sorok a sort szamara
        fprintf(fp, "xyz\n");
        fprintf(fp, "abc\n");
        fprintf(fp, "klm\n");
        fprintf(fp, "aaa\n");

        // az fflush fontos lehet ha tobb folyamat ir/olvas a stdout-ra
        fflush(fp);
        fclose(fp);

        wait(NULL); // megvarom amig a fiu vegez
    }

    exit(0);
}
