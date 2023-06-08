#define COMM_EXIT 0
#define COMM_CONNECT 1
#define COMM_DISCONNECT 2
#define COMM_LIST_CLIENTS 3
#define COMM_LIST_COMMANDS 4
#define COMM_SEND 5
#define COMM_SEND_ALL 6
#define COMM_UNKNOWN 7
#define COMM_ERROR 8

#define STR_COMM_EXIT "exit"
#define STR_COMM_CONNECT "connect"
#define STR_COMM_DISCONNECT "disconnect"
#define STR_COMM_LIST_CLIENTS "clients"
#define STR_COMM_LIST_COMMANDS "commands"
#define STR_COMM_SEND "send"
#define STR_COMM_SEND_ALL "sendall"

#define MAX_COMMAND_LENGTH 1024
#define MAX_USERNAME_LENGHT 20
#define MAX_TEXT_LENGTH 800

struct Command {
    int ctype;
    char username[MAX_USERNAME_LENGHT + 1];
    char text[MAX_TEXT_LENGTH];
};