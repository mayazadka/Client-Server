
#include "client.h" 
// ---------------------------------------------------------------------------------------------------------
// creats a new client that tries to connect to our server (this method us neccery for testing purposes only)
// ---------------------------------------------------------------------------------------------------------
int openClient(char* ip, int port)
{
	int sock;
	struct sockaddr_in client_name;
	
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Error opening channel\n");
		close(sock);
		exit(1);
	}
	printf("Client is alive and establishing socket connection. \n");
	bzero(&client_name,sizeof(client_name));
	client_name.sin_family = AF_INET;
	client_name.sin_addr.s_addr = inet_addr(ip);
	client_name.sin_port = htons(port);
	
	if(connect(sock, (struct sockaddr *)&client_name, sizeof(client_name)) < 0)
	{
		printf("Error establishing communications\n");
		close(sock);
		exit(1);
	}
	printf("Connected to server.\n");
	return sock;// needs to be recv(sock)
}
void closeClient(int sock) {
	printf("Closing client %d channel\n", sock);
	close(sock);
	exit(0);
}