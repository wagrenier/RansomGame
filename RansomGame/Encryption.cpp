#include "Encryption.h"

void Encryption::run_encryption_logic()
{
	this->generate_rsa_private_key();
	this->encrypt_files("my_text.txt");

	
	if (remove("my_text.txt") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");
	
	this->decrypt_files("my_text.txt.encryptgame");
}

void Encryption::encrypt_files(const char* file_name)
{
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(this->rsaes_oaep_sha_decryptor_->GetPublicKey());
	std::string file_name_string = file_name;
	
	const auto full_dest = const_cast<char*>(file_name_string.append(custom_extension_name).c_str());
	const auto encrypted_file_dest = new CryptoPP::FileSink(full_dest);
	CryptoPP::FileSource(file_name, true, new CryptoPP::PK_EncryptorFilter(rng_, e, encrypted_file_dest));
}

void Encryption::decrypt_files(const char* file_name)
{
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(this->rsaes_oaep_sha_decryptor_->GetPrivateKey());
	

	std::string file_name_string = file_name;
	const auto last_index = file_name_string.find_last_of(".");
	
	if (last_index == std::string::npos) {
		return;
	}
	
	file_name_string = file_name_string.substr(0, last_index);
	const auto full_name = const_cast<char*>(file_name_string.c_str());
	
	const auto decrypted_file_dest = new CryptoPP::FileSink(full_name);
	CryptoPP::FileSource(file_name, true, new CryptoPP::PK_DecryptorFilter(rng_, d, decrypted_file_dest));
}

void Encryption::generate_rsa_private_key()
{
	// Checks if an encryption file already exists, if so, it loads it
	if(std::filesystem::exists(encryption_file_))
	{
		CryptoPP::FileSource rdr(encryption_file_, true);
		this->rsaes_oaep_sha_decryptor_ = new CryptoPP::RSAES_OAEP_SHA_Decryptor(rdr);
	} else
	{
		// Otherwise it creates a new one
		this->rsaes_oaep_sha_decryptor_ = new CryptoPP::RSAES_OAEP_SHA_Decryptor(rng_, 4096);
		CryptoPP::TransparentFilter private_file(new CryptoPP::FileSink(encryption_file_));
		this->rsaes_oaep_sha_decryptor_->GetPrivateKey().Save(private_file);
	}

	std::cout << "Prime key 1: " << this->rsaes_oaep_sha_decryptor_->GetKey().GetPrime1() << std::endl;
	std::cout << "\nPrime key 2: " << this->rsaes_oaep_sha_decryptor_->GetKey().GetPrime2() << std::endl;
	std::cout << "\nPrivate exp: " << this->rsaes_oaep_sha_decryptor_->GetKey().GetPrivateExponent() << std::endl;
	std::cout << "\nModulus: " << this->rsaes_oaep_sha_decryptor_->GetKey().GetModulus() << std::endl;
}


