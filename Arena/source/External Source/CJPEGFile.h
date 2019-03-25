#ifndef _H_CJPEGFILE
#define _H_CJPEGFILE


//////////////////////////////////////
//The CJPEGFile Class
//////////////////////////////////////
class CJPEGFile
{
	public:
		void Load(char *filename, int bitsperpixel, void *where);
		void GetSize(char *filename, unsigned long &width, unsigned long &height);
};


#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com