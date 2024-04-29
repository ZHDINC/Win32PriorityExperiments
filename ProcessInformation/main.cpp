#include<iostream>
#include<Windows.h>

int main()
{
	HANDLE hProcess = GetCurrentProcess();
	BOOL success = SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
	if (success) 
	{
		std::cout << "Successfully set priority class! Check Process Explorer!\n";
	}
	else
	{
		std::cout << "Failed to set priority class! Check last error!\n";
		std::cout << GetLastError();
	}
	HANDLE hThread = GetCurrentThread();
	BOOL threadSuccess = SetThreadPriority(hThread, -2);
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
	char beginLoop;
	std::cout << "Awaiting performance monitor ready...";
	std::cin >> beginLoop;
	for (int i = 0; i < 10000000; i++)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
	char stopBackgroundMode;
	if (!success)
	{
		std::cout << "Stop background mode?\n";
		std::cin >> stopBackgroundMode;
		SetPriorityClass(hProcess, PROCESS_MODE_BACKGROUND_END);
	}
	char stopExecution;
	std::cout << "Press any key to stop execution...\n";
	std::cin >> stopExecution;
}