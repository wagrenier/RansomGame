# RansomGame
![](images/demo.png)

## Decription
**WARNING**
This application will encrypt your data, **run at your own risk!** Do not close the application or else the encryption key will be **lost**. The application only encrypts what is located in the current folder of the executable, so do not run at the root of your drive.

RansomGame is an educational app to learn about windows app memory sniffing using ```window.h```.
RansomGame encrypts files using AES and waits for a specific process to be launched (the specific process is given below). Once the process is launched, it looks int its memory for a specific values (score and difficulty).

## Decrypt Files
In order to decrypt the files, a score of 0.2 billion must be achieved in the lunatic difficulty.

## Build
The project requires an external library [crypto++](https://github.com/weidai11/cryptopp) in order to build. Be sure to install the submodules with the git command. Once downloaded, the Visual Studio Compiler should automatically compile the binaries of crypto++.

## Game
The required game to unlock the files is [Touhou 6: Koumakyou - The Embodiment of Scarlet Devil ](https://moriyashrine.org/files/file/13-touhou-6-koumakyou-the-embodiment-of-scarlet-devil/).

## Reverse Engineering
I reversed the game's code to find the address of the difficulty index. I searched for a string containing the word ```Lunatic``` and found this branching. The game loads a ```DWORD``` value from ```0x0069BCB0```. After checking with CheatEngine, this value does change accordingly to the selected difficulty, indicating that the difficulty is stored at that memory location.

![](images/reverse.png)

## Wanted Features
- [ ] Save AES key to a file
- [ ] Decryptor
- [ ] Bug testing for file handling
- [ ] UI for a cleaner experience
- [ ] Prevent force closing the application
