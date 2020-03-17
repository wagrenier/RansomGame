#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

template <class dataType>
void write_memory(dataType value_write, DWORD address_read, HANDLE hProc)
{
	WriteProcessMemory(hProc, (PVOID)address_read, &value_write, sizeof(dataType), 0);
}

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
	void sniff_process(const char* proc_name);

private:
	HANDLE hProc;
	DWORD procID;
	boolean get_process(const char* proc_name);
	void read_process();
};
