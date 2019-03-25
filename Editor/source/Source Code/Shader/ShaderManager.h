#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include "textFile.h"
#include "globals.h"
#include "myMath.h"

typedef struct
{
	int normalMap;
	int grass;
	int terrain;
	int bloom;
	int water;
	int envMap;
	int skybox;
	int skydome;
	int brightPass;

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

	void setUniform(int shaderID, char* name, int value);
	void setUniform(int shaderID, char* name, float value);
	void setUniform(int shaderID, char* name, float value1, float value2);
	void setUniform(int shaderID, char* name, SVector3 value);
	void setUniform(int shaderID, char* name, SVector3 value, float w);
	void setUniform(int shaderID, char* name, float *matrix);

	void cleanUp();
	void deleteShader(int id);

private:

	CShaderManager(){m_numberOfShaders = 0;};
	~CShaderManager(){};

	static CShaderManager *m_instance;

	int m_activeShader;

	GLint loc;
};

#endif