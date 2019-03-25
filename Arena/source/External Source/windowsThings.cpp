#include "windowsThings.h"
#include "textureSingleton.h"
#include "HCI.h"

typedef unsigned char uchar;


GLuint emptyTexture()					// Create An Empty Texture
{
	GLuint txtnumber;						// Texture ID
	unsigned int* data;						// Stored Data

	// Create Storage Space For Texture Data (128x128x4)
	data = (unsigned int*)new GLuint[((256 * 256)* 4 * sizeof(unsigned int))];

	ZeroMemory(data,((256 * 256)* 4 * sizeof(unsigned int)));	// Clear Storage Memory

	glGenTextures(1, &txtnumber);					// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, txtnumber);			// Bind The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);			// Build Texture Using Information In data
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	delete [] data;							// Release data

	return txtnumber;						// Return The Texture ID
}


void FillOpenParams(OPENFILENAME &open_params, HWND hwnd, char *filter, char *file_name)
{
	/** Initialize the "open_params" fields we need to for opening a .bmp file **/

	open_params.lStructSize = sizeof(OPENFILENAME); // The size of the struct
	open_params.hwndOwner = hwnd; // The "window owner" of this dialog box
	open_params.lpstrFilter = filter;
	
	open_params.lpstrFile = file_name; // This is a pointer to a filename used
								      // to initialize the dialog box -- Since our
								     // "file_name" equals NULL, No "default filename"
							        // will be used
				
	open_params.nMaxFile = BUFF_MAX; // The size of the "file_name" buffer
				
	open_params.lpstrInitialDir = NULL; // Now this already equals NULL, but I'm setting
									   // it again just so I can comment it -- This means
									  // (for Windows NT 5.0/Windows 98 and later)
									 // that if the "current directory" contains any files of 
									// the specified filter type(s) this directory will be the 
								   // initial directory.  Otherwise it will be the "personal
								  // files directory" for whoever's running the program 

	open_params.lpstrFileTitle = NULL; // Again it's already NULL but what this is, is the 
									  // title of the dialog box -- Since we're setting it
									 // to NULL it will use the default title ("Open" in 
									// our case)

	// As you can imagine, there is a plethora of flags that can be set for the dialog box
	// I'll break down what these four mean
	// OFN_FILEMUSTEXIST -- Only allows the typing of existing files in the File Name 
	//						entry field
	// OFN_PATHMUSTEXIST -- Allows the user to ONLY type valid paths and filenames
	// OFN_NOCHANGEDIR -- Changes the current directory back to it's original value
	//					  if the user changes the directory while searching for files
	// OFN_HIDEREADONLY -- This hides the "read only" check box (the ability to open files
	//					   as "read only" is possible)					
	open_params.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
						| OFN_HIDEREADONLY;

	// Okay we've filled what we wanted to
}

int openEnvImage()
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	HWND hwnd = FindWindow("GLUT", "Collision Madness");

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box

				
	strcat(filter,"Bitmap Texture"); // Tack on "BMP Files" to filter

		
	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';
	filter[index++] = 'b';
	filter[index++] = 'm';
	filter[index++] = 'p';
				
	FillOpenParams(open_params,hwnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetOpenFileName(&open_params))
	{
		//return CTextureMan::getInstance()->loadEnvironment(open_params.lpstrFile);
		return 0;
	}

	return NULL;
}


// The WinProc
int openFileAlj()
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	HWND hwnd = FindWindow("GLUT", "Collision Madness");

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box

				
	strcat(filter,"3ds Mesh"); // Tack on "BMP Files" to filter

		
	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';
	filter[index++] = '3';
	filter[index++] = 'd';
	filter[index++] = 's';
				
	FillOpenParams(open_params,hwnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetOpenFileName(&open_params))
	{
		//return CModelManager::getInstance()->open(open_params.lpstrFile);
	}

	return -1;
}

char* openMap()
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	HWND hwnd = FindWindow("GLUT", "Collision Madness");

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box

				
	strcat(filter,"Penguin Map"); // Tack on "BMP Files" to filter

		
	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';
	filter[index++] = 'a';
	filter[index++] = 'd';
	filter[index++] = 'm';
				
	FillOpenParams(open_params,hwnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetOpenFileName(&open_params))
	{
		char hello[1024];
		strcpy(hello,open_params.lpstrFile);

		return hello;
	}

	return NULL;
}

