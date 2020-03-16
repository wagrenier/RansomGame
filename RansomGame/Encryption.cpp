#include "Encryption.h"

void Encryption::run_encryption_logic()
{
	const auto private_key = this->generate_rsa_private_key();
}

void Encryption::encrypt_files()
{
}

CryptoPP::RSAES_OAEP_SHA_Decryptor* Encryption::generate_rsa_private_key()
{
	CryptoPP::RSAES_OAEP_SHA_Decryptor *private_key;
	
	
	if(std::filesystem::exists(encryption_file_))
	{
		CryptoPP::FileSource rdr(encryption_file_, true);
		private_key = new CryptoPP::RSAES_OAEP_SHA_Decryptor(rdr);
	} else
	{
		private_key = new CryptoPP::RSAES_OAEP_SHA_Decryptor(rng_, 4096);
		CryptoPP::TransparentFilter private_file(new CryptoPP::FileSink(encryption_file_));
		private_key->GetPrivateKey().Save(private_file);
	}

	std::cout << "Prime key 1: " << private_key->GetKey().GetPrime1() << std::endl;
	std::cout << "\nPrime key 2: " << private_key->GetKey().GetPrime2() << std::endl;
	std::cout << "\nPrivate exp: " << private_key->GetKey().GetPrivateExponent() << std::endl;
	std::cout << "\nModulus: " << private_key->GetKey().GetModulus() << std::endl;
	
	return private_key;
}
