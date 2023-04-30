#include "myinclude.h"

#define MAXCHILD 10

int main(int argc, char const *argv[]){
    int n = 3;

    setbuf(stdout, NULL);

    if (argc == 2) {
        n = atoi(argv[1]);
    }

    if ( n < 1 || n > MAXCHILD){  //fiú szám több mint MAXCHILD
        fprintf(stderr, "n 1..%d közötti, de aki szórakozni akar megnövelheti a MAXCHILD paramétert\n",MAXCHILD); 
        exit(1);
    } 

    pid_t pid = 0, wpid;

    for (int i = 0; i < n; ++i) {
        if (i == 0 || pid != 0) {
            pid = fork();
            if (pid < 0) {
                syserr("fork error");
            }
        }
        if (pid == 0) {
            // fiu folyamat
            printf("%d. fiu folyamat elkezdodott az %d pidval\n", i, getpid());
            usleep(1000000);
            //printf("Fiu folyamat befejezodott az %d pidval\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < n; ++i) {
        wpid = wait(NULL);
        printf("%d azonositoju fiu veget ert\n", wpid);
    }

    exit(EXIT_SUCCESS);

    return 0;
}
