#include "connection.h"

int main(){
	int sock = CreateNewClient(IP, PORT);
	
	//for(int i=0;i<10;i++){
		mySend(sock, 26);
		receive(sock);
	//}
	close(sock);
}