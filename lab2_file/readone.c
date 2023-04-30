/* 
 * az stdin masolasa az stdout-ra elso 
 * szintu read es write-al
 * karakterenkent
 */

#include "myinclude.h"

int main (int argc, char*argv[])
{
    char buff; 
    int n;

    if ( (n = read (STDIN_FILENO, &buff, 1)) < 0){ 
        syserr("read");
    }
    exit (0);
}

