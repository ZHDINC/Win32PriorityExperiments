#pragma once 
#include<map>
#include<string>
#include<iostream>
#include<Windows.h>

const std::map<DWORD, std::string> PsPriority = {
	{IDLE_PRIORITY_CLASS, "Idle Priority"},
	{BELOW_NORMAL_PRIORITY_CLASS, "Below Normal Priority"},
	{NORMAL_PRIORITY_CLASS, "Normal Priority"},
	{ABOVE_NORMAL_PRIORITY_CLASS, "Above Normal Priority"},
	{HIGH_PRIORITY_CLASS, "High Priority"},
	{REALTIME_PRIORITY_CLASS, "Real-time Priority"}
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

void ChangeProcessBoost()
{
	HANDLE hProcess = GetCurrentProcess();
	BOOL priorityBoostOld = false;
	GetProcessPriorityBoost(hProcess, &priorityBoostOld);
	std::string priorityBoostOldString = priorityBoostOld ? "Dynamic priority boost is disabled" : "Dynamic priority boost is enabled";
	std::cout << "Process priority boost is currently: " << priorityBoostOldString << ".\n1) Turn Off Dynamic Priority Boost\n2) Turn On Dynamic Priority Boost.\n";
	int choice;
	std::cin >> choice;
	bool priorityBoostChoice = choice == 1 ? true : false;
	SetProcessPriorityBoost(hProcess, priorityBoostChoice);
}

void ChangeProcessAffinityMask()
{
	int sum = 0;
	std::cout << "On this processor, here are the possible affinity masks for individual cpus:\n";
	for (int i = 0; i < GetMaximumProcessorCount(ALL_PROCESSOR_GROUPS); i++)
	{
		if (sum == 0)
		{
			sum++;
			std::cout << sum << " ";
			continue;
		}
		sum *= 2;
		std::cout << sum << " ";
	}
	DWORD_PTR choice;
	std::cout << "\nWhich affinity mask would you like to change to?\n";
	std::cin >> choice;
	SetProcessAffinityMask(GetCurrentProcess(), choice);
}