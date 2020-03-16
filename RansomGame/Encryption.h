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
	CryptoPP::AutoSeededRandomPool rng_;
	CryptoPP::RSA::PublicKey public_key_ = CryptoPP::RSA::PublicKey();
	CryptoPP::RSA::PrivateKey private_key_ = CryptoPP::RSA::PrivateKey();
	void encrypt_files();
	CryptoPP::RSAES_OAEP_SHA_Decryptor* generate_rsa_private_key();
	
};


