#include "password.h"

        char* GenerateSaltedHash(char* plainText, char* salt)
        {
            BYTE buf[SHA256_BLOCK_SIZE];
            SHA256_CTX ctx;
            char* hashed_plainTextWithSalt = (char*)malloc(SHA256_BLOCK_SIZE);
            int i;
            int plainTextLen = strlen(plainText); 
            int saltLen = strlen(salt);
            BYTE plainTextWithSalt[plainTextLen + saltLen];

            //
            for(i = 0; i < plainTextLen; i++)
            {
                plainTextWithSalt[i] = plainText[i];
            }
           
            for (i = 0; i < saltLen; i++)
            {
                plainTextWithSalt[plainTextLen + i] = salt[i];
            }
            plainTextWithSalt[plainTextLen + i] = '\0';

            sha256_init(&ctx);
            sha256_update(&ctx, plainTextWithSalt, plainTextLen + saltLen);
            sha256_final(&ctx, buf);
             

            memcpy(hashed_plainTextWithSalt , buf , SHA256_BLOCK_SIZE );
    
            return hashed_plainTextWithSalt;
        }
        char* getSalt()
        {
            unsigned int randval;
            FILE *f;
            int i;
           
            int max_length = 32;  // Maximum length of salt
            
            char *salt = (char*)malloc((sizeof(int) * max_length) + 1); // Empty salt array
                    
            f = fopen("/dev/random", "r");
            fread(&randval, sizeof(randval), 1, f);

            // Build the random bytes
            for(i = 0 ; i < max_length*4 ; i+=4)
            {
                fread(&randval, sizeof(randval), 1, f);
                sprintf(salt + i , "%u" , randval); 
            }
            fclose(f);
            // Return the string encoded salt
            return salt;
        }