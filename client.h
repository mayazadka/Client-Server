#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTC 7777
#define IP "127.0.0.1"

void clean_up(int cond, int sock);
int openClient(char* ip, int port);
void closeClient(int sock);