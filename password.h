#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"sha256.h"
#include <linux/random.h>

char* GenerateSaltedHash(char* plainText,char* salt);
char* getSalt();