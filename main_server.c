#include"server.h"

int main() {
    int* sock = openServer(PORTS);
	sleep(10);
    close(*sock);
}