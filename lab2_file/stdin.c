/* 
 * az stdin masolasa az stdout-ra elso 
 * szintu read es write-al
 * karakterenkent
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define N 10

int main (int argc, char*argv[])
{
    char buff[N]; 
    int n;

    while ( (n=read (STDIN_FILENO, buff, N)) > 0){ // a STDIN_FILENO erteke 0
        write (STDOUT_FILENO, buff, n);            // a STDOUT_FILENO erteke 1
    }
    exit (0);
}

