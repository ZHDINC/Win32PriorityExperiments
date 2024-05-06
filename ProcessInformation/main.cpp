#include<iostream>
#include<Windows.h>
#include<map>
#include<string>
#include<vector>
#include<exception>
#include"ProcessUtilities.h"
#include"ThreadUtilities.h"
#include"ThreadHolder.h"

void BoostPriorityChangerSelector()
{
	std::cout << "Which boost do you want to change?\n1) Process\n2) Thread\n";
	int choice;
	std::cin >> choice;
	choice == 1 ? ChangeProcessBoost() : ChangeThreadBoost();
}

void LongRunningLoop(bool* flag)
{
	//char beginLoop;
	//std::cout << "Awaiting performance monitor ready...";
	//std::cin >> beginLoop;
	int count = 0;
	while(*flag == false)
	{
		count++;
		//std::cout << i << ' ';
	}
	//std::cout << '\n';
}

DWORD WINAPI ThreadWork(PVOID pdata);

int main()
{
	std::vector<ThreadHolder> hThreads;
	char keepGoing = 'Y';
	DWORD_PTR processAffinityMask, systemAffinityMask;
	GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask); // initially tried to be smart about this by using DWORD variables and then casting the address of these to PDWORD_PTR, but this corrupted the stack
	while (keepGoing == 'Y')
	{
		system("cls");
		std::cout << "Current process priority is: " << PsPriority.at(GetPriorityClass(GetCurrentProcess())) << '\n';
		
		std::cout << "Process Affinity Group: " << processAffinityMask << " System Affinity Mask: " << systemAffinityMask << '\n';
		std::cout << "Current thread priority is: " << ThPriority.at(GetThreadPriority(GetCurrentThread())) <<  '\n';
		if (hThreads.size() != 0)
		{
			int i = 1;
			for (const auto& hThread : hThreads)
			{
				std::cout << "Thread " << i << "  priority: " << ThPriority.at(GetThreadPriority(hThread.GetThread())) << "\n";
				i++;
			}
		}
		int choice;
		std::cout << "What do you want to do?\n1) Change Process Priority\n2) Change Thread Priority\n3) Initiate a long running loop to track thread state changes\n" << 
			"4) Change dynamic priority boosts\n5) Create a new thread and start executing the long running loop\n6) Change process affinity mask\n7) Terminate spawned threads\n" <<
			"8) Thinking about quitting...\n";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			ChangeProcessPriority();
			break;
		case 2:
		{
			int threadChoice;
			std::cout << "Which thread to change the priority of?\n(thread 0 for current process)\n";
			std::cin >> threadChoice;
			if (threadChoice == 0)
			{
				ChangeThreadPriority(GetCurrentThread());
			}
			else
			{
				try
				{
					ChangeThreadPriority(hThreads.at(threadChoice-1).GetThread());
				}
				catch(std::out_of_range e)
				{
					std::cout << "Invalid Index!\n";
				}
			}
			break;
		}
		case 3:
			// LongRunningLoop();
			break;
		case 4:
			BoostPriorityChangerSelector();
			break;
		case 5:
		{
			HANDLE hEvent = CreateEvent(nullptr, false, false, nullptr);
			HANDLE thread = CreateThread(nullptr, 0, ThreadWork, (LPVOID)hEvent, 0, nullptr);
			hThreads.push_back(ThreadHolder(thread, hEvent));
			break;
		}
		case 6:
			ChangeProcessAffinityMask();
			GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);
			break;
		case 7:
		{
			if (hThreads.size() == 0)
			{
				std::cout << "There are no extra threads to terminate!\n";
			}
			else
			{
				std::cout << "Which thread do you wish to terminate? ";
				int choice;
				std::cin >> choice;
				try
				{
					SetEvent(hThreads.at(choice - 1).GetEvent());
					hThreads.at(choice - 1).ThreadFinished();
					std::erase_if(hThreads, [&](ThreadHolder& th) { return th.CanRemoveThread(); });
				}
				catch (std::out_of_range e)
				{
					std::cout << "Invalid Index!\n";
				}
			}
			break;
		}
		case 8:
			break;
		default:
			std::cout << "Did not understand that choice. Please try again...\n";
			break;
		}
		std::cout << "Keep going? (Y/N)";
		std::cin >> keepGoing;
	}
}

DWORD WINAPI ThreadWork(PVOID pdata)
{
	HANDLE hEvent = (HANDLE)pdata;
	int count = 0;
	while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0)
	{
		count++;
	}
	return 0;
}

// Awaiting refactor for start/stop background mode...
//char stopBackgroundMode;
//if (!success)
//{
//	std::cout << "Stop background mode?\n";
//	std::cin >> stopBackgroundMode;
//	SetPriorityClass(hProcess, PROCESS_MODE_BACKGROUND_END);
//}