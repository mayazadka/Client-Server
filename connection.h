#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif

#include "client.h"
#include "server.h"


#define PORT 2000
#define IP "127.0.0.1"

int CreateNewClient(char* ip, int port);
int openSerialServer(int port);
void mySend(int sock, int number);
void receive(int sock);
