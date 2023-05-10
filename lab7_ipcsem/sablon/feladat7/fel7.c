#include "myinclude.h"
#include "randlimit.h"

int main()
{
    int semid;
    pid_t pid1, pid2;
    int N = 6; //ciklusok száma
    int r, i, fd;

    //megoldás 3 szemaforral
    short init[] = {0, 1, 1}; //kezdőértékek, fiúk lépnek először

    //apa szemafor műveletei
    struct sembuf apa_down = {0, -2, 0};
    struct sembuf apa_up[] = {{1, +1, 0}, {2, +1, 0}};
    //első fiú szemafor down művelete
    struct sembuf fiu1_down = {1, -1, 0};
    //második fiú szemafor down művelete
    struct sembuf fiu2_down = {2, -1, 0};
    //mindkét fiú up művelete
    struct sembuf fiuk_up = {0, +1, 0};

    setbuf(stdout,NULL);

    //TODO: itt megoldani
    
    //mindkét fiúban használjuk az 
    //  srand(getpid()) hívást, hogy más random számsort kapjunk
    //  vagy még jobb:
    //  srand(time(NULL) ^ (getpid()<<16));

    exit(EXIT_SUCCESS);
}