/*
 *
 * Labor 3 , feladat 4.
 *
 */

#include "myinclude.h"

int main(int argc, char *argv[])
{

    pid_t pid;              //folyamat azonosító
    setbuf(stdout,NULL);    //pufferelés letiltva


    if((pid=fork())<0) {   //fiú indítása
        syserr("fork");
    }  

    //fiú folyamat kódja
    if (pid==0)
    {
        while(1){  //végtelen ciklus 
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }else
    {
        int status;  //állapot lekérdezéséhez

        // ez a rész a sig1.c feladathoz kell

        int cnt=0;   //számoljuk az 5 szekundumot

        //TODO: addig tart a while, amíg a waitpid() visszaadja azt
        //hogy a folyamat kilépett
        //a status a kilépési adatokat tartalmazza
        //a waitpid()-et WNOHANG opcióval hívjuk, hogy ne blokkoljon
        while (waitpid(pid, &status, WNOHANG) == 0) {
            printf("tick %d\n",cnt);
            sleep(1);
            //TODO:
            //ha  a számláló eléri az 5-öt, utána küldjünk TERM jelzést 
            //várjuk meg a fiút és lépjünk ki a while-ból break-el
            //a status a kilépési adatokat tartalmazza
            ++cnt;
            if (cnt == 5) {
                if (kill(pid, SIGTERM) == 0) {
                    printf("jelzes ok\n");
                }
            }
        } 
        //egy keveset várunk
        sleep(1);
        

       //apa megvárja a fiát 
       //wait(&status);//ez csak a sig.c-hez kell 
       
       //ha jelzéssel lépett ki kiírjuk a jelzés azonosítóját
       if (WIFSIGNALED(status)){
          printf("jelzéssel állt meg a fiú, azonosító: %d\n", WTERMSIG(status));
       } 

    }

    printf("apa kilép\n");
    exit(0);
}      
