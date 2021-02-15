#include "server.h"

int main() {
    char line[81] = "";
    int sock;
    int opened = 0;

    while(strcmp(line, "quit")!=0)
    {
        puts("enter command (open/close/quit):");
        printf("> ");
        scanf("%s", line);

        if(strcmp(line, "open") == 0)
        {
            if(!opened)
            {
                sock = openServer(PORTS);
                opened = 1;
            }
            else
            {
                puts("server allready open");
            }
        }
        else if(strcmp(line, "close") == 0)
        {
            if(opened)
            {
                closeServer(sock);
                opened = 0;
            }
            else
            {
                puts("server is not open");
            }
        }
        else if(strcmp(line, "quit")== 0)
        {
            if(opened)
            {
                closeServer(sock);
            }
            exit(0);
        }

        puts("");
    }
}