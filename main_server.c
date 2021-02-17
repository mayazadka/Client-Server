#include "server.h"

int main() {
    char line[81] = "";
    int sock;

    sock = openServer(PORTS);
    puts("enter something to close the server:");
    scanf("%s", line);
    closeServer(sock);
    exit(0);
}