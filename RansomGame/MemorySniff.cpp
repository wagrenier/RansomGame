#include "MemorySniff.h"

MemorySniff::MemorySniff(game_profile* game_ptr)
{
	game_ = game_ptr;
}

void MemorySniff::sniff_process()
{
	while(!this->get_process())
	{
		// Waits 1 seconds
		Sleep(wait_time);
	}

	this->read_process();
}

boolean MemorySniff::get_process()
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

		// Finds a process that contains the string of the game title
		if(strstr(game_->game_title, proc_entry32.szExeFile) != nullptr)
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
	int read_difficulty;
	int read_score;

	while(true)
	{
		read_difficulty = read_memory<int>(game_->difficulty_memory_address, hProc);
		read_score = read_memory<int>(game_->score_memory_address, hProc);

		if(read_difficulty == lunatic && read_score > game_->minimum_score)
		{
			std::cout << "Unlocking files." << std::endl;
			break;
		}

		if(read_difficulty != lunatic)
		{
			std::cout << "Wrong difficulty Selected." << std::endl;
		}
		
		std::cout << "Score: " << read_score << ", Difficulty: " << read_difficulty << std::endl;
		Sleep(wait_time);
	}
}