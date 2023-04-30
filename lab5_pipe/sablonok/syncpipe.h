#ifndef MYINCLUDE
    #include "myinclude.h"
#endif

extern int fd1[2];
extern int fd2[2];

void init_wait();
void wait_child();
void tell_parent();
void wait_parent();
void tell_child();