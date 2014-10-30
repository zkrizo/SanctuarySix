#include "LandGen.h"

void LandGen::setup(s32 type)
{

	//sets the general settings for the Perlin Noise function which controls ALL of the other modules
	noise.SetOctaveCount(8);		//default
	noise.SetFrequency(0.5);		//default 1
	noise.SetPersistence(0.33);		//default 0.5

	//switch(type)
	{

	//default:		//default for the switch, generates a calm plain with a few mountains
		baseFlat.SetFrequency(2.0);
		plains.SetSourceModule(0,baseFlat);
		plains.SetScale(0.4);			//0.125		0.4
		plains.SetBias(0.5);		//-0.5			1.75

		final.SetSourceModule(0,sharpMountain);
		final.SetSourceModule(1,plains);
		final.SetControlModule(noise);

		final.SetBounds(0,100);
		final.SetEdgeFalloff(0.125);

		//break;
	}
	

}

real32 LandGen::genValue(s32 x, s32 y, s32 seed)
{
	real32 val;
	val=(real32)final.GetValue(x,y,seed);

	return val;
}