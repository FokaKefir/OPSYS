/* fork feladatok
 * a 4. feladat megoldasa , 5_pipe_feladatok.pdf
 * forditas:
 *       gcc -Wall start2.c -o start2
 * inditas:
 *  
 *      ./start2  prog1 prog2 
 *
 *   stdin -> PROG1 -> pipe -> PROG2 
 *
 *      peldaul:
 *      ./start2  cat sort
 *      vagy:
 *      ./start2 tac cat
 *
 *  egy apa folyamat felugyeli a ket fiut
 *
 *  ebben a pelda inditasban: ./start2  cat sort
 *  az egyik fiuban  a cat fut, a masik fiuban pedig a sort
 *  amit a cat ir, azt a sort fogja olvasni
 *
 *  a cat a standard bementrol olvas, be kell irni sorokat
 *  a sort a standard kimenetre ir, rendezve kiirja a sorokat
 *
 *  mindket folyamatban duplikalni kell a fajl azonositokat
 */

#include "myinclude.h"

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;

    if (argc != 3) {
        printf("használat: %s prog1 prog2\n", argv[0]);
        exit(1);
    }

    if (pipe(fd) < 0) {
        syserr("pipe");
    }

    if ((pid = fork()) < 0) {
        syserr("fork hiba");
    }

    //elso fiu:
    if (pid == 0) {
        //ez argv[1] -et kell inditsa, a csobe ir, olvashato veget lezarja
        close(fd[0]);
        //hozzarendelem a pipe irhato felet az stdout-hoz
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
            syserr("dup2 hiba");
        }
        //elinditom az elso argumentumnak megfelelo programot
        execlp(argv[1], argv[1], NULL);
        syserr("exec fiu1-ben nem indul");
        //fiu ide nem er el
    }

    if ((pid = fork()) < 0) {
        syserr("fork hiba");
    }
    // masodik fiu:
    if (pid == 0) {
        //cso irhato fele lezarva
        close(fd[1]);
        //a pipe olvashato fele a stdin leiro szamat kapja
        //innen fog olvasni a sort
        if (dup2(fd[0], STDIN_FILENO) < 0)
            syserr("dup2 hiba");

        execlp(argv[2], argv[2], NULL);
        syserr("exec a fiu2-ben nem indul");
    }
    //  !!! figyelem:
    //az apa lezarja a csovezetek mindket veget, mert ezek az o folyamataban
    //nyitva vannak - bar nem fogja hasznalni oket
    //ez a muvelet fontos - ha nyitva hagyja, a fiai azt hiszik, hogy
    //meg valaki hasznalja a csovet, es
    //arra varnak, hogy irjon bele
    //igy blokkolodni fognak, es ha az apa elkezdi a wait-et a zaras
    //nelkul akkor o is blokkolodik

    close(fd[0]);
    close(fd[1]);

    wait(NULL), wait(NULL);

    exit(0);
}
