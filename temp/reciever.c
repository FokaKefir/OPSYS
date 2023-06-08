#include "myinclude.h"
#include "ipc_data.h"

int main(int argc, char const *argv[]) {
    
    int msgid = msgget(MSG_KEY, 0660 | IPC_CREAT);
    
    pid_t pid1 = fork();
    if (pid1 < 0) {
        syserr("fork");
    }
    if (pid1 == 0) {
        // fiu folyamat 1
        struct msgbuf message;
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("1: The message: %s\n", message.mtext);
        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        syserr("fork");
    }
    if (pid2 == 0) {
        struct msgbuf message;
        msgrcv(msgid, &message, sizeof(message), 2, 0);
        printf("2: The message: %s\n", message.mtext);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    wait(NULL);
    printf("Mind a ket uzenet berkezett\n");
    

    return 0;
}
