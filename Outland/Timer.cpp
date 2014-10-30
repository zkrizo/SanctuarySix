#include "Timer.h"

/****************************
* Default Constructor
****************************/
Timer::Timer():
	FPS(0),
	avgFPS(0)
{
	startFrame.QuadPart=0;
	if(!QueryPerformanceFrequency(&freq))
		freq.QuadPart=0;	//freq INVALID

	QueryPerformanceCounter(&absoluteStart);
}

/****************************
* Deconstructor
****************************/
Timer::~Timer()
{
	while(!avgFPSdata.empty())
	{
		avgFPSdata.pop();
	}
}

/****************************
* update - This function is called on the beginning of the game
			loop to determine the values of time
****************************/
void Timer::update()
{
	if(startFrame.QuadPart==0)		//initial state
	{
		QueryPerformanceCounter(&startFrame);
		endFrame=startFrame;
	}
	else
	{
		startFrame=endFrame;
		QueryPerformanceCounter(&endFrame);
		
		//calculates the FPS values
		calcFPS();
		calcAvgFPS();
	}

}

/****************************
* getFPS - Returns the frames per second
****************************/
real32 Timer::getFPS()
{
	return FPS;
}

/****************************
* getAvgFPS - Returns the average frames per second
				for the last 30 frames
****************************/
real32 Timer::getAvgFPS()
{
	return avgFPS;
}

/****************************
* getMSPF -	Returns the number of milliseconds the last
			frame took
****************************/
real32 Timer::getMSPF()
{
	if(FPS>0)		//sanity check
	{
		return 1000/FPS;
	}
	else
		return 0;
}

/****************************
* getStart - Returns the time value of the beginning of
			the last frame
****************************/
LARGE_INTEGER Timer::getStart()
{
	return startFrame;
}

/****************************
* getEnd -	Returns the time value of the beginning of
			the current frame
****************************/
LARGE_INTEGER Timer::getEnd()
{
	return endFrame;
}

/****************************
* getFreq - Returns the frequency of the cpu clock
****************************/
LARGE_INTEGER Timer::getFreq()
{
	return freq;
}

/****************************
* calcFPS -	Private function used to calculate the frames per second
			for the last frame
****************************/
void Timer::calcFPS()
{
	FPS=(endFrame.QuadPart - startFrame.QuadPart)/ (freq.QuadPart/1000);
}

/****************************
* calcAvgFPS -	Private function used to calculate the average FPS
				for the last 30 frames
****************************/
void Timer::calcAvgFPS()
{
	real32 temp=0;
	std::queue<real32> temp2;


	//adds the FPS data to the avg data variable - capped at last 30 frames
	if(avgFPSdata.size()<30)
	{
		avgFPSdata.push(FPS);
	}
	else
	{
		avgFPSdata.pop();
		avgFPSdata.push(FPS);
	}
	temp2=avgFPSdata;
	//totals the FPS data
	for(u32 i=0;i<avgFPSdata.size();i++)
	{
		temp=temp+temp2.front();
		temp2.pop();
	}

	//takes the average of the FPS data
	avgFPS=temp/avgFPSdata.size();
}

/****************************
* getTime() - Gets the absolute time since the start of the program.
****************************/
real32 Timer::getTime()
{
	if(endFrame.QuadPart-absoluteStart.QuadPart>0)
	{
		real32 temp;
		temp=endFrame.QuadPart-absoluteStart.QuadPart;
		temp=temp/(freq.QuadPart/1000);		
		//temp=1000/temp;
		return temp;
	}
	else
		return 0;
}