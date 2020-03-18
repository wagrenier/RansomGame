#include "MemorySniff.h"

void MemorySniff::sniff_process(const char* proc_name)
{
	while(!this->get_process(proc_name))
	{
		// Waits 1 seconds
		Sleep(1 * 1000);
	}

	this->read_process();
}

boolean MemorySniff::get_process(const char* proc_name)
{
	PROCESSENTRY32 proc_entry32;

	proc_entry32.dwSize = sizeof(proc_entry32);

	// Takes a snapshot of all currently running processes
	const auto h_proc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(h_proc_snap == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to take snapshot of processes." << std::endl;
		return false;
	}

	while(Process32Next(h_proc_snap, &proc_entry32))
	{
		std::cout << proc_entry32.szExeFile << std::endl;

		// Change here to that it only compares with str.find(str2)
		if(strstr(proc_name, proc_entry32.szExeFile) != nullptr)
		{
			std::cout << "Found Process " << proc_entry32.szExeFile << " with process ID " << proc_entry32.th32ProcessID << std::endl;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_entry32.th32ProcessID);
			
			procID = proc_entry32.th32ProcessID;
			
			if (hProc == nullptr)
			{
				std::cout << "Cannot Obtain Process." << std::endl;
			}
			
			CloseHandle(h_proc_snap);
			return true;
		}
	}

	std::cout << "Could not find process." << std::endl;
	return false;	
}

void MemorySniff::read_process()
{
	int difficulty;
	int score;

	while(true)
	{
		// Scoring points seem to be at a static address -> 0x0069BCA0
		// Difficulty address is at -> 0x0069BCB0
		difficulty = read_memory<int>(0x0069BCB0, hProc);
		score = read_memory<int>(0x0069BCA0, hProc);
		std::cout << "Score: " << score << ", Difficulty: " << difficulty << std::endl;
		Sleep(1 * 1000);
	}
	
}
