#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__         // UNIX/Linux			
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

struct info_client{
    pthread_t tid;
    int port;
    char* ip;
}typedef info_client;

int openClient(void* args);
void closeClient(int sock);