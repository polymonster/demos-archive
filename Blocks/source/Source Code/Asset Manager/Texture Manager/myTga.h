#include "Globals.h"

// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB		 2		
#define TGA_A		 3		
#define TGA_RLE		10		

// This is our image structure for our targa data
struct tImageTGA
{
	int channels;			
	int sizeX;				
	int sizeY;				
	unsigned char *data;
};

tImageTGA *LoadMyTGA(const char *filename);