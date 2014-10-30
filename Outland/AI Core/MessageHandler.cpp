#include "MessageHandler.h"


/****************************
* Constructor
****************************/
MessageHandler::MessageHandler()
{

}

/****************************
* Destructor
****************************/
MessageHandler::~MessageHandler()
{

}

/****************************
* Destructor
****************************/
void MessageHandler::Update()
{

}

MessageHandler* MessageHandler::Instance()
{
	static MessageHandler instance;
	return &instance;
}