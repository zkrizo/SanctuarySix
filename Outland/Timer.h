#ifndef Timer_H
#define Timer_H

#include "UnitAbstraction.h"
#include <Windows.h>
#include <queue>


class Timer
{
public:
	Timer();
	~Timer();
	void update();
	real32 getFPS();
	real32 getAvgFPS();
	real32 getMSPF();
	LARGE_INTEGER getStart();
	LARGE_INTEGER getEnd();
	LARGE_INTEGER getFreq();
	real32 getTime();

private:
	LARGE_INTEGER startFrame;
	LARGE_INTEGER endFrame;
	LARGE_INTEGER freq;
	LARGE_INTEGER absoluteStart;
	real32 FPS;
	real32 avgFPS;
	std::queue<real32> avgFPSdata;

	void calcFPS();
	void calcAvgFPS();
};

#endif