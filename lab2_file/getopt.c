/*
 * opció kezelés példa a getopt() függvénnyel
 * lásd man 3 getopt
 *
 * fordítás:   
 *  gcc -Wall getopt.c -o getopt
 * futtatás:
 *  ./getopt -a -b -c 3 alma
 */

#include "myinclude.h"
#include "getopt.h"

// a fejléc fájlokban definiálva  vannak:
extern int optopt; // - az utolsó beolvasott opció
extern int optind; // - az index az argv-ben ahol elmaradt
extern char * optarg; // - ha az opciónak van argumentuma is, ez arra mutat

int main(int argc, char *argv[])
{
    int af = 0; //opciók megjegyzéséhez használt jelző változók
    int bf = 0;

    int cf = 0;          //a c-nek kell paraméter is
    char *cparam = NULL; // paraméteres opció értéke

    int ret;

    //a getopt() paraméterei:
    //1 és 2: argc és argv úgy, ahogy a program kapja
    //a harmadik paraméter az opciók listája, ahol az opciónak
    //paramétere is van, ott kell még egy : az opció karakter után
    //tehát ez a program a következőket fogadja el:
    //   ./getopt -a -b -c cparam f1 f2 f3
    // ahol a c opciónak van egy paramétere is
    // az f1 f2 stb más paraméterek lehetnek, f1 az első ami
    // nem tartozik az opciókhoz
    //
    opterr = 0; //ha ez 0, akkor a getopt() nem ír hibaüzeneteket
                //amikor meghívjuk
    while ((ret = getopt(argc, argv, "abc:")) != -1)
    {
        switch (ret)
        {
        case 'a':
            af = 1; // a opciót talált
                    //bejelöljük az af kapcsolót
            break;
        case 'b':
            bf = 1;
            break;
        case 'c':
            cf = 1;          // a c opciónak kell paraméter is
            cparam = optarg; //az optarg egy külső változó
                             //ezt a getopt.h definiálja
            break;
        case '?': //ismeretlen opció vagy hiányzik egy opció paramétere, ekkor  ?-t kap

            //az ismeretlen opció karakter az optopt -ban van
            //
            //ha  a c opcio nem kap paramétert ismeretlennek számít
            if (optopt == 'c')
            {
                fprintf(stderr, "a c opció paramétere hiányzik\n");
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "ismeretlen opció -%c\n", optopt);
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "optind hiba\n");
            exit(EXIT_FAILURE);
        }
    }

    //beállított opciók

    if (af)
        printf("a opció beállítva\n");
    if (bf)
        printf("b opció beállítva\n");
    if (cf)
        printf("c opció beállítva, paramétere: %s\n", cparam);

    // int optind : az első paraméter indexe ami nem opció
    int i;
    for (i = optind; i < argc; i++)
    {
        printf("fennmaradó paraméter: %s\n", argv[i]);
    }
    exit(EXIT_SUCCESS);
}
