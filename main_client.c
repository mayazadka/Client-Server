#include "client.h"
#include "log.h"

int main(int argc, char** argv){
	char bufInput[1000] = "";
	char bufOutput[1000] = "";
	int sock;
	int n;
	int error;
	creatLog();
	error = openClient(IP, PORTC, &sock);
	if(error > 0)
	{
		writeToLog("error : " + error);
		return error;
	}
	while(1)
	{
        puts("enter command:");
        printf("> ");
    	fgets(bufInput, 1000, stdin); 
		write(sock, bufInput, strlen(bufInput) - 1);
		if(strcmp(bufInput, "quit") == 0)
		{
			n = read(sock, bufOutput, 1000);
			bufOutput[n] = '\0';
			puts(bufOutput);
			puts("");
			break;
		}
		n = read(sock, bufOutput, 1000);
		bufOutput[n] = '\0';
		puts(bufOutput);
        puts("");
		if(strncmp(bufOutput, "error", 5) == 0)
			break;
		else if(strcmp(bufOutput, "bye") == 0)
			break;
    }
	closeClient(sock);
	return 0;
}