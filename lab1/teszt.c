/* C teszt file a gcc es gdb tanulmányozásához
 *
 * a fájl egyszerű és buta számításokat tartalmaz
 * olvassuk végig, értsük meg mit végez 
 *
 * ezen az állományon gyakoroljuk a C programokban végzett hibakeresést
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//egy buta függvény a stack keret tesztelésére
//egész szám négyzetét használja
int negyzet (int k)
{
    int n;
    n=k*k;
    return n;
} 

//visszatéríti a bemenet négyzetét
int fun1 (int i)
{
    int j;
    j=negyzet(i);
    return j;
}

int fun2 ( int ) ; //a függvény a fájl végén

int main (int argc, char * argv [] )
{
    int i,j;
    float x;
    int d;
    //ha kivesszük a megjegyzést a nem használt ures változó elől
    //a fordító csak akkor jelzi ezt, ha a -Wall opcióval fordítunk
    //char * ures = NULL; 
    char s[] = "kérek egy egész számot(szám>=5 && szam<=10):";
    int  tomb[100]; //számok tárolására

    j=8;
    i=j; //ha ezt kivesszük a -Wall opció jelzi, hogy j-t nem használjuk

    //i-be olvasunk egy egész számot
    do
    {
        printf ("%s",s);
        scanf ("%d",&i);
#ifdef DEBUG
        printf ("i=%d értéket olvastunk be\n",i);
#endif        
        j=i; 
    }
    while (i<5 || i>10); //amíg a szám kisebb mint 5

    //lefuttatjuk a fun2()-t k-szor
    int k;
    for (k=1; k<=i; k++)
    {
        d=fun2(k); //függvény hívás
        tomb[k-1]=d;
    }

    //kiírjuk
    for (k=1; k<=i; k++)
    {
        printf ("%d ",tomb[k-1]);
    }

    printf("\n");

    //vegyük ki ez a megjegyzést, és akkor meghívunk egy
    //függvényt a matematikai könyvtárból
    //x = pow(i,2);//a pow függvény a matematikai könyvtárban van: libm
    x = 3; //és ezt a sort tegyük megjegyzésbe

    i = fun1(i);

    printf("x * i = %.2f\n",x);

    exit (0);
}

//mit végez a függvény ?
int fun2 (int k)
{
    int r;
    if (k==1)
    {
        return k;
    }
    else
    {
        r = k * fun2(k-1);
        return r;
    }

}
