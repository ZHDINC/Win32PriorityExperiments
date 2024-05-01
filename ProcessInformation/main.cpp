#include<iostream>
#include<Windows.h>
#include<map>
#include<string>

const std::map<DWORD, std::string> PsPriority = {
	{IDLE_PRIORITY_CLASS, "Idle Priority"},
	{BELOW_NORMAL_PRIORITY_CLASS, "Below Normal Priority"},
	{NORMAL_PRIORITY_CLASS, "Normal Priority"},
	{ABOVE_NORMAL_PRIORITY_CLASS, "Above Normal Priority"},
	{HIGH_PRIORITY_CLASS, "High Priority"},
	{REALTIME_PRIORITY_CLASS, "Real-time Priority"}
};

const std::map<DWORD, std::string> ThPriority = {
	{THREAD_PRIORITY_IDLE, "Idle Priority"},
	{THREAD_PRIORITY_LOWEST, "Lowest Priority"},
	{THREAD_PRIORITY_BELOW_NORMAL, "Below Normal Priority"},
	{THREAD_PRIORITY_NORMAL, "Normal Priority"},
	{THREAD_PRIORITY_ABOVE_NORMAL, "Above Normal Priority"},
	{THREAD_PRIORITY_HIGHEST, "Highest Priority"},
	{THREAD_PRIORITY_TIME_CRITICAL, "Time Critical Priority"}
};

DWORD ProcessPrioritySwitch(const int& choice, bool& defaultchoice)
{
	DWORD priority;
	switch (choice)
	{
	case 1:
		priority = IDLE_PRIORITY_CLASS;
		break;
	case 2:
		priority = BELOW_NORMAL_PRIORITY_CLASS;
		break;
	case 3:
		priority = NORMAL_PRIORITY_CLASS;
		break;
	case 4:
		priority = ABOVE_NORMAL_PRIORITY_CLASS;
		break;
	case 5:
		priority = HIGH_PRIORITY_CLASS;
		break;
	case 6:
		priority = REALTIME_PRIORITY_CLASS;
		break;
	default:
		priority = NORMAL_PRIORITY_CLASS;
		defaultchoice = true;
		break;
	}
	return priority;
}

DWORD ThreadPrioritySwitch(const int& choice, bool& defaultchoice)
{
	DWORD priority;
	switch (choice)
	{
	case 1:
		priority = THREAD_PRIORITY_IDLE;
		break;
	case 2:
		priority = THREAD_PRIORITY_LOWEST;
		break;
	case 3:
		priority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case 4:
		priority = THREAD_PRIORITY_NORMAL;
		break;
	case 5:
		priority = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case 6:
		priority = THREAD_PRIORITY_HIGHEST;
		break;
	case 7:
		priority = THREAD_PRIORITY_TIME_CRITICAL;
		break;
	default:
		priority = THREAD_PRIORITY_NORMAL;
		defaultchoice = true;
		break;
	}
	return priority;
}

void ChangeProcessPriority()
{
	HANDLE hProcess = GetCurrentProcess();
	std::cout << "Which priority do you wish to change to process to?\n1) Idle\n2) Below Normal\n3) Normal\n4) Above Normal\n5) Highest\n6) Real-time\n";
	int choice;
	std::cin >> choice;
	bool defaultchoice = false;
	DWORD priority = ProcessPrioritySwitch(choice, defaultchoice);
	std::cout << "Attempting to set the current process to " << PsPriority.at(priority) << "...\n";
	if (defaultchoice)
	{
		std::cout << "You chose an invalid option, so went with defaults.\n";
	}
	if (priority == REALTIME_PRIORITY_CLASS)
	{
		std::cout << "If this process doesn't have SeIncreaseBasePriorityPrivilege, then this will go to high priority values.\n";
	}
	BOOL success = SetPriorityClass(hProcess, priority);
	if (success)
	{
		std::cout << "Successfully set priority class! Check Process Explorer!\n";
	}
	else
	{
		std::cout << "Failed to set priority class! Check last error!\n";
		std::cout << GetLastError();
	}
	CloseHandle(hProcess);
}

void ChangeThreadPriority()
{
	HANDLE hThread = GetCurrentThread();
	int choice;
	std::cout << "Which priority do you wish to set the current thread to?\n1) Idle\n2)Lowest\n3) Below Normal\n4) Normal\n5) Above Normal\n6) Highest\n7) Time Critical\n";
	std::cin >> choice;
	bool defaultchoice = false;
	DWORD priority = ThreadPrioritySwitch(choice, defaultchoice);
	std::cout << "Attempting to set current thread to " << ThPriority.at(priority) << "...\n";
	if (defaultchoice)
	{
		std::cout << "You chose an invalid option, so went with defaults.\n";
	}
	BOOL threadSuccess = SetThreadPriority(hThread, priority);
	if (threadSuccess)
	{
		std::cout << "Successfully set thread priority! Check Process Explorer!\n";
		std::cout << "Thread Priority is currently: " << ThPriority.at(GetThreadPriority(hThread)) << '\n';
	}
	else
	{
		std::cout << "Failed to set thread priority! Check last error!\n";
		std::cout << GetLastError();
	}
	CloseHandle(hThread);
}

void LongRunningLoop()
{
	char beginLoop;
	std::cout << "Awaiting performance monitor ready...";
	std::cin >> beginLoop;
	for (int i = 0; i < 1000000; i++)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

int main()
{
	char keepGoing = 'Y';
	while (keepGoing == 'Y')
	{
		system("cls");
		std::cout << "Current process priority is: " << PsPriority.at(GetPriorityClass(GetCurrentProcess())) << '\n';
		std::cout << "Current thread priority is: " << ThPriority.at(GetThreadPriority(GetCurrentThread())) << '\n';
		int choice;
		std::cout << "What do you want to do?\n1) Change Process Priority\n2) Change Thread Priority\n3) Initiate a long running loop to track thread state changes\n4) Thinking about quitting...\n";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			ChangeProcessPriority();
			break;
		case 2:
			ChangeThreadPriority();
			break;
		case 3:
			LongRunningLoop();
			break;
		case 4:
			break;
		default:
			std::cout << "Did not understand that choice. Please try again...\n";
			break;
		}
		std::cout << "Keep going? (Y/N)";
		std::cin >> keepGoing;
	}
}

// Awaiting refactor for start/stop background mode...
//char stopBackgroundMode;
//if (!success)
//{
//	std::cout << "Stop background mode?\n";
//	std::cin >> stopBackgroundMode;
//	SetPriorityClass(hProcess, PROCESS_MODE_BACKGROUND_END);
//}