#include "myinclude.h"
#include "ipc_data.h"

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        syserr("Missing arg");
    } 
    int n = atoi(argv[1]);

    int msgid = msgget(MSG_KEY, 0660 | IPC_CREAT);

    struct msgbuf message;
    message.mtype = n;

    char text[100];
    int ind = 0;
    while (ind < 5) {
        fgets(text, 100, stdin);
        strcpy(message.mtext, text);
        
        msgsnd(msgid, &message, sizeof(message), 0);

        printf("Message sended: %s\n", message.mtext);
        ind++;
    }
    
    return 0;
}
