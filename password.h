//includes
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <linux/random.h>

#include"sha256.h"

//functions
char* GenerateSaltedHash(char* plainText,char* salt);
char* getSalt();