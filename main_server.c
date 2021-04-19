#include "server.h"

int main(void) {
    char line[MAX_MESSAGE_SIZE] = "";
    int sock;
    int error;

    if((error = openServer(PORT, &sock))!=SUCCESS){exit(error);}

    puts("enter something to close the server:");
    scanf("%s", line);

    closeServer(sock);

    return SUCCESS;
}