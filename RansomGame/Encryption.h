#pragma once
#include <string>
#include <utility>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cryptlib.h>
#include <rsa.h>
#include <files.h>
#include <modes.h>
#include <osrng.h>
#include <rsa.h>
#include <sha.h>

class Encryption
{
public:
	Encryption() = default;
	~Encryption() = default;
	void run_encryption_logic();

private:
	const char* encryption_file_ = "rsakey.der";
	const char* custom_extension_name = ".encryptgame";
	CryptoPP::AutoSeededRandomPool rng_;
	CryptoPP::RSAES_OAEP_SHA_Decryptor* rsaes_oaep_sha_decryptor_;
	void encrypt_files(const char* file_name);
	void decrypt_files(const char* file_name);
	void generate_rsa_private_key();
	
};


