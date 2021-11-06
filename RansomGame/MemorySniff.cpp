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
#ifdef DEBUG
		std::cout << "Failed to take snapshot of processes." << std::endl;
#endif

		return false;
	}

	while(Process32Next(h_proc_snap, &proc_entry32))
	{
#ifdef DEBUG
		std::cout << proc_entry32.szExeFile << std::endl;
#endif

		// Finds a process that contains the string of the game title
		if(strstr(game_->game_title, proc_entry32.szExeFile) != nullptr)
		{
#ifdef DEBUG
			std::cout << "Found Process " << proc_entry32.szExeFile << " with process ID " << proc_entry32.th32ProcessID << std::endl;
#endif
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_entry32.th32ProcessID);
			
			procID = proc_entry32.th32ProcessID;
			
			if (hProc == nullptr)
			{
#ifdef DEBUG
				std::cout << "Cannot Obtain Process." << std::endl;
#endif
			}
			
			CloseHandle(h_proc_snap);
			return true;
		}
	}
#ifdef DEBUG
	std::cout << "Could not find process." << std::endl;
#endif

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
#ifdef DEBUG
			std::cout << "Unlocking files." << std::endl;
#endif
			break;
		}

		if(read_difficulty != lunatic)
		{
#ifdef DEBUG
			std::cout << "Wrong difficulty Selected." << std::endl;
#endif
		}

#ifdef DEBUG
		std::cout << "Score: " << read_score << ", Difficulty: " << read_difficulty << std::endl;
#endif
		Sleep(wait_time);
	}
}