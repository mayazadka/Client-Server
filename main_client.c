#include "client.h"

int main(int argc, char** argv){
	char bufInput[100] = "";
	char bufOutput[100] = "";
	int sock;
	int n;

	sock = openClient(IP, PORTC);
	do {
        puts("enter command:");
        printf("> ");
        gets(bufInput);
		write(sock, bufInput, strlen(bufInput));
		if(strcmp(bufInput, "quit") == 0)
			break;
		n = read(sock, bufOutput, 100);
		bufOutput[n] = '\0';
		puts(bufOutput);
        puts("");
		if(strcmp(bufOutput, "error") == 0)
			break;
    }while(strcmp(bufInput, "quit")!=0);
	closeClient(sock);
}