/* 
 * 
 * több szintű folyamat indítás, minden fiú folyamat új fiút indít vízesésszerűen
 * a mélység száma implicit 3, vagy megadható a parancssoron
 *
 *   gcc -Wall cascade1.c -o cascade1
 *   ./cascade1 10
 */

#include "myinclude.h"

#define MAXCHILD 20  //maximális mélység
//ennél többet ne, mert blokkolni fogják a rendszert

int main(int argc, char * argv[])
{

    pid_t pid, wpid;
    int n = 3; /* 3 ciklus implicit, ha nincs paraméter */

    setbuf(stdout,NULL);  // stdout: nincs pufferelés 

    if (argc>1){
        n=atoi(argv[1]); //milyen mélységben indítsanak folyamatokat
    }

    if ( n < 1 || n > MAXCHILD){  //fiú szám több mint MAXCHILD
        fprintf(stderr, "n 1..%d közötti, de aki szórakozni akar megnövelheti a MAXCHILD paramétert\n",MAXCHILD); 
        exit(1);
    } 

    //fiú folyamatok indítása
    for (int i=0; i<=n; i++) {
        pid=fork();
        if (pid<0){
            syserr("fork hiba");
        }
        if ( pid == 0 ) {
            // a fiú folyamat vissza megy a for-ra és ismét indít egyet 
            //usleep(100000); //ha lassabban akarjuk futtatni  
            printf ("%d feladata elvégezve\n",getpid());
        }else
        {
            /* az apa bevárja fiát, nem megy vissza a for-ra
             * az exit() miatt 
             * */
            wpid=wait(NULL);
            printf ("%d megvárta %d-t és kilépett\n",getpid(),wpid);
            exit(0);
        }
    }

    /* az utolsó fiú jut ide */
    printf ("%d utolsó fiú vége\n", getpid());
    exit(EXIT_SUCCESS);

}      

