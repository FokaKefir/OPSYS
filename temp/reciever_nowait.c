#include "myinclude.h"
#include "ipc_data.h"

int main(int argc, char const *argv[]) {
    setbuf(stdout, NULL);

    int msgid = msgget(MSG_KEY, 0660 | IPC_CREAT);

    struct msgbuf message;
    int n = 0;
    while(n < 2) {
        msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT);
        if (errno != ENOMSG) {
            printf("%ld: The message: %s\n", message.mtype, message.mtext);
            n++;
        }

        msgrcv(msgid, &message, sizeof(message), 2, IPC_NOWAIT);
        if (errno != ENOMSG) {
            printf("%ld: The message: %s\n", message.mtype, message.mtext);
            n++;
        }
    }
    
    printf("Mind a ket uzenet berkezett\n");


    return 0;
}
