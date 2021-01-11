#include "client.h"

int main(){
	int sock = openClient(IP, PORTC);
	char* buf = malloc(100);
	read(sock, buf, 8);
	printf(buf);
}