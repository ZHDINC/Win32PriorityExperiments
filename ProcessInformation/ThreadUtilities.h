#pragma once
#include<map>
#include<string>
#include<iostream>
#include<Windows.h>

const std::map<DWORD, std::string> ThPriority = {
	{THREAD_PRIORITY_IDLE, "Idle Priority"},
	{THREAD_PRIORITY_LOWEST, "Lowest Priority"},
	{THREAD_PRIORITY_BELOW_NORMAL, "Below Normal Priority"},
	{THREAD_PRIORITY_NORMAL, "Normal Priority"},
	{THREAD_PRIORITY_ABOVE_NORMAL, "Above Normal Priority"},
	{THREAD_PRIORITY_HIGHEST, "Highest Priority"},
	{THREAD_PRIORITY_TIME_CRITICAL, "Time Critical Priority"}
};

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

void ChangeThreadBoost(HANDLE hThread)
{
	BOOL priorityBoostOld = false;
	GetThreadPriorityBoost(hThread, &priorityBoostOld);
	std::string priorityBoostOldString = priorityBoostOld ? "Dynamic priority boost is disabled" : "Dynamic priority boost is enabled";
	std::cout << "Thread priority boost is currently: " << priorityBoostOldString << ".\n1) Turn Off Dynamic Priority Boost\n2) Turn On Dynamic Priority Boost.\n";
	int choice;
	std::cin >> choice;
	bool priorityBoostChoice = choice == 1 ? true : false;
	SetThreadPriorityBoost(hThread, priorityBoostChoice);
}

void ChangeThreadPriority(HANDLE hThread)
{
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
}