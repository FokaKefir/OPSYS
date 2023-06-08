#include "myinclude.h"
#include "chatapp.h"


struct Command cropCommand(char* strCommand) {
    struct Command command = {COMM_UNKNOWN, "", ""};
    char* start = strchr(strCommand, '/');
    char result[32];
    char username[MAX_USERNAME_LENGHT + 1];
    if (start != NULL) {
        start++;  
        char* end = strchr(start, ' ');
        if (end == NULL) {
            end = strchr(start, '\n');
        }
        if (end != NULL) {
            if (end - start > 31) {
                return command;
            } else {
                strncpy(result, start, end - start);
                result[end - start] = '\0';

                if (strcmp(result, STR_COMM_EXIT) == 0) {
                    command.ctype = COMM_EXIT;
                } else if (strcmp(result, STR_COMM_CONNECT) == 0) {
                    command.ctype = COMM_CONNECT;
                    char *start_username = end + 1;
                    char *end_username = strchr(start_username, ' ');
                    if (end_username == NULL) {
                        end_username = strchr(start_username, '\n');
                    }
                    if (end_username != NULL) {
                        if (end_username - start_username > MAX_USERNAME_LENGHT) {
                            command.ctype = COMM_ERROR;
                            strcpy(command.text, "too long username");
                        } else {
                            strncpy(username, start_username, end_username - start_username);
                            username[end_username - start_username] = '\0';
                            strcpy(command.username, username);
                        }
                    } else {
                        command.ctype = COMM_ERROR;
                        strcpy(command.text, "missing username");
                    }
                } else if (strcmp(result, STR_COMM_DISCONNECT) == 0) {
                    command.ctype = COMM_DISCONNECT;
                } else if (strcmp(result, STR_COMM_LIST_CLIENTS) == 0) {
                    command.ctype = COMM_LIST_CLIENTS;
                } else if (strcmp(result, STR_COMM_LIST_COMMANDS) == 0) {
                    command.ctype = COMM_LIST_COMMANDS;
                } else if (strcmp(result, STR_COMM_SEND) == 0) {
                    command.ctype = COMM_SEND;
                    char *start_username = end + 1;
                    char *end_username = strchr(start_username, ' ');
                    if (end_username != NULL) {
                        if (end_username - start_username > MAX_USERNAME_LENGHT) {
                            command.ctype = COMM_ERROR;
                            strcpy(command.text, "too long username");
                            return command;
                        } else {
                            strncpy(username, start_username, end_username - start_username);
                            username[end_username - start_username] = '\0';
                            strcpy(command.username, username);
                        }
                    }
                } else if (strcmp(result, STR_COMM_SEND_ALL) == 0) {
                    command.ctype = COMM_SEND_ALL;
                }

                return command;
            }
        }
    }
    return command;
}

void list_commands() {
    printf("\t/%s - to exit\n", STR_COMM_EXIT);
    printf("\t/%s <your_username> - to connect\n", STR_COMM_CONNECT);
    printf("\t/%s - to disconnect\n", STR_COMM_DISCONNECT);
    printf("\t/%s - to list clients\n", STR_COMM_LIST_CLIENTS);
    printf("\t/%s <username> <text> - to send message to a friend\n", STR_COMM_SEND);
    printf("\t/%s <text> - to send message to all the users connected to the server\n", STR_COMM_SEND_ALL);
}

void connect_to_server(char *username) {
    printf("Connecting to server with username: %s\n", username);
}

void disconnect_from_server() {
    printf("Disconnecting from server\n");
}

void list_clients() {
    printf("Listing clients\n");
}

void sender() {
    printf("To see all the command type: /commands\n");

    char strCommand[MAX_COMMAND_LENGTH];
    struct Command command;
    while (1) {
        // beolvassa a parancsot a terminalrol
        fgets(strCommand, MAX_COMMAND_LENGTH, stdin);

        // ellenorzni hogy /-el kezdodjon a parancs
        if (strCommand[0] != '/') {
            printf("\tWARNING: Command must start with the / character \n");
        } else {
            // kinyerjuk az adatokat a string-bol
            command = cropCommand(strCommand);
            printf("%d\n", command.ctype);

            // vizsgalja hogy ha exit akkor lepjen ki
            if (command.ctype == COMM_EXIT) {
                break;
            }
            switch (command.ctype){
                case COMM_CONNECT:
                    connect_to_server(command.username);
                    break;
                case COMM_DISCONNECT:
                    disconnect_from_server();
                    break;
                case COMM_LIST_CLIENTS:
                    list_clients();
                    break;
                case COMM_LIST_COMMANDS:
                    list_commands();
                    break;
                case COMM_SEND:
                    
                    break;
                case COMM_SEND_ALL:
                    
                    break;
                case COMM_ERROR:
                    printf("\tERROR: %s\n", command.text);
                    break;
                default:
                    printf("\tWARNING: Unknown command\n");
                    break;
            }
        }
    }
}


int main(int argc, char const *argv[]) {

    setbuf(stdout, NULL);
    
    pid_t pid_sender = fork();
    if (pid_sender < 0) {
        syserr("fork");
    }
    if (pid_sender == 0) {
        // sender process
        sender();
        exit(EXIT_SUCCESS);
    }

    pid_t pid_reciever = fork();
    if (pid_reciever < 0) {
        syserr("fork");
    }
    if (pid_reciever == 0) {
        // reciever process

        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    wait(NULL);


    return 0;
}
