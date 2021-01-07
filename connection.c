#include "connection.h"

int CreateNewClient(char* ip, int port) {
	printf("%s, %d, ", ip, port);
	info_client info; 
	info.ip = (char*)malloc(sizeof(ip));
	strcpy(info.ip, ip);
	info.port = port;
	return openClient(&info);
}
int openSerialServer(int port) {
	pthread_t tid;
	int err;
	info_server info;
	info.port = port;

	err = pthread_create(&tid, NULL, openServer, &info);
	if(err != 0)
	{
		printf("error with creating thread\n");
		exit(1);
	}
	return tid;
}
void mySend(int sock, int number){
	if(write(sock, &number, sizeof(int)) < 0)
	{
		printf("Error with writing");
		close(sock);
		exit(1);
	}
	sleep(1);
	printf("%d has written %d to socket.\n", sock, number);
}
void receive(int sock) {
	int value;
	if(read(sock, &value, 4) < 0)
	{
		printf("Error with reading");
		close(sock);
		exit(1);
	}
	printf("%d has received %d from socket.\n", sock, value);
}