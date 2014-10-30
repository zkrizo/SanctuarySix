#ifndef MessageHandler_H
#define MessageHandler_H

#include <Outland\UnitAbstraction.h>

class MessageHandler
{
public:
	int getID() const{return ID;}
	static MessageHandler* Instance();
	void Update();
private:
	int ID;

	MessageHandler(const MessageHandler&);
	MessageHandler& operator=(const MessageHandler&);
	MessageHandler();
	~MessageHandler();


};

#endif