char* saveMap()
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	HWND hwnd = FindWindow("GLUT", "level builder");

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box

				
	strcat(filter,"Penguin Map"); // Tack on "BMP Files" to filter

		
	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';
	filter[index++] = 'a';
	filter[index++] = 'd';
	filter[index++] = 'm';
				
	FillOpenParams(open_params,hwnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetSaveFileName(&open_params))
	{
		char hello[1024];
		strcpy(hello,open_params.lpstrFile);

		return hello;
	}

	return NULL;
}




void OrthoMode(int left, int top, int right, int bottom)
{
	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						

	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								

	//Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, bottom, top, 0, 1 );	
	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								

	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();										
}


///////////////////////////////// PERSPECTIVE MODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function changes our returns our projection mode from 2D to 3D
/////
///////////////////////////////// PERSPECTIVE MODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void PerspectiveMode()										// Set Up A Perspective View
{
	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											

	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );							

	// We should be in the normal 3D perspective mode now
}

void translateWidget(int activeAxis){

	if(activeAxis > 2) activeAxis -= 3;
	
	glDisable(GL_LIGHTING);

	glLineWidth(3);

	if(activeAxis < 3)
	{

		glColor3f(1,0,0);

		if(activeAxis == 0) glColor3f(1,1,1);

		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(50,0,0);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(56,-2,0);
			glVertex3f(52,2,0);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(52,-2,0);
			glVertex3f(56,2,0);
		glEnd();

		glColor3f(0,1,0);

		if(activeAxis == 1) glColor3f(1,1,1);

		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,50,0);
		glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(0,52,0);
			glVertex3f(0,55,0);
			glVertex3f(0,58,2);
			glVertex3f(0,55,0);
			glVertex3f(0,58,-2);
		glEnd();

		glColor3f(0,0,1);

		if(activeAxis == 2) glColor3f(1,1,1);

		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,50);
		glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(0,-2,52);
			glVertex3f(0,-2,55);
			glVertex3f(0,2,52);
			glVertex3f(0,2,55);
		glEnd();

	}
	else
	{
		glLineWidth(1);

		int radius = 20;

		glColor3f(0,1,0);

		glBegin(GL_LINE_STRIP);
 
		for (int i=0; i < 360; i++)
		{
			float radians = i*180/PI;

			glVertex3f(cos(radians)*radius,0,sin(radians)*radius);
		}
 
		 glEnd();
	}

}

SVector3 get3dCursorPos(float x, float y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return SVector3(posX, posY, posZ);
}

char* parseString(const char* filename, const char *directory){

	char file[1024];

	int charPos = 0;
	int len = strlen(filename);

	for(int i = len -1; i >= 0; i--)
	{
		if((filename[i] == '/') || (filename[i] == '\\'))
		{
			file[charPos] = NULL;

			char flip[1024];
			int flipPos = 0;
			
			for(int i = charPos -1; i >= 0; i--)
			{
				flip[flipPos++] = file[i];
			}

			flip[flipPos] = NULL;

			char final[1024];

			strcpy(final,directory);
			strcat(final,flip);

			return final;
		}
		else
		{
			file[charPos++] = filename[i];
		}
	}

	return "fail";

}

void billboardQuad(){

		glEnable(GL_TEXTURE_2D);
		//CTextureMan::getInstance()->bindIcon(0);

		glPushMatrix();
			
		billboardSphericalBegin();

			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_BLEND);

				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex2f(-2500,-2500);
					glTexCoord2f(0,1); glVertex2f(-2500,2500);
					glTexCoord2f(1,1); glVertex2f(2500,2500);
					glTexCoord2f(1,0); glVertex2f(2500,-2500);
				glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

		billboardEnd();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

}

void billboardSprite(){

		glPushMatrix();
	
		billboardSphericalBegin();

			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex2f(-2000,-2000);
					glTexCoord2f(0,1); glVertex2f(-2000,2000);
					glTexCoord2f(1,1); glVertex2f(2000,2000);
					glTexCoord2f(1,0); glVertex2f(2000,-2000);
				glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

		billboardEnd();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

}

