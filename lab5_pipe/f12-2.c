
#include "myinclude.h"

int main()
{
    pid_t pid1, pid2;
    int pfd1[2], pfd2[2]; /* a csovezetek azonositoi */
    FILE * fp;
    double x;

    if (pipe(pfd1) < 0)
    {
        syserr("pipe");
    }

    if ((pid1 = fork()) < 0)
    {
        syserr("fork");
    }
    if (pid1 == 0)
    {// első fiú
        //bezárja amit nem használ
        if(close(pfd1[0])<0) syserr("cls");

        if ((fp=fopen("be1.txt","r"))==NULL) syserr("o");
        while (fscanf(fp,"%lf",&x)==1){
            if (write(pfd1[1],&x,sizeof(double))<0) syserr("wr");
        }
        fclose(fp);
        close(pfd1[1]);
        exit(0);
    }
    if (pipe(pfd2) < 0)
    {
        syserr("pipe");
    }

    if ((pid2 = fork()) < 0)
    {
        syserr("fork");
    }
    //fiu folyamat:
    if (pid2 == 0)
    {// második fiú
        //bezárja amit nem használ
        if (close(pfd1[0])<0) syserr("cls");
        if (close(pfd1[1])<0) syserr("cls");
        if (close(pfd2[0])<0) syserr("cls");

        if ((fp=fopen("be2.txt","r"))==NULL) syserr("o");
        while (fscanf(fp,"%lf",&x)==1){
            if (write(pfd2[1],&x,sizeof(double))<0) syserr("wr");
        }
        fclose(fp);
        close(pfd2[1]);
        exit(0);
    }
    //bezárja amit nem használ
    if (close(pfd1[1])<0) syserr("cls");
    if (close(pfd2[1])<0) syserr("cls");
    int n1,n2;
    double x1,x2;
    while ((n1=read(pfd1[0],&x1,sizeof(double)))>0 && (n2=read(pfd2[0],&x2,sizeof(double)))>0){
        printf("x1=%lf x2=%lf\n",x1,x2);
    }
    close(pfd1[0]); close(pfd1[0]);
    wait(NULL); wait(NULL);
    exit(0);
}