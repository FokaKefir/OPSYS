#include "../myinclude.h"
#include "../parancssor.h"

#define MAX_ARGS 128

//a parancssort feldolgozo fuggveny
//bemenet: parancssor sztring, pl:
//   ls -l /home
//kimenet: hibakod, ha nem hajtodik végre a parancs   
int parancssor ( char * parancs)
{
    char * argv[MAX_ARGS] ; //ebben lesz az argv
    char * s;
    int argc = 0; 
    char ** ptr = argv;   //mutató az argv-re

    s = strtok(parancs, " "); //elso sztring

    while ( s != NULL ){

        *ptr++ = s;            //mutato bekerul az argv-be
                               //s sztring marad a helyen
        s = strtok(NULL, " "); //kovetkezo sztringek
        
        //ellenorzes, hogy ne lepjem tul a MAX_ARGS-ot
        if (++argc >= MAX_ARGS){
            fprintf(stderr, "túl sok argumentum\n");
            return 1;      
        }
    }
    *ptr = NULL ; //argv tömb lezárása
    // itt megvan az argv 
    execvp (argv[0], argv);
    // ide akkor jutunk ha nem sikerul 
    return 1;
} 

int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        syserr("fork");
    }
    if (pid == 0) {
        //fiu folyamat

        int fd = open("ki1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
            syserr("open");

        if (dup2(fd, STDOUT_FILENO) < 0)
            syserr("dup");
        
        execlp("egrep", "egrep", "[aeiou]{3}", "/usr/share/dict/words", (char *) NULL);
        
        syserr("exec");
    } else {
        // apa folyamat
        
        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {

            struct stat st;
            if (stat("ki1.txt", &st) == 0 && st.st_size > 0) {
                int fd = open("ki1.txt", O_RDONLY);
                if (fd < 0)
                    syserr("open");

                if (dup2(fd, STDIN_FILENO) < 0)
                    syserr("dup");

                execlp("shuf", "shuf", (char *) NULL);
                syserr("exec");
            } else {
                syserr("file size");
            }
        } else {
            syserr("error son");
        }

    }


    return 0;
}

