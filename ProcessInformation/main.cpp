#include<iostream>
#include<Windows.h>

void ChangeProcessPriority()
{
	HANDLE hProcess = GetCurrentProcess();
	std::cout << "Which priority do you wish to change to process to?\n1) Idle\n2) Below Normal\n3) Normal\n4) Above Normal\n5) Highest\n6) Real-time\n";
	int choice;
	std::cin >> choice;
	
	DWORD priority;
	switch (choice)
	{
	case 1:
		priority = IDLE_PRIORITY_CLASS;
		std::cout << "Attempting set to Idle priority...\n";
		break;
	case 2:
		priority = BELOW_NORMAL_PRIORITY_CLASS;
		std::cout << "Attempting set to Below Normal priority...\n";
		break;
	case 3:
		priority = NORMAL_PRIORITY_CLASS;
		std::cout << "Attempting set to Normal priority...\n";
		break;
	case 4:
		priority = ABOVE_NORMAL_PRIORITY_CLASS;
		std::cout << "Attempting set to Above Normal Priority...\n";
		break;
	case 5:
		priority = HIGH_PRIORITY_CLASS;
		std::cout << "Attempting set to High priority...\n";
		break;
	case 6:
		priority = REALTIME_PRIORITY_CLASS;
		std::cout << "Attempting set to Realtime Priority (in spite of reported success, if process doesn't have SeIncreaseBasePriorityPrivilege, then this will likely just go to high priority values).\n";
		break;
	default:
		priority = NORMAL_PRIORITY_CLASS;
		std::cout << "Misunderstood option. Defaulting to Normal priority. Returning to main...\n";
		break;
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
	DWORD priority;
	switch (choice)
	{
	case 1:
		priority = THREAD_PRIORITY_IDLE;
		std::cout << "Attempting to set current thread to idle priority...\n";
		break;
	case 2:
		priority = THREAD_PRIORITY_LOWEST;
		std::cout << "Attempting to set current thread to lowest priority...\n";
		break;
	case 3:
		priority = THREAD_PRIORITY_BELOW_NORMAL;
		std::cout << "Attempting to set current thread to below normal priority...\n";
		break;
	case 4:
		priority = THREAD_PRIORITY_NORMAL;
		std::cout << "Attempting to set current thread to normal priority...\n";
		break;
	case 5:
		priority = THREAD_PRIORITY_ABOVE_NORMAL;
		std::cout << "Attempting to set current thread to above normal priority...\n";
		break;
	case 6:
		priority = THREAD_PRIORITY_HIGHEST;
		std::cout << "Attempting to set current thread to highest priority...\n";
		break;
	case 7:
		priority = THREAD_PRIORITY_TIME_CRITICAL;
		std::cout << "Attempting to set current thread to time critical priority...\n";
		break;
	default:
		priority = THREAD_PRIORITY_NORMAL;
		std::cout << "Misunderstood option. Defaulting to normal priority thread...\n";
		break;
	}
	BOOL threadSuccess = SetThreadPriority(hThread, priority);
	if (threadSuccess)
	{
		std::cout << "Successfully set thread priority! Check Process Explorer!\n";
		std::cout << "Thread Priority is currently: " << GetThreadPriority(hThread) << '\n';
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
		int choice;
		std::cout << "What do you want to do?\n1) Change Process Priority\n2) Change Thread Priority\n3) Initiate a long running loop to track thread state changes\n";
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