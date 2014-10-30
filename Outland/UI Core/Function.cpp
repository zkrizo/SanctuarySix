#include "Function.h"

Function::Function(int key):
	boundKey(key)
{}

int Function::getKey()
{
	return boundKey;
}

void Function::setKey(int key)
{
	boundKey=key;
}