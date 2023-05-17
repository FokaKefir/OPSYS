#include <stdio.h>
#include "colors.h"


int main() {
    printf ( ANSI_COLOR_RED "Ez piros" ANSI_COLOR_RESET "\n");
    printf ( ANSI_COLOR_GREEN "Ez %d zöld" ANSI_COLOR_RESET "\n",1);
    printf ( ANSI_COLOR_BLACK "Ez fekete" ANSI_COLOR_RESET "\n");
    printf ( ANSI_COLOR_WHITE "Ez fehér" ANSI_COLOR_RESET "\n");
    return 0;
}