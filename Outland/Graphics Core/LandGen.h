#ifndef LandGen_H
#define LandGen_H

//Disable max warning level for outside library
#pragma warning(push,3)
#include <libnoise\noise.h>
#pragma warning(pop)

#include <Outland\UnitAbstraction.h>

using namespace noise;

class LandGen
{
public:
	void setup(s32 type);

	real32 genValue(s32 x,s32 z, s32 seed);
private:
	module::Perlin noise;
	module::RidgedMulti softMountain;
	module::RidgedMulti sharpMountain;
	module::Billow baseFlat;
	module::ScaleBias plains;
	module::Select final;
};

#endif