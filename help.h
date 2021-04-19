//includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>

//structures
typedef struct strings{
	char** strings;
	int size;
    int error;
}Strings;

//functions
int stringToPositiveFloat(char* string, float *number);
void freeStrings(Strings strings);
Strings split(char* str);
char* appendStrings(int count, ...);
int stringToFloat(char* string, float *number);
int stringToInt(char* string, int *number);
int onlyNumbers(char* string);
int onlyLetters(char* string);
int emailPattern(char* string);
int onlyLettersAndNumbers(char* string);