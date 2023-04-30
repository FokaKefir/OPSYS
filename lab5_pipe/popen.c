/* 
 * pelda a popen() hasznalatara
 * 
 * a popen() fuggveny egy uj folyamatot indit, elotte azonban 
 * egy csovezeteket hoz letre. Az uj folyamatban (fiu) meghivja a shellt
 * a parametereben megadott paranccsal, amely a pipe-rol
 * olvas vagy ir. 
 *
 * Azt, hogy olvas vagy ir szinten a popen() parametereiben kell megadni.
 *
 * pl. 
 *      FILE * fp = popen ("grep 'alma'", "w");
 *
 *      Elindul a "grep alma" shell parancs, es a pipe amit a fuggveny
 *      visszaad irhato lesz 2. szintu fuggvenyekkel.
 *      Az iras a parancs standard bemenetere tortenik.
 *      Hiba eseten NULL pointert ad vissza.
 *
 *      Az fp fajl mutatot a pclose(fp) hivassal 
 *      kell lezarni, a pclose() lezarja a pipe apa feloli oldalat, 
 *      es elvegzi az inditott fiura vonatkozo  wait parancsot is
 *  
 * forditas:
 *      gcc -Wall popen.c -o popen
 * teszteles:
 *      ./popen teszt.txt
 *      
 */

#include "myinclude.h"
#define MAX 80  //puffer hossza

int main(int argc, char *argv[]) {
    FILE *fp, *fp1;  // fp pipe az elinditott grep program fele
                     // fp1 egy szoveges fajl, ebbol olvasom az adatokat
    char buff[MAX];  //max 80 karakter egy sor a szovegfile-ban

    int status;  //a fiu kimeneti allapota

    if (argc != 2) {
        printf("használat: %s szoveg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fp = popen("grep 'alma'", "w"); /* fiu inditas es pipe nyitas 
                                       elindul a "grep 'alma'" parancs 
                                       egy fiu folyamatban, olvassa a csovezeteket
                                       w: irhato lesz a pipe az indito folyamatban 
                                     */
    //sikertelen popen
    if (fp == NULL) {
        syserr("popen");
    }

    // ebbol olvasom a teszt adatokat, soronkent:
    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        //ha az apa hibaval kilep:
        status = pclose(fp);  //a pclose lezarja a pipe irhato felet
                              //ennek kovetkezteben a fiu kilep
                              //a pclose meghivja  a wait()-et is, így megvarja a fiut
        syserr("fopen");
    }

    while (fgets(buff, MAX, fp1) != NULL) {  //amig van meg sor, olvasom
        if (fprintf(fp, "%s", buff) < 0) {   // itt irom a pipe-ba
            syserr("fprintf");
        }
    }

    if (ferror(fp1)!=0){
        printf("olvasási hiba a bemeneten\n");
    }

    fclose(fp1);

    // a pclose megvarja a popen-nel nyitott folyamatot
    // tartalmazza a wait fuggvenyt is
    // a status a elinditott parancs exit által visszaadott értéke
    // de ha a wait hibát ad, akkor -1
    if ((pclose(fp)) < 0) {
        syserr("pclose");
    }

    exit(0);
}
