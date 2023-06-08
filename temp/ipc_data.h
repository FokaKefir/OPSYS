#define MSG_KEY 16

#define MAX_TEXT 126

struct msgbuf {
    long mtype;
    char mtext[MAX_TEXT];
};