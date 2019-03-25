#ifndef common_h__
#define common_h__

//commomn includes that will be required in most parts of the engine

#include <GL\glee.h>
#include <GL\gl3.h>
#include <GL\glu.h>

#include "console.h"
#include "polyspoon_math.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifdef _ENGINE

#ifdef _RELEASE_DIST

//distribution paths
	#define SHADER_PATH "Data\\shaders\\"
	#define TEXTURE_PATH "Data\\textures\\"
	#define MODEL_PATH "Data\\models\\"
	#define LIB_PATH "Libs\\"

#else

	//engine paths
	#define SHADER_PATH "..\\..\\Data\\shaders\\"
	#define TEXTURE_PATH "..\\..\\Data\\textures\\"
	#define MODEL_PATH "..\\..\\Data\\models\\"
	#define LIB_PATH "..\\..\\Libs\\"

#endif

#else

//editor paths

#define SHADER_PATH "..\\..\\..\\..\\..\\Data\\shaders\\"
#define TEXTURE_PATH "..\\..\\..\\..\\..\\Data\\textures\\"
#define MODEL_PATH "..\\..\\..\\..\\..\\Data\\models\\"
#define LIB_PATH "..\\..\\..\\..\\..\\Libs\\"

#endif


#endif // common_h__