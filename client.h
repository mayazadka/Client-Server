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
#include <pthread.h>
#endif
#define MAX_CLIENTS 5

#define PORTC 2001
#define IP "127.0.0.1"

int openClient(char* ip, int port);
void closeClient(int sock);