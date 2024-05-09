#include<iostream>
#include<Windows.h>
#include<map>
#include<string>
#include<vector>
#include<exception>
#include"ProcessUtilities.h"
#include"ThreadUtilities.h"
#include"ThreadHolder.h"

void BoostPriorityChangerSelector(HANDLE hProcess, HANDLE hThread)
{
	std::cout << "Which boost do you want to change?\n1) Process\n2) Thread\n";
	int choice;
	std::cin >> choice;
	choice == 1 ? ChangeProcessBoost(hProcess) : ChangeThreadBoost(hThread);
}

DWORD WINAPI ThreadWork(PVOID pdata);

int main()
{
	std::vector<ThreadHolder> hThreads;
	char keepGoing = 'Y';
	DWORD_PTR processAffinityMask, systemAffinityMask;
	HANDLE currentProcess = GetCurrentProcess();
	HANDLE currentThread = GetCurrentThread();
	GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask); // initially tried to be smart about this by using DWORD variables and then casting the address of these to PDWORD_PTR, but this corrupted the stack
	while (keepGoing == 'Y')
	{
		system("cls");
		std::cout << "Current process priority is: " << PsPriority.at(GetPriorityClass(currentProcess)) << '\n';
		
		std::cout << "Process Affinity Group: " << processAffinityMask << " System Affinity Mask: " << systemAffinityMask << '\n';
		std::cout << "Current thread priority is: " << ThPriority.at(GetThreadPriority(currentThread)) <<  '\n';
		if (hThreads.size() != 0)
		{
			int i = 1;
			for (const auto& hThread : hThreads)
			{
				PGROUP_AFFINITY groupAffinity = { };
				//PPROCESSOR_NUMBER idealProcessor = { };
				GetThreadGroupAffinity(hThread.GetThread(), groupAffinity);
				//GetThreadIdealProcessorEx(hThread.GetThread(), idealProcessor);
				std::cout << GetLastError() << '\n';
				std::cout << "Thread " << i << " TID(" << GetThreadId(hThread.GetThread()) << ") " << "  priority: " << ThPriority.at(GetThreadPriority(hThread.GetThread())) << "\n" <<
					"-------Group Affinity: " << groupAffinity << '\n'; // " Ideal Processor: " << idealProcessor->Number << '\n';
				i++;
			}
		}
		int choice;
		std::cout << "What do you want to do?\n1) Change Process Priority\n2) Change Thread Priority\n3) Change dynamic priority boosts\n" << 
			"4) Create a new thread and start executing the long running loop\n5) Change process affinity mask\n6) Change thread ideal processor\n7) Terminate spawned threads\n" <<
			"8) Thinking about quitting...\n";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			ChangeProcessPriority(currentProcess);
			break;
		case 2:
		{
			int threadChoice;
			std::cout << "Which thread to change the priority of?\n(thread 0 for current process)\n";
			std::cin >> threadChoice;
			if (threadChoice == 0)
			{
				ChangeThreadPriority(currentThread);
			}
			else
			{
				try
				{
					ChangeThreadPriority(hThreads.at(threadChoice - 1).GetThread());
				}
				catch (std::out_of_range e)
				{
					std::cout << "Invalid Index!\n";
				}
			}
			break;
		}
		case 3:
			BoostPriorityChangerSelector(currentProcess, currentThread);
			break;
		case 4:
		{
			HANDLE hEvent = CreateEvent(nullptr, false, false, nullptr);
			HANDLE thread = CreateThread(nullptr, 0, ThreadWork, (LPVOID)hEvent, 0, nullptr);
			hThreads.push_back(ThreadHolder(thread, hEvent));
			break;
		}
		case 5:
			ChangeProcessAffinityMask(currentProcess);
			GetProcessAffinityMask(currentProcess, &processAffinityMask, &systemAffinityMask);
			break;
		case 6:
		{
			int threadChoice;
			std::cout << "Which thread do you wish to change the affinity mask of? (thread 0 for current process)\n";
			std::cin >> threadChoice;
			if (threadChoice == 0)
			{
				ChangeThreadIdealProcessor(currentThread);
			}
			else
			{
				try
				{
					ChangeThreadIdealProcessor(hThreads.at(threadChoice - 1).GetThread());
				}
				catch (std::out_of_range e)
				{
					std::cout << "Invalid Index!\n";
				}
			}
			break;
		}
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
					hThreads.erase(std::remove_if(hThreads.begin(), hThreads.end(), [](ThreadHolder& th) { return th.CanRemoveThread(); }));
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
	CloseHandle(currentThread);
	CloseHandle(currentProcess);
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