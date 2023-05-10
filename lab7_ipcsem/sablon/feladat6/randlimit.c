#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//egy pseudo random egesz szamot general, amely a [0,max) 
//intervallumban van

int randlimit(int max)
  {
    int L,d,x,r;

    // kivalasztunk egy hatart a random fuggveny eredmeny-nek:
    // ez a max-nak az a legnagyobb tobbszorose, amely 
    // kissebb mint RAND_MAX
    
    L = RAND_MAX - (RAND_MAX % max);   
                   //L a max legnagyobb tobbszorose, < RAND_MAX
    d = L / max;   

    // behivunk egy random szamot
    // addig hivjuk a fuggvenyt amig az eredmeny a [0,L) intervallumban
    // lesz. Ekkor ezt d - vel osztva, egy [0,max) intervallumbeli
    // egyenletesen eloszlo random szamot kapunk

    do {

        x = rand(); 
    
    } while (x >= L);

    r= (int) (x / d );
 
    // printf("rand call: %d L=%d rlimit: %d from pid: %d\n",x,L,r,getpid());

    return r;
  }

