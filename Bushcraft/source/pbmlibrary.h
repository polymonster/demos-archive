/*	This is a header file containing Input Output Procedures used for 
	detecting edge based on estimating	the distance between an edge.

	Start date :		Wed, 27-10-1999
	Last Modified date:	Wed, 27-10-1999
	Filename:			PBMLibrary.h
	Copyright:			Sudirman 1999

*/

#ifndef INC_PBMLIBRARY_H
#define INC_PBMLIBRARY_H


unsigned char* ReadPBM(const char* path,int* width, int* height,bool* color,char* msg);
bool WritePPM(const char* path,unsigned char* image, int width, int height, char* msg);
bool WritePGM(const char* path,unsigned char* image, int width, int height, char* msg);

#endif /* INC_PBMLIBRARY_H */