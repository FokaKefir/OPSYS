/* 
 * sablon az autos feladathoz
 * 
 * forditas - mindent:
 *        make all
 *    vagy csak az autok.c-t
 *        make autok   
 * futtatas:
 *        ./autok
 */

#include "myinclude.h"
#include "randlimit.h"

#define N 10           // a fiu folyamatok (autok) szama
// #define MSEC100 1000000 //1000ms tobbszoros az usleep() -nek
#define MSEC100 100000 //100ms tobbszoros az usleep() -nek

static int semid ;    //itt lesz a szemafor tomb id-je,
                      //minden fuggvenybol el lehet erni
                      //de csak ebből a C fájlból

//visszaad egy mutatot egy 3 elemu short tombre
//ebbe keri le, hogy hanyan varakoznak a 3 szemaforra
short *sem_waitq()
{
    short i;
    short *val;
    val = malloc(3 * sizeof(short));
    if (val == NULL)
    {
        syserr("val");
    }
    //TODO: lekeri a harom szemaforra varakozo folyamatok
    //szamat  val[0], val[1] es val[2]-be
    return val;
}

//a feladat amit a fiuk kell vegezzenek
//msec : hanyszor 100 ms a kapun valo athajtas
void fiu_feladata(int msec)
{
    short sem; //ebben tárolja a kiválasztott szemafort
    // printf("%d",sem);

    //TODO: szemafor műveletek megadása
    //struct sembuf down = ...
    //struct sembuf up = ...

    //TODO: kiválassza, melyik szemaforon várakozik
    //ez az lesz amelyiken a legkevesebben várakoznak

    //TODO: megváltoztatja a down es up művelet szemafor számát
    // down.sem_num = sem;
    // up.sem_num = sem;

    //TODO: beáll a sorba, lehúzza 1-el a kiválasztott szemafort

    //átmegy a kapun
    usleep(msec * MSEC100);
    
    // printf("pid %d áthajtott a kapun, áthaladási idő: %d x 100 ms\n", getpid(), msec);

    //TODO: kilép a kapun, vissza állítja ugyanazt a szemafort
}

//kiiro fiu feladata
void kiiro_fiu()
{
    //TODO lekerni m kezdo milliszekundumos idobelyeget

    //TODO: megirni a kiiro ciklust
    //ezt printf-et lehet hasznalni:
    //printf("idő: %6d ms kapu1: %2d   kapu2 %2d   kapu3 %2d\n", t, k1, k2, k3);
    //a t a kezdo es a ciklusbeli idobelyeg kozti kulonbseg 

}

int main(int argc, char **argv)
{
    int status;
    pid_t pid, wpid;
    int exited = 0; //ebben szamolja, hany fiut vart mar meg wait-el

    setbuf(stdout, NULL);

    //TODO: egy szemafortomb lekerese amelyben 3 szemafor lesz
    //semid

    //TODO: a szemaforok kezdeti ertekenek beallitasa

    int pidk; //a kiiro fiu pid-je
    //TODO: kiiro fiu inditasa egy fork(), es utana a fiuban
    //a kiiro_fiu () meghivasa
    //a vegen exit(EXIT_SUCCESS);

    //auto fiuk inditasa
    int i;
    for (i = 0; i < N; i++)
    {
        pid = fork();
        if (pid < 0)
            syserr("fork hiba");

        if (pid == 0)
        {
            pid_t pid_fiu;
            pid_fiu = getpid();
            setbuf(stdout, NULL);
            //a random generatort inicializaljuk
            //minden folyamatban mas maggal
            srand(pid_fiu);

            //   a fiuk 1 es 5 szekundum kozt varakoznak a kritikus szakaszban
            int r;
            do
            {
                r = randlimit(6);
                // printf("random szám: %d\n",r);
            } while (!r);

            printf("%d.fiú indul: pid: %d várakozás: %d x100 milliszekundum\n", i+1, pid_fiu, r);
            fiu_feladata(r);
            exit(EXIT_SUCCESS);
        }
        else
        {
            usleep(MSEC100); //var minden fiu inditasa utan, 
            // az autok egymas utan erkeznek kapukhoz, de nem egyszerre
            // megnezi, lepett-e ki fia
            if ((wpid = waitpid(-1, &status, WNOHANG))<0)
                syserr("wpid");
            if (wpid){
                printf("    a pid=%d azonosítójú fiú véget ért (wp)\n", wpid);
                exited++;
            }

            // az apa itt semmi mast semmit sem csinal, tovabb fut uj fiut inditani
        }
    }

    // ide csak az apa jut el ha minden fiut elinditott
    //megvarja az osszes fiat, amit nem vart meg a waitpid()-el
    while ( exited++ < N )
    {
        wpid = wait(&status);
        printf("    a pid=%d azonosítójú fiú véget ért\n", wpid);
    }

    //TODO: kiíró folyamat leállítása TERM jelzéssel
    // if (kill(pidk, SIGTERM) < 0)
        // syserr("kill");

    //TODO: szemafor tomb torlese

    // printf("apa kilép\n");

    exit(EXIT_SUCCESS);
}
