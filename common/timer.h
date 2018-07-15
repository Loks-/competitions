#pragma once

#include "base.h"

class Timer
{
protected:
	bool running;
	chrono::time_point<chrono::system_clock> start_time, end_time;

public:
	void Start() { start_time = chrono::system_clock::now(); running = true; }
	void Stop() { end_time = chrono::system_clock::now(); running = false; }
	
	Timer(bool start = true) : running(false)
	{
		if (start) Start();
	}

	size_t GetMilliseconds() const
	{
		chrono::time_point<chrono::system_clock> time = running ? chrono::system_clock::now() : end_time;
		return chrono::duration_cast<chrono::milliseconds>(time - start_time).count();
	}
};
