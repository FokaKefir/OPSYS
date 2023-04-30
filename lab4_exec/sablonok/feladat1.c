/* 
 * sablon az 1. feladathoz
 *
 */

#include "../myinclude.h"
#include "../parancssor.h"

/*
    gcc -Wall ../parancssor.c feladat1.c -o feladat1
*/

int main (int argc, char * argv[])
{
    pid_t pid;   //folyamat azonosito
    int status;  //kilepesi allapot tarolasara

    pid = fork();  //uj folyamat
    if ( pid < 0){
        syserr("fork");
    }

    if ( pid == 0){

        //TODO: ide beirni az exec hivasokat
        //egyszerre egyet, kiprobalni es utana
        //magyarazatba tenni
     
       //execl("/usr/bin/sort","/usr/bin/sort", "-rn", "szam.txt", (char*) NULL); 
    
        char parancs[]="sort -nr szam.txt";
        parancssor(parancs);
        //ide nem ter vissza a fiu ha helyes

        syserr("execl");
    } else
    {  
        //apa folyamat megvarja a fiut 
        wait (&status);
        
        //TODO: az apa irja ki, hogy milyen exit koddal 
        //vagy jelzessel lepett ki a fiu
    }
    exit(EXIT_SUCCESS);
}
