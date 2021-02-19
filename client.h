#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#ifdef __linux__            // UNIX/Linux
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#define PORTC 7777
#define IP "84.94.84.90"

void clean_up(int cond, int sock);
int openClient(char* ip, int port);
void closeClient(int sock);