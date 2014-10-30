#ifndef Polygroup_H
#define Polygroup_H

#include <Outland\UnitAbstraction.h>
#include "Polygon.h"

class Polygroup
{
public:
	Polygroup();
	void addPoly(const Poly& poly);
	void removePoly(const Poly& poly);
	void polygonReduction();
	void renderGroup();
	void clearPolygroup();
	void toggleSetup() {initialSetup=!initialSetup;};
	void setRender(bool high) {highPolyReady=high;};
	bool isHighPolyReady() const {return highPolyReady;};
	std::vector<Poly> getHighPolys() const {return highEfficientPolys;};
	std::vector<Poly> getLowPolys() const {return lowEfficientPolys;};
private:
	std::vector<Poly> grid[10][10];
	void clearGrid();
	std::vector<Poly> lowEfficientPolys;
	std::vector<Poly> highEfficientPolys;
	bool highPolyReady;
	bool initialSetup;
};

#endif