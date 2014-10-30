#include "Logger.h"
#include <cstdarg>
#include <fstream>
#include <iostream>

using namespace std;

Logger* Logger::Instance()
{
	static Logger instance;
	return &instance;
}

/****************************
* Default Constructor
****************************/
Logger::Logger():
	debugLevel(LOG_ALL - LOG_POLYGON)
{
	ofstream File;

	File.open("Logfile.log");

	if(File)
	{
		File << "Outland version 0.02.65\n";
		File << "Debug log:\n\n";
	}
	else
	{
		cout << "Error opening log file!\n";
	}
	File.close();
}

/****************************
* Deconstructor
****************************/
Logger::~Logger()
{

}

void Logger::LogMessage(const int& messageLevel,const std::string& message) const
{
	if((messageLevel&debugLevel)==messageLevel)
	{
		ofstream File;

		File.open("Logfile.log", fstream::out | fstream::app);

		if(File)
		{
			File << message;
		}
		File.close();
	}
}