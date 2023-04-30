/* egyszerű fork példa 
   ! hibás kód, nincs használva a wait() függvény
*/

#include "myinclude.h"

int main() {
    int i;

    pid_t fiu_pid ;
    pid_t apa_pid ;

    fiu_pid = fork();
    if (fiu_pid < 0) {
        syserr("fork");
    } 

    if ( fiu_pid == 0) {
        // erre a kód részre csak a fiú folyamat lép rá
        apa_pid = getppid();
        i=2;
        printf ("fiú a fork után i=%d, apa pid= %d, fiú  pid = %d\n",\
                i, apa_pid, (int) getpid());
        exit(0);	/* a fiú itt kilép */
    }
    else 
    {	
        // erre a kódrészre csak az apa lép rá 
        i=6;
        sleep(1);  /* 1 másodpercet vár: így valószínű, hogy a fiú hamarabb
                      fejeződik be */

        /* ez nem helyes, ilyenkor kell a wait függvényt használni 
         * semmi sem garantálja, hogy a fiú eddig lefut */
        printf("apa a fork után i=%d a fiú pid: %d\n",i, (int) fiu_pid);
        exit (0);
    }

}
