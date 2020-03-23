#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "GameProfile.h"

inline int wait_time = 1 * 1000;

// Template Class to write a value to a memory location
template <class dataType>
void write_memory(dataType value_write, DWORD address_read, HANDLE hProc)
{
	WriteProcessMemory(hProc, (PVOID)address_read, &value_write, sizeof(dataType), 0);
}

// Template Class to read a value at a memory location
template <class dataType>
dataType read_memory(DWORD address_read, HANDLE hProc)
{
	dataType read_process_memory_buffer;

	ReadProcessMemory(hProc, (PVOID)address_read, &read_process_memory_buffer, sizeof(dataType), 0);
	return read_process_memory_buffer;
}

class MemorySniff
{
public:
	explicit MemorySniff(game_profile *game_ptr);
	void sniff_process();

private:
	game_profile *game_;
	HANDLE hProc;
	DWORD procID;
	boolean get_process();
	void read_process();
};
