#ifndef Logger_H
#define Logger_H

#include "UnitAbstraction.h"
#include <Windows.h>


class Logger
{
public:
	static Logger* Instance();
	void LogMessage(const int& messageLevel, const std::string& message) const;
	void setLevel(const int& level){debugLevel=level;};
private:
	Logger();
	~Logger();
	Logger(const Logger&);
	Logger operator=(const Logger&);
	int debugLevel;
};

#endif