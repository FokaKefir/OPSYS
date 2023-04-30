#include "myinclude.h"
int main() {
    int fd;
    if ((fd = open("kecske.txt", O_RDONLY)) < 0) {
        syserr("open");
    }
    close(fd);
    return 0;
}