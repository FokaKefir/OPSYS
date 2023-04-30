/* 
    2. feladat megoldása

    ! csak egy TODO van !
*/

#include "myinclude.h"

#define MAXBUFF 80

//be: pfd0 cső olvasható fele, fájl név
void fiu_feladat (int pfd0, char * nev)
{
    FILE * fpbe, * fpki ; char buff[80]; 

    //átalakítja a csővet 2. szintű fájl pointerré
    if ((fpbe=fdopen(pfd0,"r"))==NULL){
        syserr("fdopen2");
    }
    //fájl
    if ((fpki=fopen(nev,"w"))==NULL){
        syserr(nev);
    }
    //egy  sort olvas, egy sort ír
    while (fgets(buff, MAXBUFF,fpbe)!=NULL){
        if(fputs(buff,fpki)==EOF){
            syserr("fputs2");
        }
    } 

    //a NULL érték esetében nem tudjuk, hogy hiba vagy fájl vége
    //ezért meg kell nézni
    if(ferror(fpbe)){
        syserr("ferr");
    }
    fclose(fpbe);fclose(fpki);
}

//be: pfd1 cső írható fele, nev fájl név
void apa_feladat (int pfd1, char * nev)
{
    FILE * fpbe, *fpki ; char buff[80]; 

    //átalakítja a csővet 2. szintű fájl pointerré
    if ( (fpki=fdopen(pfd1,"w"))==NULL){
        syserr("fdopen1");
    }
    if ( (fpbe=fopen(nev,"r"))==NULL){
        syserr(nev);
    }

    //TODO: írjuk meg az apa másolási ciklusát

    if(ferror(fpbe)){
        syserr(nev);   
    }
    fclose(fpbe);fclose(fpki);
}

int main(int argc, char * argv[])
{
    pid_t pid;  
    int pfd[2];
    int status;

    if (argc!=3){
        printf("2 fájlnév!\n"); exit(1);
    }
    if (!strcmp(argv[1],argv[2])){
        printf("a két név ugyanaz!\n"); exit(1);
    }


    //cső
    if (pipe(pfd)<0) syserr("pipe");
    //fiú létrehozás
    if ( (pid=fork()) < 0) syserr("fork");

    if (pid==0){
        close(pfd[1]); // fiú olvasni fog
        fiu_feladat (pfd[0],argv[2]); 
        exit(0);
    }
    //apa
    close(pfd[0]); //apa írni fog
    apa_feladat (pfd[1],argv[1]); 
    wait(&status);

    //összehasonlítás a 2. fiúval
    if (WIFEXITED(status) && WEXITSTATUS(status)==0){
        printf("exit fiú ok\n");
    
        
        if ( (pid=fork()) < 0) syserr("fork");
        //cmp indítása
        if (pid==0){
            execlp("cmp","cmp",argv[1],argv[2],NULL);
           syserr("execlp");
        }     
        wait(&status);

        //ha cmp igazat ad vissza, a két fájl egyforma 
         if (WIFEXITED(status) && WEXITSTATUS(status)==0){
            printf("a két fájl egyforma\n"); 
        }else
        {
            printf("összehasonlítás nem sikerült\n"); 
        }
    }

    exit(0);

}
