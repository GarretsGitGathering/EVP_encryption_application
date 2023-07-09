#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <string>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/conf.h>
#include <openssl/rand.h>

#include "main.h"
#include "encryptor.c"

// TODO: change GenerateKey() to actually create a 256 bit key and not just a 32 char key https://stackoverflow.com/questions/33325814/how-to-loop-generate-random-256bit-hex-in-c-or-c

#define KeyLength (256/8)

unsigned char key[16];                                              // made public for easy passing 
unsigned char iv[16];

// INITIALIZE STRING WITH MonoString* 
void DE::Encryptor(MonoString* directory, MonoString* ext)
{
  char* direct = mono_string_to_utf8(directory);                    // cast char to utf8 for string conversion
  std::cout << "The directory is: " << direct << "\n";              // otherwise it would be it's location 
                                                                    // address in memory
  char* extension = mono_string_to_utf8(ext);
  std::cout << "The extension is: " << extension << "\n";

  char collected_files[100][100] = {};
  char targeted_files[100][100] = {};

  DE DE;  // create DE instance
  DE.Generate_key_iv(key, iv);
  DE.Full_Encrypt(collected_files, targeted_files, direct, extension, key, iv);
  DE.file_key_iv(direct);

  mono_free(direct);                                                // clears place in memory
  mono_free(extension);

  std::cout << collected_files[100][100] << std::endl;
}

extern "C" void DE::Update_key_iv(unsigned char* key, unsigned char* iv) {    // update key and iv from c to cpp
// Definition of the C++ function to be called from C.
// The function is written in C++, only the linkage is C.
  printf("key: %u\n", key);
  printf("iv: %u\n\n", iv);
}

void DE::file_key_iv(char direct[100]){
  unsigned char path[100] = "";

  strcat(path, direct);                                             // place directory into path
  strcat(path , "key_and_iv.txt");                                  // place file into path

  FILE *file = fopen (path, "w+");                                  // open path
              
  fprintf(file, "key: %u | iv: %u", key, iv);                       // put the key and iv into file
  //printf("passed key in cpp: %u\npassed iv in cpp: %u\n", key, iv); 
  fclose(file);
  
  printf("key file created!\n");
}

MonoString* DE::Return_key_and_iv(MonoString* directory){
  char* direct = mono_string_to_utf8(directory);       // cast char to utf8 for string conversion  // otherwise it would be it's location 

  char path[100] = "";
  strcat(path, direct);                                // place directory into path
  strcat(path , "key_and_iv.txt");                     // place file into path

  mono_free(direct);                                   // clears place in memory

  std::ifstream MyReadFile(path);                      // Read from the text file
  std::string myText;
  char str[200] = "";

  while (getline (MyReadFile, myText)){               // Convert string to char array
    for (int x = 0; x < sizeof(myText); x++) {
      str[x] = myText[x]; 
      //std::cout << str[x]; 
    }
  }

  MyReadFile.close();                                  // Close the file
  return mono_string_new (mono_domain_get (), str);    // return contents of file
}

int main() {

  // SET PATHS FOR MONO DIRECTORIES
  mono_set_dirs("/usr/lib/", "/etc/");

  // INITIALIZE THE DOMAIN 
  MonoDomain* domain = mono_jit_init("DE_domain");
  if(!domain){
    std::cout << "mono_jit_init failed" << std::endl;
    return 1;
  }

  // POINT MONO RUTIME TO CONFIG FILE WITH POINTERS TO BINARIES
  mono_config_parse("/etc/mono/config");

  // INITIALIZE THE MONO ASSEMBLY
  MonoAssembly* csharpAssembly = mono_domain_assembly_open(domain, "DE.exe");
  if(!csharpAssembly){
    std::cout << "mono_domain_assembly_open failed" << std::endl;
    return 1;
  }

  // GET ASSEMBLY IMAGE (necessary to get methods from c# with mono_method_desc_new)
  MonoImage* image;
	image = mono_assembly_get_image(csharpAssembly);
	if (!image){
		std::cout << "mono_assembly_get_image failed" << std::endl;
		return 1;
	}

  // ESTABLISH INTERNAL CALLS AND GRAB ASSEMBLIES
  mono_add_internal_call("DE.MainClass::Encryptor(string,string)", &DE::Encryptor);
  mono_add_internal_call("DE.MainClass::Return_key_and_iv(string)", &DE::Return_key_and_iv);

  // POINTS IN THE MEMORY TO START... idk im high rn and dont remember
  int argc = 1;
  char* argv[1] = { (char*)"CSharp" };

  // INITIALIZE THE RUNTIME
  mono_jit_exec(domain, csharpAssembly, argc, argv);

  //run the method ... no methods aalled from c# in this project so not needed :)
	//std::cout << "Running the static method: " << TypeMethodDescStr << std::endl;
	//mono_runtime_invoke(method, nullptr, nullptr, nullptr);

  // CLEAN UP THE DOMAIN IN REGISTER (clear it...)
  mono_jit_cleanup (domain);

  return 0;
}