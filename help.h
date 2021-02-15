#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef struct strings{
	char** strings;
	int size;
    int error;
}Strings;


int stringToPositiveFloat(char* string, float *number);

void freeStrings(Strings strings);

Strings split(char* str);

char* appendStrings(int count, ...);

int stringToFloat(char* string, float *number);

int stringToInt(char* string, int *number);