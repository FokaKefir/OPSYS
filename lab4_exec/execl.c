/* 
 * pelda az execl() hasznalatara
 *
 */

#include "myinclude.h"

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara

    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){
        // fiu folyamat, uj programot inditunk 
        //  az ls -l parancsot inditjuk
        // a /bin/ls azert szerepel ketszer, mert az elso sztring
        // az inditando program helye, a masodik a parancssor elso argumentuma
        //execl ("/bin/ls", "/bin/ls", "-l", (char*) 0);
        execlp("ls", "ls", "-l", (char *) 0);
        //execlp("sort", "sort", "-rn", "szam.txt", (char *) 0);
        // ide mar nem ter vissza 
        // ha megis, akkor hiba 
        syserr("execl");
    } else
    {  
        //apa folyamat megvarja a fiut 
        wait (&status);
    }
    exit(EXIT_SUCCESS);
}
