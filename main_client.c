#include "connection.h"

int main(){
	int sock = CreateNewClient(IP, PORT);
	
	for(int i=0;i<10;i++){
		mySend(sock, i);
		receive(sock);
	}
	close(sock);
}