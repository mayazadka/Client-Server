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
#define PORTC 2021
#define IP "127.0.0.1"

void clean_up(int cond, int sock);
int openClient(char* ip, int port);
void closeClient(int sock);