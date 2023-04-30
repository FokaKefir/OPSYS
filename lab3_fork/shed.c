/* 
  példa két folyamat egy processzoron való futtatására

  fordítás:
    gcc -Wall shed.c -o shed

  futtatás:
   taskset -c 0 ./shed 30 50

   A taskset parancs -c 0 opcióval a 0. processzoron fogja futtatni mindkét folyamatot 

 */

#include "myinclude.h"

#define TIMES 7800

//count darab what karaktert nyomtat, minden karakter után redundáns ciklust hív
//így nem usleep() vagy hasonló függvények miatt kerül ki az ütemezésből
static inline void print_char (const char what, const int count)
{
    char buff[1];
    *buff = what;

    for (int i=0; i<count; i++){
        if (write(STDOUT_FILENO, buff,1)<0) 
            syserr("write");
        //időtöltés processzoron
        for (int t=0; t < TIMES*1000; t++ );
    }
}
int main(int argc, char * argv[]) {
    setbuf (stdout,NULL);

    pid_t fiu_pid ; 

    if (argc!=3){
      printf("Használat: %s fiú_ciklus apa_ciklus\n", argv[0]);
      exit(1);
    }

    //az első paraméter a fiú ciklusszáma, a második az apáé
    const int cf = atoi(argv[1]);
    const int ca = atoi(argv[2]);

    if (cf<1 || ca<1){
      printf("A paraméterek pozitív egészek\n");
      exit(1);
    }


    fiu_pid = fork();           

    if ( fiu_pid < 0 ) {                  
        syserr("fork");
    } 

    if ( fiu_pid == 0) {
        // fiu kod resze 
        
        print_char('b',cf); 

        exit(0);
    } else {    
        
        print_char('a',ca); 
        
        wait(NULL);      

    }    
    printf ("\n");

    return 0;
}

