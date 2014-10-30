#ifndef Message_H
#define Message_H

#include <Outland\UnitAbstraction.h>

class Message
{
public:
	Message();
	~Message();

	int Sender;
	int Receiver;
	int Msg;
	double DispatchTime;
	void* extraInfo;
private:


};

#endif