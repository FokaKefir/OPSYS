#include "syncpipe.h"

int fd1[2];
int fd2[2];


void init_wait() {
    if (pipe(fd1) < 0) {
        syserr("pipe1");
    }
    if (pipe(fd2) < 0) {
        syserr("pipe1");
    }
}

void wait_child() {
    char buff[1];
    if (read(fd2[0], buff, 1) == 1) {
        if (buff[0] == 'p') {
            return;
        } else {
            syserr("wait_child");
        }
    } 
    syserr("wait_child");
}

void tell_parent(){
    char buff[1] = "p";
    if (write(fd2[1], buff, 1) != 1) {
            syserr("tell_parent");
    }
}

void wait_parent() {
    char buff[1];
    if (read(fd1[0], buff, 1) == 1) {
        if (buff[0] == 'p') {
            return;
        } else {
            syserr("wait_child");
        }
    } 
    syserr("wait_child");
}

void tell_child() {
    char buff[1] = "p";
    if (write(fd1[1], buff, 1) != 1) {
            syserr("tell_parent");
    }
}
