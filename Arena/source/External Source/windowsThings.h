#ifndef _WINDOWS_THINGS_H
#define _WINDOWS_THINGS_H

#include "globals.h"
//#include "3dsLoader.h"

#define BUFF_MAX 256 // The size of the buffers that have to do with opening a file
#define DLG_BOX                         103
#define IDC_CHECK1                      1000
#define IDC_CHECK2                      1001
#define IDC_CHECK3                      1002
#define IDC_BUTTON1                     1003
#define IDD_PASSWORD                    101
#define IDC_PASSWORD                    1000
#define IDC_STATIC                      -1

// Next default values for new objects
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1007
#define _APS_NEXT_SYMED_VALUE           101

void FillOpenParams(OPENFILENAME &open_params, HWND hwnd, char *filter, char *file_name);
int openEnvImage();
int openFileAlj();
char* openMap();
char* saveMap();
void lightBox();
void OrthoMode(int left, int top, int right, int bottom);
void PerspectiveMode();
void translateWidget(int activeAxis);
SVector3 get3dCursorPos(float x, float y);
void setShaders();
void billboardQuad();
void blackQuad();
void billboardSprite();
void texturedQuad();
void texturedQuadMasked(SVector3 center, float size);
void texturedCircleMasked(SVector3 center, float size, int sides);
void translateWidgetNew(SVector3 center, SVector3 selected);

char* parseString(const char* filename, const char *directory);

#endif