#include "../myinclude.h"
#include "../parancssor.h"

/**
 * ./mysystem cat ki.txt | sort | uniq | head -1
 * ./mysystem "cat ki.txt | sort | uniq | head -1"
*/

int main(int argc, char const *argv[]){
    
    if (argc < 2) {
        syserr("arg");
    }

    
    char *myargv = (char *) malloc(1000 * sizeof(char));
    strcpy(myargv, "");
    for (int i = 1; i < argc; ++i) {
        strcat(myargv, argv[i]);
        strcat(myargv, " ");
    }
    //printf("%s\n", myargv);

    execlp(
        "bash",
        "bash",
        "-c",
        myargv,
        (char *) NULL
    );
    
    syserr("exec");

    return 0;
}
