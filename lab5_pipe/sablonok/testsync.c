#include "myinclude.h"
#include "syncpipe.h"

#define FILENAME "sync.txt"

/**
 * gcc testsync.c syncpipe.c -o testsync
*/

void child() {

    // megnyitja a filet
    FILE *fout;
    fout = fopen(FILENAME, "w");
    if (fout == NULL) {
        syserr("open");
    }

    // olvassa a szamokat a terminalrol
    float a, b;
    printf("Write 2 numbers: ");
    scanf("%f%f", &a, &b);

    // kiirja a file-ba majd bezarja azt
    fprintf(fout, "%f %f", a, b);
    fclose(fout);

    // szol az apa folyamatnak hogy folytathatja
    tell_parent();

    // varja az apa folyamatot
    wait_parent();

    // megnyitja a file-t
    FILE *fin;
    fin = fopen(FILENAME, "r");
    if (fin == NULL) {
        syserr("open");
    }

    // kiolvassa az osszeget
    float c;
    fscanf(fin, "%f", &c);

    // kiirja standard kimenetre
    printf("Az osszeg: %f\n", c);

}

void father() {
    
    // varja a fuit
    wait_child();

    // megnyitja a file-t
    FILE *fin;
    fin = fopen(FILENAME, "r");
    if (fin == NULL) {
        syserr("open");
    }

    // kiolvassa a szamokat es osszeadja
    float a, b;
    fscanf(fin, "%f%f", &a, &b);
    float c = a + b;

    // bezarja a file-t majd megnyitja csonkolva
    fclose(fin);

    FILE *fout;
    fout = fopen(FILENAME, "w");
    if (fout == NULL) {
        syserr("open");
    }

    // beirja az osszeget majd zarja a file-t
    fprintf(fout, "%f", c);
    fclose(fout);

    // szol a fiu folyamatnak
    tell_child();
}

int main(int argc, char const *argv[]){
    
    init_wait();

    pid_t pid = fork();
    if (pid < 0) {
        syserr("fork");
    }

    if (pid == 0) {
        // fiu folyamat
        child();
        exit(EXIT_SUCCESS);
    } else {
        // apa folyamat
        father();
        wait(NULL);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    return 0;
}
