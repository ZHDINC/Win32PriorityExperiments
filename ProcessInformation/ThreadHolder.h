#pragma once
#include<Windows.h>

class ThreadHolder
{
	HANDLE hThread = INVALID_HANDLE_VALUE;
public:
	ThreadHolder(HANDLE hThread) : hThread{ hThread } { };
	ThreadHolder(const ThreadHolder& other) = delete; // I would be fine with copying assuming there was a way to create a new handle from the existing one. Move constructor does what I need it to. 
	ThreadHolder& operator=(ThreadHolder& other) = delete;
	ThreadHolder(ThreadHolder&& other)
	{
		this->hThread = other.hThread;
		other.hThread = INVALID_HANDLE_VALUE;
	}
	~ThreadHolder()
	{
		if (hThread != INVALID_HANDLE_VALUE)
			CloseHandle(hThread);
	}
	HANDLE GetThread() const
	{
		return hThread;
	}
};