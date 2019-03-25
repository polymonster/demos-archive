#ifndef OUTPUT_H
#define OUTPUT_H

#include "globals.h"

class COutput {

public :

	COutput();
	void output(float x,float y,const char *name, const char *string);
	void output(float x,float y, const char *name,const char *string,int font);
	void output(float x,float y,const char *name, int variable);
	void output(float x,float y,const char *name, int variable,int font);
	void output(float x,float y,const char *name, float variable);
	void output(float x,float y,const char *name, double variable);

	void bigNums(float x,float y,int variable,unsigned int bitmap);
	void bigNums(float x,float y,int variable,SVector2 dimemsions);
	void bigNums(float x,float y,int variable,SVector2 dimemsions,bool center);

	void bigFont(float x,float y,const char *text,unsigned int bitmap);

private :

	void out(float x,float y,const char *string);
	void fontSelect(int font);

	void *activeFont;
	void *font10;
	void *font12;
	void *font18;

	unsigned int scoreNumbers;

};

#endif