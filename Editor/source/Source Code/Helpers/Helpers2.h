#ifndef _HELPERS_H
#define _HELPERS_H

#include "Globals.h"
#include "myMath.h"
#include "FBO.h"
//WIDGET HELPERS
SVector3 translateWidgetDepthTest(SVector3 center);
void translateWidget(SVector3 center, SVector3 selected);
void rotateWidget(SVector3 center, SVector3 selected);
SVector3 rotateWidgetDepthTest(SVector3 center);
void useScaleWidget(SVector3 *selectedAxis, SVector3 *position, SVector3 *scale);
void useTranslateWidget(SVector3 *selectedAxis, SVector3 *position);
void useRotateWidget(SVector3 *selectedAxis, SVector3 *position,SVector3 *rotation);

//OPENGL HELPERS
void cube();
void tangentQuad();
void screenSpaceQuad();
void grid(int cellSize, int dimension, int divisions);
void quad();
void texturedQuad();
void grassQuad();
void quickTranslate(SVector3 pos);
void quickScale(SVector3 scale);
void orthoMode(int left, int top, int right, int bottom);
void perspectiveMode();
void circle(float radius,int segments, bool x, bool y, bool z);
void quickClear();
void setCullMode(int mode);

//WINDOWS DIALOGUE HELPERS
#define BUFF_MAX 256 // The size of the buffers that have to do with opening a file

void initColorChooser();
void FillOpenParams(OPENFILENAME &open_params, HWND hwnd, char *filter, char *file_name);
int openFile();
char* openFile(const char *desc, const char *ext);
void polyCounter();

//VARIABLE OUTPUT HELPERS
class CConsole{

public:

	CConsole();

	void updateConsole();
	void displayConsole();

private:

	SVector2 <int> rootPos;
	bool m_consoleOpen;

};


#endif