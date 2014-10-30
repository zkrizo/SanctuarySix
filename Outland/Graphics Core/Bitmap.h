#ifndef Bitmap_H
#define Bitmap_H

#include <Windows.h>
#include <Outland\UnitAbstraction.h>

class Bitmap
{
public:
	tBYTE *pixeldata;
	tBOOL loaded;
	u64 width,height;
	u16 bpp;

	Bitmap();
	Bitmap(t8 *file);
	~Bitmap();
	tBOOL loadBMPImage(t8 *file);
//private:
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

};

#endif