/* 
   5. feladat sablon
   2 fiú szinkronizálása pipe-al

   TODO: a szinkronizálás meg van írva, írjuk meg a feladat
   alábbi követelményét:
  - apa létrehoz egy fájlt amíg a fiúk várnak, benne "Helló" szöveg
  - fiúk kiírják a terminálra a fájlt miután újraindulnak

  -tesztelés:
     $ gcc -Wall fel5.c -o fel5
     $ ./fel5

*/

#include "myinclude.h"

int main(int argc, char *argv[])
{
  int pfd[2]; //pipe azonosítók
  pid_t p1, p2;
  char b;

  setbuf(stdout, NULL); //pufferelés tiltása

  //cső
  if (pipe(pfd) < 0) syserr("pipe");

  //első fiú
  if ((p1 = fork()) < 0) syserr("p1:");

  //első fiú folyamat
  if (p1 == 0)
  {
    //olvasni fog a csőből
    if (close(pfd[1])<0) syserr("close");

    printf("fiú 1 vár\n");
    //próbál egy byte-ot olvasni, blokkolódik
    if (read(pfd[0], &b, 1) < 0) syserr("read2");

    //TODO fájl kiolvasása 
    FILE *fin1 = fopen("fel5.txt", "r");
    if (fin1 == NULL) {
      syserr("open");
    } 
    char txt1[10];
    fscanf(fin1, "%s", txt1);
    printf("1. %s\n", txt1);

    fclose(fin1);

    printf("fiú 1 fut\n");
    close(pfd[0]);
    exit(EXIT_SUCCESS);
  }

  //apa folytatja, 2. fiú létrehozása
  if ((p2 = fork()) < 0)
    syserr("p2:");

  //fiú 2
  if (p2 == 0)
  {
    //olvasni fog
    if (close(pfd[1])<0) syserr("close");
    
    printf("fiú 2 vár\n");
    //olvasni próbál, blokkolódik
    if (read(pfd[0], &b, 1) < 0) syserr("read1");
    
    //TODO fájl kiolvasása  
    FILE *fin2 = fopen("fel5.txt", "r");
    if (fin2 == NULL) {
      syserr("open");
    } 
    char txt2[10];
    fscanf(fin2, "%s", txt2);
    printf("2. %s\n", txt2);

    fclose(fin2);
    
    printf("fiú 2 fut\n");
    close(pfd[0]);
    exit(0);
  }

  //apa dolgozik

  //írni fog a csőbe
  if (close(pfd[0])<0) syserr("close");

  //apa
  printf("apa dolgozik\n");

  //TODO fájl létrehozása

  FILE *fout;
  fout = fopen("fel5.txt", "w");
  if (fout == NULL) {
    syserr("open");
  }

  char *hello = "Hello!";
  fprintf(fout, "%s", hello);

  fclose(fout);

  sleep(2);
  printf("apa befejezte\n");

  //ha lezárja a csövet, a két read visszatér fájl vége 0-val
  //és a két fiú folyamat újra indul
  //ha írtunk volna 2 byte-ot és utána zárjuk a csővet ugynaz történik
  close(pfd[1]);

  //megvárja a fiait
  wait(NULL);
  wait(NULL);
  exit(EXIT_SUCCESS);
}
