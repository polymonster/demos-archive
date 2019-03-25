#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <windows.h>
#include <windowsx.h>

#include "GLee/GLee.h"
#include "glut.h"
#include <gl/glu.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <math.h>

#include "myMath.h"

#pragma warning( disable : 4786 )
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include <gl\glaux.h>
#include <fstream>
#include <vector>									
#include <crtdbg.h>
#include <iostream>
#include <tchar.h> 
#include <wchar.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string>

#include <cal3d.h>

#define MAXTEXTURE 100

using namespace std;

//The tVector3 Struct
//////////////////////////////////////
typedef struct tVector3					// expanded 3D vector struct
{			
	tVector3() {}	// constructor
	tVector3 (float new_x, float new_y, float new_z) // initialize constructor	 
	{x = new_x; y = new_y; z = new_z;}
	// overload + operator so that we easier can add vectors
	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	// overload - operator that we easier can subtract vectors
	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	// overload * operator that we easier can multiply by scalars
	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
	// overload / operator that we easier can divide by a scalar
	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}
	
	float x, y, z;						// 3D vector coordinates
}tVector3;

//////////////////////////////////////
//The tVector2 Struct
//////////////////////////////////////											
struct tVector2 
{
	float x, y;							// 2D vector coordinates
};



//////////////////////////////////////
//The Global Variables
//////////////////////////////////////
extern	HDC			hDC;			// Device Context
extern	HGLRC		hRC;			// Permanent Rendering Context
extern	HWND		hWnd;			// Holds Our Window Handle
extern	HINSTANCE	hInstance;		// Holds The Instance Of The Application

extern int xScreenSize, yScreenSize;
extern float aspectRatio;

extern int serverSocket;
extern int clientSocket;

extern unsigned int glslDecal;
extern int glslScale;

typedef struct{
//mouse
float mousex, mousey;
bool leftMouse, rightMouse, middleMouse;
float increment;
//keyb
bool key[256];
//position
float xpos,ypos,zpos;
float xrot,yrot,zrot;
float newxpos, newypos, newzpos;
float incx, incy;
float lastx, lasty;
bool wheelUp;
bool wheelDown;
bool specialKey[10];
float cursorx,cursory,cursorz;
float interfaceMousex, interfaceMousey; 
float mousemovex, mousemovey;
int screenWidth, screenHeight;
}SControls;


typedef enum{

	TRANSLATE_X = 0,
	TRANSLATE_Y,
	TRANSLATE_Z,
	SCALE_X,
	SCALE_Y,
	SCALE_Z,
	ROTATE,
	ROTATE_Y,
	ROTATE_X,
	PROPERTIES,
	CLONE,
	HIDDEN,
	DELETEB,
	SOLIDMODE,
	HOLLOWMODE,
	SLIDE,
	CUBE,
	RAMP,
	FLOOR,
	WATER,
	POPSCENERY,
	SKYBOX,
	BUILD_BOX,
	BOUNDING,
	ROAM,
	SPAWN,
	IDLE,
	PATH,
	NEXT_PATH,
	PATH_START,
	SELECT,


}eTransforms;

typedef enum{

	ENVIRONMENT = 0,
	ENVIRONMENT2,
	TERRAIN,
	UNDERWATER,
	LIGHTMAP,
	BLOCKING,
	OFFSETTING,
	ALPHABLEND,

}eScenery;

typedef enum{
	
	PLAYERCAM = 0,
	SHOPCAM,

}eCameras;

//for the player md3 model
class CVector3 
{
public:
	float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
	float x, y;
};

// This is our face structure.  This is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them.
struct tMaterialInfo
{
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	BYTE  color[3];				// The color of the object (R, G, B)
	int   texureId;				// the texture ID
	float uTile;				// u tiling of texture  
	float vTile;				// v tiling of texture	
	float uOffset;			    // u offset of texture
	float vOffset;				// v offset of texture
} ;


// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	CVector3  *pVerts;			// The object's vertices
	CVector3  *pNormals;		// The object's normals
	CVector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
	
};


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This holds our information for each animation of the Quake model.
// A STL vector list of this structure is created in our t3DModel structure below.
struct tAnimationInfo
{
	char strName[255];			// This stores the name of the animation (I.E. "TORSO_STAND")
	int startFrame;				// This stores the first frame number for this animation
	int endFrame;				// This stores the last frame number for this animation
	int loopingFrames;			// This stores the looping frames for this animation (not used)
	int framesPerSecond;		// This stores the frames per second that this animation runs
};

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


// This our model structure
struct t3DModel 
{
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	vector<tMaterialInfo> pMaterials;	// The list of material information (Textures and colors)
	vector<t3DObject> pObject;			// The object list for our model
	

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	int numOfAnimations;				// The number of animations in this model 
	int currentAnim;					// The current index into pAnimations list 
	int nextAnim;						// The current index into pAnimations list 
	int currentFrame;					// The current frame of the current animation 
	int nextFrame;						// The next frame of animation to interpolate too
	float t;							// The ratio of 0.0f to 1.0f between each key frame
	float lastTime;						// This stores the last time that was stored
	vector<tAnimationInfo> pAnimations; // The list of animations

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	int numOfTags;						// This stores the number of tags in the model
	t3DModel			**pLinks;		// This stores a list of pointers that are linked to this model
	struct tMd3Tag		*pTags;			// This stores all the tags for the model animations
	int muzzleId;
};

#endif