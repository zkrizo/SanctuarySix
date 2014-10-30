#include "Bitmap.h"
#include <Outland\UnitAbstraction.h>
#include <cstdio>
#include <Windows.h>

Bitmap::Bitmap()
{
	loaded=FALSE;
	pixeldata=NULL;
	width=0;
	height=0;
	bpp=0;
}

Bitmap::Bitmap(t8* filename)
{
	loaded=loadBMPImage(filename);
}

Bitmap::~Bitmap()
{
	if(pixeldata!=NULL)
	{
		delete [] pixeldata;
	}
}

tBOOL Bitmap::loadBMPImage(t8* filename)
{
	DWORD imagesize;
	DWORD difference;
	u64 offset;
	LONG byteWidth;				//width of actual image in bytes
	LONG padWidth;				//width of image + the padding
	tBYTE *tempPixel;

	//declare pointer to file stream
	FILE *instream;

	//open it for reading in binary mode
	fopen_s(&instream,filename,"rb");

	if(instream)
	{
		//read file header
		fread(&bmfh,sizeof(BITMAPFILEHEADER),1,instream);

		//checks to make sure the file is a bitmap file
		if(bmfh.bfType!=19778)
		{
			fclose(instream);
			return FALSE;
		}

		//read info header
		fread(&bmih,sizeof(BITMAPINFOHEADER),1,instream);

		//calculate the size of the image
		imagesize=bmfh.bfSize-bmfh.bfOffBits;

		height=bmih.biHeight;

		if(imagesize!=0)
		{
			//create and read the pixel data
			tempPixel=new BYTE[imagesize];
			fread(tempPixel,sizeof(BYTE),imagesize,instream);
		}
		else //file is empty after the header information
		{
			fclose(instream);
			return FALSE;
		}

		width=bmih.biWidth;
		bpp=bmih.biBitCount;
		//sets bytewidth and padwidth to the actual image size
		byteWidth=padWidth=(t64)((real32)width*(real32)bpp/8.0);
	
		//set the difference to non padded image size
		difference=height*byteWidth;

		//sets up the variable for the 
		pixeldata=new BYTE[difference];

		//increases the padwidth to account for any padding
		while(padWidth%4!=0){
			padWidth++;
		}

		if(height<0)
		{
			s32 j=imagesize-4;
			offset=padWidth-byteWidth;
			for(u32 i=0;i<imagesize;i+=4)
			{
				if((i+1)%padWidth==0)
				{
					i+=offset;
				}
				*(pixeldata+j+3)=*(tempPixel+i);
				*(pixeldata+j+2)=*(tempPixel+i+1);
				*(pixeldata+j+1)=*(tempPixel+i+2);
				*(pixeldata+j)=*(tempPixel+i+3);
				j-=4;
			}
		}
		else		//height is not reversed
		{
			//height=height*-1;
			offset=0;
			do{
				memcpy((pixeldata+(offset*byteWidth)),(tempPixel+(offset*padWidth)),byteWidth);
				offset++;
			}while(offset<height);
		}
	
		loaded=TRUE;

		//closes file
		fclose(instream);

	
		return TRUE;
	}
	else
		return FALSE;
}