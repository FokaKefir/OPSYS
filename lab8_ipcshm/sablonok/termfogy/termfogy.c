/* 
 * sablon a  "Termelő/fogyasztó feladat közös pufferrel"  feladathoz
*/

#include "myinclude.h"
#include "colors.h"    //kodok szines kiirashoz

#define  TERM    4     // a termelo fiu folyamatok szama  
#define  FOGY    4     // a fogyaszto fiu folyamatok szama  
                       //elso valtozatban a termelo es fogyaszto fiuk szama ugyanannyi

#define  ARRAY   3     // a float tomb hossza  

#define MS 20          //idozites varakozashoz, 20 ms 
// #define MS 100          //idozites varakozashoz 
// #define MS 0          //idozites varakozashoz 

//szimbolikus nevek a szemaforoknak
enum { mutex, empty, full }; // 0., 1., 2. szemafor indexek

//valtozok az osztott memoria kezelesehez
static int semid, shmid;  //szemafor, osztott mmeoria id
static char * mem;       //szegmens kezdo cime

static int * wr, *rd;    //iro es olvaso index
static float *arr;       //adat kezdocim

//a termelo fiuk feladata
//egy sorszamot és 
//egy bemeneti fajl nevet kap
void termelo ( int sor, char * fname )
{
    //termelo muvelet
    struct sembuf termel[3] = { 
        { mutex, -1, 0}, 
        { empty, -1, 0},
        { full, +1, 0} 
    };
    //kritikus szakaszbol kilepo muvelet
    struct sembuf jelez = { mutex, +1, 0}; 

    FILE * fp;
    if ( (fp=fopen(fname,"r")) == NULL ) 
        syserr("adatok");
    float szam; //ide olvas a fajlbol
                //nem olvasunk direkt a memoriaba, mert  a fajl muvelet idoigenyes,
                //es azt a kritikus szakaszon kivul vegezzuk
    int count = 0;
    while  (1)
    {
        //TODO: kivenni a break-et !!!
        break;

        //TODO: olvas egy szamot a fajlbol, ha fajl vege, a szam=NAN
        
        //TODO: termelo muvelet meghivasa !!! 3 szemaforon 

        //kritikus szakasz

        //TODO: beirja  a szamot  a tombbe az iro index helyre 

        //TODO: tovabbviszi az indexet, ha eleri a veget, akkor nullazza 

        //TODO: alszik MS*1000 mikroszekundumot

        //TODO: jelez muvelet

        count++;
        if ( isnan(szam) ) break;
    }
    printf (ANSI_COLOR_GREEN "a %d. termelő pid=%d termelő kilép, %d számot írt\n" ANSI_COLOR_RESET, sor, getpid(), count);

    //TODO: zarja  a fajlt es lecsatolj a memoriat
}

//a fogyaszto fiuk feladata
//egy sorszamot  kap 
void fogyaszto (int sor )
{
    //fogyaszto muvelet
    struct sembuf fogyaszt[3] = { 
        { mutex, -1, 0}, 
        { empty, +1, 0},
        { full, -1, 0} 
    };
    //kritikus szakaszbol kilepo muvelet
    struct sembuf jelez = { mutex, +1, 0}; 

    float szam; //ide olvassa ki a szamot a tombbol 
    int count = 0; //hany szamot olvas
    while  (1)
    {
        //TODO: kivenni a break-et !!!
        break;
        
        //TODO: fogyaszto muvelet meghivasa ! 3 szemaforon 

        //kritikus szakasz

        //TODO: kiolvassa a szamot, kiirja  a terminalra

        //TODO: tovabbviszi az olvaso indexet, ha eleri a veget, akkor nullazza 

        //TODO: alszik MS*1000 mikroszekundumot

        //TODO: jelez muvelet


        count++;
        if ( isnan(szam) ) break;
    }
   printf ( ANSI_COLOR_YELLOW "a %d. fogyasztó, pid=%d kilép, %d számot olvasott\n" ANSI_COLOR_RESET, sor, getpid(), count);

   //TODO: lecsatolja a memoriat
}

int main( int argc, char ** argv )
{
    pid_t pid,wpid;
    int i,j,status;

    short kezdo[3] = {1, ARRAY, 0}; //szemafor kezdo ertekek 

    if (argc != 2 || access(argv[1],R_OK)){
        printf ( ANSI_COLOR_RED "a bemeneti adatfájl hiányzik vagy nem olvasható\n" ANSI_COLOR_RESET "\n");
        exit(EXIT_FAILURE);
    }

    setbuf(stdout,NULL);

    //TODO: egy szemafortomb lekerese amelyben 3 szemafor lesz, IPC_PRIVATE kulccsal
    if ((semid=semget(IPC_PRIVATE, 3, 0660 | IPC_CREAT))<0) syserr("semget");

    //TODO: a szemaforok kezdeti ertekenek beallitasa 
    if (semctl(semid, 0 , SETALL, kezdo)<0) syserr("ctl");

    //TODO: kozos osztott memoria szegmens id megszerzése
    //mérete: 2*sizeof(int)+ARRAY*sizeof(float)
    if ((shmid=shmget(IPC_PRIVATE, 2*sizeof(int)+ARRAY*sizeof(float), 0660 | IPC_CREAT))<0) syserr("shmget");

    //TODO: csatolas
    // a mem mutatóra 
    mem = (char*) shmat (shmid, NULL, 0);
    if (mem==(void*)-1)  syserr("at");

    wr = (int*) mem;                  //iro index
    rd = (int*) (mem + sizeof(int));      //olvaso index
    *wr=0;   //kezdo ertekek - Linuxon 0-k vannak a memoria byte-okban felcsatolaskor
    *rd=0;   //igy ez felesleges

    arr= (float*) (mem + 2*sizeof(int));  //adattomb

    // TERM termelo folyamatot indit
    for (i=0; i < TERM ; i++) {
        pid=fork();
        if (pid<0)
            syserr("fork hiba");
        if ( pid == 0 ) {
            termelo ( i, argv[1] ); //a termelo megkapja a sorszamat es a fajl nevet
            exit(EXIT_SUCCESS); 
        }
    }
    // FOGY fogyaszto folyamatot indit
    for (i=0; i < FOGY ; i++) {
        pid=fork();
        if (pid<0) 
            syserr("fork hiba");

        if ( pid == 0 ) {
            fogyaszto (i);   
            exit(EXIT_SUCCESS);
        }
    }
    // ide csak az apa jut el, bevárja őket 
    for (j=0; j<(TERM+FOGY); j++){
        if ((wpid=wait(&status))<0)
            syserr("wait");
        printf ("a pid=%d azonosítójú fiú véget ért\n",wpid);
    }

    //TODO: szemafor tomb torlese  
    if (semctl(semid,0,IPC_RMID,0)<0) syserr("ctl2");

    //TODO: lecsatolja mem-et
    if (shmdt((void*) mem)<0) syserr("dt");

    //TODO: osztott memoria id torlese
    if (shmctl(shmid, IPC_RMID, 0)<0) syserr("ctl3");

    printf ( ANSI_COLOR_MAGENTA "pid=%d apa kilép\n" ANSI_COLOR_RESET "\n", getpid());

    exit(EXIT_SUCCESS);
}
