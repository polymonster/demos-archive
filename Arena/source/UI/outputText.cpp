#include "outputText.h"
//#include "imageLoader.h"

using namespace std;

COutput::COutput(){

	font10 = GLUT_BITMAP_TIMES_ROMAN_10;
	font12 = GLUT_BITMAP_HELVETICA_12;
	font18 = GLUT_BITMAP_HELVETICA_18;
	activeFont = NULL;

};

void COutput::out(float x,float y,const char *string)
{	
	int len, i;

	glRasterPos2f(x,y);

	len = (int) strlen(string);

	for(i = 0; i < len;i++)
	{
		glutBitmapCharacter(activeFont,string[i]);
	}

}

void COutput::output(float x,float y, const char *name,const char *string)
{
	char text[256];

	fontSelect(12);

	strcpy(text,name);
	strcat(text,string);
	out(x,y,text);

}

void COutput::output(float x,float y, const char *name,const char *string,int font)
{
	char text[256];

	fontSelect(font);

	strcpy(text,name);
	strcat(text,string);
	out(x,y,text);

}

void COutput::output(float x,float y,const char *name, int variable)
{
	char buf[256];
	char text[256];

	fontSelect(12);

	itoa(variable, buf, 10);
	strcpy(text,name);
	strcat(text,buf);
	out(x,y,text);

}

void COutput::output(float x,float y,const char *name, float variable)
{
	char buf[256];
	char text[256];

	fontSelect(12);

	itoa(variable, buf, 10);
	strcpy(text,name);
	strcat(text,buf);
	out(x,y,text);

}

void COutput::output(float x,float y,const char *name, double variable)
{
	char buf[256];
	char text[256];

	fontSelect(12);

	itoa(variable, buf, 10);
	strcpy(text,name);
	strcat(text,buf);
	out(x,y,text);

}

void COutput::output(float x,float y,const char *name, int variable,int font)
{
	char buf[256];
	char text[256];

	fontSelect(font);

	itoa(variable, buf, 10);
	strcpy(text,name);
	strcat(text,buf);
	out(x,y,text);	

}

void COutput::bigNums(float x, float y,int variable, unsigned int bitmap){

	int  digitToDisplay[3];
	char digits[3];
	
	itoa(variable,digits,10);

	int len = strlen(digits);

	for(int i = 0; i < len; i++)
	{
		digitToDisplay[i] = atoi(digits);

	}

	glPushMatrix();
	
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, bitmap);
		glTranslated(x,y,1);

	
		for(i = 0; i < variable; i++)
		{

		float pos = 0.1 * digitToDisplay[i];

		glBegin(GL_QUADS);
			glTexCoord2f(pos, 1.0f); glVertex2i(0,0);
			glTexCoord2f(pos, 0.0f); glVertex2i(0,25);
			glTexCoord2f(pos + 0.1f, 0.0f); glVertex2i(30,25);
			glTexCoord2f(pos + 0.1f , 1.0f); glVertex2i(30,0);
		glEnd();
		glTranslated(32,0,0);
		}


		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

void COutput::bigNums(float x,float y,int variable,SVector2 dimemsions)
{
	int  digitToDisplay[3];
	char digits[3];
	
	itoa(variable,digits,10);

	int len = strlen(digits);

	for(int i = 0; i < len; i++)
	{
		//convert back to decimal using ascii
		digitToDisplay[i] = digits[i] - 48;
	}

	glPushMatrix();
	
		for(i = len - 1; i >= 0; i--)
		{

			float pos = 0.1 * digitToDisplay[i];

				glBegin(GL_QUADS);
					glTexCoord2f(pos, 0.0f); glVertex2f(0,0);
					glTexCoord2f(pos, 1.0f); glVertex2f(0,dimemsions.y);
					glTexCoord2f(pos + 0.1f, 1.0f); glVertex2f(dimemsions.x,dimemsions.y);
					glTexCoord2f(pos + 0.1f , 0.0f); glVertex2f(dimemsions.x,0);
				glEnd();

				glTranslated(-dimemsions.x,0,0);
		}

	glPopMatrix();

}

void COutput::bigNums(float x,float y,int variable,SVector2 dimemsions,bool center)
{
	int  digitToDisplay[3];
	char digits[3];
	
	itoa(variable,digits,10);

	int len = strlen(digits);

	for(int i = 0; i < len; i++)
	{
		//convert back to decimal using ascii
		digitToDisplay[i] = digits[i] - 48;
	}

	glPushMatrix();
 
		float halfDim = dimemsions.x / 2;

		if(len > 1) glTranslated(halfDim * len / 2,0,0);
	
		for(i = len - 1; i >= 0; i--)
		{

			float pos = 0.1 * digitToDisplay[i];

				glBegin(GL_QUADS);
					glTexCoord2f(pos, 0.0f); glVertex2f(-halfDim,0);
					glTexCoord2f(pos, 1.0f); glVertex2f(-halfDim,dimemsions.y);
					glTexCoord2f(pos + 0.1f, 1.0f); glVertex2f(halfDim,dimemsions.y);
					glTexCoord2f(pos + 0.1f , 0.0f); glVertex2f(halfDim,0);
				glEnd();

				glTranslated(-dimemsions.x,0,0);
		}

	glPopMatrix();
}

void COutput::fontSelect(int font){

	if(font == 10) activeFont = GLUT_BITMAP_TIMES_ROMAN_10;
	if(font == 12) activeFont = GLUT_BITMAP_HELVETICA_12;
	if(font == 18) activeFont = GLUT_BITMAP_HELVETICA_18;

}


float getAscii(char letter){

	int ascii = letter - 97;

	if(ascii < 0) return -1;

	return ascii * 0.0385;

}
void COutput::bigFont(float x,float y,const char *text,unsigned int bitmap)
{
	int len, i;

	glRasterPos2f(x,y);

	len = (int) strlen(text);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bitmap);

	glTranslated(x,y,0);

	for(i = 0; i < len;i++)
	{
		float pos = getAscii(text[i]);

		if(pos != -1)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(pos,			1.0f); glVertex2i(0,0);
				glTexCoord2f(pos,			0.0f); glVertex2i(0,25);
				glTexCoord2f(pos + 0.0385f,  0.0f); glVertex2i(30,25);
				glTexCoord2f(pos + 0.0385f ,  1.0f); glVertex2i(30,0);
			glEnd();
		}

		glTranslated(32,0,0);
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

}