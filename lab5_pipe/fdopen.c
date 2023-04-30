/*
 *
 * pelda az fdopen fuggveny hasznalatara
 * 2. - feladat megoldasa a 5_pipe_feladatok.pdf -bol
 * a program atnezese elott olvassuk el az 1. es 2. feladatokat
 * teszteles: 
 *     gcc -Wall fdopen.c -o fdopen
 *     ./fdopen
 */

#include "myinclude.h"

int main()
{
    pid_t pid;
    int pfd[2]; //csovezetek
    FILE *fp;   //fajl mutato, egyik folyamat irasra, a masik olvasasra hasznalja
    int status; //fiu kilepesi allapota

    char msg[] = "Helló!\n";

    int c;

    // a pipe fuggveny hozza letre a csovezetek deszkriptorait
    if (pipe(pfd) < 0)
    {
        syserr("pipe");
    }

    if ((pid = fork()) < 0)
    {
        syserr("fork");
    }

    //fiu folyamat:
    if (pid == 0)
    {
        //szabaly: a folyamat lezarja a csovezetek azon felet amelyet nem hasznal
        close(pfd[0]);

        //atalakitja kettes szintu fajl mutatova a csovezetek azonositojat
        fp = fdopen(pfd[1], "w");
        if (fp == NULL)
        {
            syserr("fdopen");
        }

        //beir egy sztringet 2. szintu fuggvennyel
        if (fprintf(fp, "%s", msg) < 0)
        {
            syserr("fprintf");
        }
        //az fflush a standard konyvtar pufferet uriti ki
        fflush(fp);
        fclose(fp);
        exit(0);
    }
    else
    {                  //apa folyamat:
        close(pfd[1]); //nem hasznalja
        // ez a folyamat olvas: amig a fiu nem ir a csobe,
        // addig a folyamat blokkolva van

        //file mutato lekeres a csore
        fp = fdopen(pfd[0], "r");
        if (fp == NULL)
        {
            syserr("fdopen");
        }
        //kettes szintu faljlkezelo fuggveny hasznalata
        while ((c = fgetc(fp)) != EOF)
        {
            putchar(c);
        }

        fclose(fp);
        //fiura var
        wait(&status);
        if (WIFSIGNALED(status))
        {
            printf("fiú hiba, jelzés: %d\n", WTERMSIG(status));
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            printf("fiú hiba, exit: %d\n", WEXITSTATUS(status));
        }
        exit(0);
    }
}
