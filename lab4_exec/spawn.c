/* 
 * fork es exec egyutt = spawn
 */

#include "myinclude.h"

//ez a fuggveny elvegzi a fork() es execvp() hivast es
//ha sikerul nem ter vissza
int spawn ( char * prognev, char ** argumentumok)
{
    pid_t fiu_pid;
    fiu_pid = fork();        /*elinditja a fiu processzt */
    if (fiu_pid!=0) {
        /* az apa itt befejezi */
        return fiu_pid;
    }else{
        /* ez a fiu processz */    
        /* az execvp betolti az uj programot a fiu virtualis memoriajaba */

        execvp ( prognev, argumentumok );
        /* ide nem szabad eljutni, csak ha hiba tortent, es execvp
         * nem volt vegrehajthato 
         */
        syserr("execvp");
    }    

    return -1;
}

int main() 
{
    pid_t pid ;
    int status;

    /* az argumentumok sztring listaval vannak megadva , tehat az
     * argv tomb elo van keszitve: nem kell rajta semmit alakitani 
     */
    char * argumentumok[] = {"ls", "-l", "/home/", (char*) NULL};
    //char * argumentumok[] = {"sort", "-rn", "szam.txt", (char*) NULL};

    /* a spawn fuggveny elinditja a fiut es az apa ag visszater ugyanide
     * tehat itt az apa fut tovabb a spawn utan 
     */
    pid = spawn ("ls", argumentumok);
    //pid = spawn("sort", argumentumok);

    wait(&status); /* megvarja a fiut */ 

    if (pid>0){
        printf("\nVége, a fiu PID=%d volt.\n", pid );
    }else{
        printf("fiú indítása nem sikerült\n");
    }

    exit (EXIT_SUCCESS);
}

