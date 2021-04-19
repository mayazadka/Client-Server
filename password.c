#include "password.h"

// generate a numeric salted hash password
char* GenerateSaltedHash(char* plainText, char* salt)
{
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
    char* hashed_plainTextWithSalt, *hash;
    int i;
    int plainTextLen = strlen(plainText); 
    int saltLen = strlen(salt);
    BYTE plainTextWithSalt[plainTextLen + saltLen];


    // allocation of hash password and numeric hash password
    hashed_plainTextWithSalt = (char*)malloc(SHA256_BLOCK_SIZE +1); 
    hash = (char*)malloc(SHA256_BLOCK_SIZE*3*sizeof(char)+1);
    if(hashed_plainTextWithSalt == NULL || salt == NULL || plainText == NULL || hash == NULL)
    {
        return NULL;
    }

    // concatenation of password and salt
    for(i = 0; i < plainTextLen; i++)
    {
        plainTextWithSalt[i] = plainText[i];
    } 
    for (i = 0; i < saltLen; i++)
    {
        plainTextWithSalt[plainTextLen + i] = salt[i];
    }

    // calculate hash password
    sha256_init(&ctx);
    sha256_update(&ctx, plainTextWithSalt, plainTextLen + saltLen);
    sha256_final(&ctx, buf);

    // copy hash password to the allocates memory         
    memcpy(hashed_plainTextWithSalt , buf , SHA256_BLOCK_SIZE );
    hashed_plainTextWithSalt[SHA256_BLOCK_SIZE] = '\0';

    // calculate numeric hash password
    for(int i=0;i<SHA256_BLOCK_SIZE+1; i++)
    {
        sprintf(hash+i*3, "%d", (hashed_plainTextWithSalt[i]+228));
    }

    free(hashed_plainTextWithSalt);
    return hash;
}

// generate random salt
char* getSalt()
{
    FILE *f;
    int max_length = 32;  // Maximum length of salt
    int i;
    char randChar;
    char *salt;

    // allocation of memory for salt
    salt = (char*)malloc(3*max_length + 1);
    if(salt == NULL)
    {
        return NULL;
    }

    // get random salt  
    f = fopen("/dev/random", "r");
    for(i = 0 ; i < 3 * max_length + 1 ; i += 3)
    {
        fread(&randChar, sizeof(char), 1, f);
        sprintf(salt+i, "%d", (randChar+228));
    }
    fclose(f);

    return salt;
}