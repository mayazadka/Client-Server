#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "help.h"
#ifdef __linux__            // UNIX/Linux
#include <netinet/in.h>
#include <mysql.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
struct info_sockh{
    pthread_t tid;
    int connect_sock;
}typedef info_sockh;
void* handleClient(void* args);
MYSQL_RES * queryAndResponse(int isResult, MYSQL *con, char* query);