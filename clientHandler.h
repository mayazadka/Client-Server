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

void* handleClient(void* args);