#ifndef MY_INCL
    #include "myinclude.h"
#endif

//szerver kulcs
//ezt használjuk közös kulcsként
#define SVRKEY 0x123456789

//kliens üzenete
typedef struct {
    long mtype;
    long data; //hely adatok számára (pid)
} client_msg;

//kliens üzenet hossz
#define CSIZE sizeof(client_msg) - sizeof(long)

//szerver üzenet
typedef struct {
    long mtype;
    int seq;
} server_msg;

//szerver üzenet hossz
#define SSIZE sizeof(server_msg) - sizeof(long)