void blackQuad(){

		/*OrthoMode(0, 0, 1024, 768);

			glDisable(GL_LIGHTING);
			glDisable(GL_CULL_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

				glBegin(GL_QUADS);
					glVertex2f(-5000,-5000);
					glVertex2f(-5000,5000);
					glVertex2f(5000,5000);
					glVertex2f(5000,-5000);
				glEnd();

			glDisable(GL_BLEND);
			glEnable(GL_LIGHTING);
			glEnable(GL_CULL_FACE);
		
		PerspectiveMode();*/

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(0,0);
			glTexCoord2f(0,1); glVertex2f(0,-1);
			glTexCoord2f(1,1); glVertex2f(1,-1);
			glTexCoord2f(1,0); glVertex2f(1,0);
	glEnd();

}

void texturedQuad()
{
	/*glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
			glTexCoord2f(-1,-1); glVertex2f(0,0);
			glTexCoord2f(-1,1); glVertex2f(0,-1);
			glTexCoord2f(1,1); glVertex2f(1,-1);
			glTexCoord2f(1,-1); glVertex2f(1,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);*/

	glEnable(GL_TEXTURE_2D);

	glNormal3f(0,0,1);

	glBegin(GL_QUADS);
			glColor3f(0.5,0.5,0.5);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor4f(0,0,0,1);
			//glColor3f(1.0f, 0.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,-1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1,1);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 0.0f, 1.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(1,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,1); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
			glNormal3f(0.0f, 0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);
			//glColor4f(1,1,1,1);
			glVertex2f(-1,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void texturedQuadMasked(SVector3 center, float size)
{
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
			glTexCoord2f(center.x - size,center.y - size); glVertex2f(0,0);
			glTexCoord2f(center.x - size,center.y + size); glVertex2f(0,-1);
			glTexCoord2f(center.x + size,center.y + size); glVertex2f(1,-1);
			glTexCoord2f(center.x + size,center.y - size); glVertex2f(1,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void texturedCircleMasked(SVector3 center, float size, int sides)
{
	glEnable(GL_TEXTURE_2D);

	/*glBegin(GL_QUADS);
			glTexCoord2f(center.x - size,center.y - size); glVertex2f(0,0);
			glTexCoord2f(center.x - size,center.y + size); glVertex2f(0,-1);
			glTexCoord2f(center.x + size,center.y + size); glVertex2f(1,-1);
			glTexCoord2f(center.x + size,center.y - size); glVertex2f(1,0);
	glEnd();*/

	float twoPi = 2.0f * 3.14159f;

	glBegin(GL_TRIANGLE_FAN);

	for(int i = 0; i < sides; i++)
	{
		float rot = i * (360 / sides) + 90;
		float tcx,tcy;

		float rotrad;
		rotrad = (rot / 180 * 3.141592654f);

		tcx = center.x + float(sin(rotrad)) * 0.1;
		tcy = center.y - float(cos(rotrad)) * 0.1;

		glTexCoord2f(tcx,tcy);

        glVertex2f(size * cos(i *  twoPi / sides), 
                     size * sin(i * twoPi / sides));
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void translateWidgetNew(SVector3 center, SVector3 selected)
{
	SVector3 worldSpacePos;
	worldSpacePos.x = user::getInstance()->input.xpos;
	worldSpacePos.y = user::getInstance()->input.ypos;
	worldSpacePos.z = user::getInstance()->input.zpos;

	float scale = distance(center,worldSpacePos) / 100;

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		//x-axis
		glPushMatrix();
			if(selected.x == 1) glColor3f(1,0.7,0);
			else glColor3f(0.5,0,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
			glEnd();
		glPopMatrix();
		//y axis
		glPushMatrix();
			if(selected.y == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0.5,0);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
			glEnd();
		glPopMatrix();
		//z axis
		glPushMatrix();
			if(selected.z == 1) glColor3f(1,0.7,0);
			else glColor3f(0,0,0.5);
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
			glEnd();
		glPopMatrix();

	glPopMatrix();

}





