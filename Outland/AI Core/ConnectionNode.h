#ifndef ConnectionNode_H
#define ConnectionNode_H

#include <Outland\Graphics Core\Polygon.h>
#include "ConnectionPath.h"

class ConnectionNode
{
public:
	ConnectionNode(const Poly& poly);
private:
	std::vector<ConnectionPath> ZM;
	std::vector<ConnectionPath> ZP;
	std::vector<ConnectionPath> XM;
	std::vector<ConnectionPath> XP;
	Poly nodePoly;	
};

#endif