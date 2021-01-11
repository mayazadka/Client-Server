#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#ifdef __linux__         // UNIX/Linux
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif
struct info_sockh{
    pthread_t tid;
    int connect_sock;
}typedef info_sockh;
void* handleClient(void* args);