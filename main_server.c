#include "connection.h"

int main() {
    int sock = openSerialServer(PORT);
    for(int i = 0 ; i < 10 ; i++){
		receive(sock);
        mySend(sock, i);
	}
	close(sock);
}