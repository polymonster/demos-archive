#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include "textFile.h"
#include "Globals.h"

typedef struct
{
	int normalMap;
	int grass;
	int terrain;
	int bloom;
	int water;
	int envMap;
	int celShader;
	int skyBox;

}SShaderId;

extern SShaderId g_shaderId;

class CShaderManager{

public:

	static void create();
	static CShaderManager* getInstance(){return m_instance;};
	static void destroy();

	int loadShader(char *shaderConfigFile);

	void detachShader();
	void attachShader(int shaderId);

	void shaderOptionHandler(WPARAM &wParam);
	void shaderCompiler(char *shaderConfigFile);

	int m_numberOfShaders;

	void setup();

	GLhandleARB getShaderProgram(int id);

private:

	CShaderManager(){m_numberOfShaders = 0;};
	~CShaderManager(){};

	static CShaderManager *m_instance;

	GLint loc;
};

#endif