#include "connection.h"
//
//
//
int CreateNewClient(char* ip, int port) {
	info_client info; 
	printf("%s, %d, ", ip, port);
	info.ip = (char*)malloc(sizeof(ip));
	strcpy(info.ip, ip);
	info.port = port;
	return openClient(&info);
}
//
//opens a server in a new thread (using openServer)
//
int openSerialServer(int port) 
{
	pthread_t tid;
	int error;
	info_server info;
	info.port = port;

	error = pthread_create(&tid, NULL, openServer, &info);
	if(error != 0)
	{
		printf("error with creating thread\n");
		exit(1);
	}
	pthread_join(tid, NULL);
	info.tid = tid;
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
		printf("Error with reading\n");
		close(sock);
		exit(1);
	}
	printf("%d has received %d from socket.\n", sock, value);
}