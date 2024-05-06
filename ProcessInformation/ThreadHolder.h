#pragma once
#include<Windows.h>

class ThreadHolder
{
	HANDLE hThread = INVALID_HANDLE_VALUE;
	HANDLE hEvent = INVALID_HANDLE_VALUE;
	bool threadIsTerminated = false;
public:
	ThreadHolder(HANDLE hThread, HANDLE hEvent) : hThread{ hThread }, hEvent{ hEvent } { };
	ThreadHolder(const ThreadHolder& other) = delete; // I would be fine with copying assuming there was a way to create a new handle from the existing one. Move constructor does what I need it to. 
	ThreadHolder& operator=(ThreadHolder& other) = delete;
	ThreadHolder& operator=(ThreadHolder&&) = default;
	ThreadHolder(ThreadHolder&& other)
	{
		this->hThread = other.hThread;
		this->hEvent = other.hEvent;
		other.hThread = INVALID_HANDLE_VALUE;
		other.hEvent = INVALID_HANDLE_VALUE;
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
	HANDLE GetEvent()
	{
		return hEvent;
	}
	void ThreadFinished()
	{
		threadIsTerminated = true;
	}
	bool CanRemoveThread()
	{
		return threadIsTerminated;
	}
};