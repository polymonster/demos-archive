#include "ShaderManager.h"
#include "Helpers.h"
#include "Input.h"
#include "toolbars.h"
#include <iostream>


GLhandleARB vertexShader[32],fragmentShader[32],shaderProgram[32];

GLhandleARB testVertexShader,testFragmentShader,testShaderProgram;

CShaderManager* CShaderManager::m_instance = NULL;

HWND hSb;

SShaderId g_shaderId;

void CShaderManager::cleanUp()
{
	for(int i = 0; i < m_numberOfShaders; i++)
	{
		glDeleteObjectARB(shaderProgram[i]);
	}
}

void deleteShader(int id)
{
	glDeleteObjectARB(shaderProgram[id]);
}

void CShaderManager::create(){

	if(m_instance == NULL)
	{
		m_instance = new CShaderManager;
	}
}

void CShaderManager::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CShaderManager::detachShader()
{
	if(m_activeShader != 0)
	{
		m_activeShader = -1;
		glUseProgram(0);
	}

	std::cout << "shader fails" << "\n";
}

void CShaderManager::attachShader(int shaderId)
{
	if(m_activeShader != shaderProgram[shaderId])
	{
		m_activeShader = shaderProgram[shaderId];
		glUseProgram(shaderProgram[shaderId]);
	}
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();

		PostQuitMessage(0);
    }
    return retCode;
}

void printInfoLog(GLhandleARB obj, char *shaderCompilerMessage)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);

		hSb = GetDlgItem(g_hWnd, IDC_MAIN_STATUS);

		SendMessage(hSb, SB_SETTEXT, 0, (LPARAM)infoLog);

		if(shaderCompilerMessage != NULL) strcat(shaderCompilerMessage,infoLog);
		
        free(infoLog);
    }
}

int CShaderManager::loadShader(char *shaderConfigFile) {

	std::ifstream fin;
	std::string word;
	char vertexFile[32];
	char fragmentFile[32];

	//read in the shader config file
	fin.open(shaderConfigFile);

		//verify the shader config is legitimate
		fin >> word;

		if(word != "shader_config") 
		{
			MessageBox(NULL, "Not A Valid Shader File", "Error!", MB_OK);
			return -1;
		}

		//obtain the vertex shader name
		fin >> word;
		if(word == "vertex") fin >> vertexFile;

		//obtain the framgent shader name
		fin >> word;
		if(word == "fragment") fin >> fragmentFile;
	
	//close the shader config
	fin.close();

	//create a blank shader program
	shaderProgram[m_numberOfShaders] = glCreateProgramObjectARB();

	//create a blank vertex shader and fragment shader
	vertexShader[m_numberOfShaders] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShader[m_numberOfShaders] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	//read the vertex and framgment source code
	char *vs = NULL,*fs = NULL;
	vs = textFileRead(vertexFile);
	fs = textFileRead(fragmentFile);

	const char * vv = vs;
	const char * ff = fs;

	//compile the vertex shader
	glShaderSourceARB(vertexShader[m_numberOfShaders], 1, &vv,NULL);
	glCompileShaderARB(vertexShader[m_numberOfShaders]);
	glAttachObjectARB(shaderProgram[m_numberOfShaders],vertexShader[m_numberOfShaders]);

	//compile the fragment shader
	glShaderSourceARB(fragmentShader[m_numberOfShaders], 1, &ff,NULL);
	glCompileShaderARB(fragmentShader[m_numberOfShaders]);
	glAttachObjectARB(shaderProgram[m_numberOfShaders],fragmentShader[m_numberOfShaders]);

	//link the program
	glLinkProgramARB(shaderProgram[m_numberOfShaders]);

	//print error reports
	printInfoLog(vertexShader[m_numberOfShaders],NULL);
	printInfoLog(fragmentShader[m_numberOfShaders],NULL);
	printInfoLog(shaderProgram[m_numberOfShaders],NULL);

	//clean up
	free(vs);free(fs);

	return m_numberOfShaders++;
}

void CShaderManager::setup()
{
}

void CShaderManager::shaderCompiler(char *shaderConfigFile)
{
	std::ifstream fin;
	std::string word;
	char vertexFile[32];
	char fragmentFile[32];

	fin.open(shaderConfigFile);

		fin >> word;

		if(word != "shader_config") 
		{
			MessageBox(NULL, "Not A Valid Shader File", "Error!", MB_OK);
			return;
		}

		fin >> word;
		if(word == "vertex") fin >> vertexFile;

		fin >> word;
		if(word == "fragment") fin >> fragmentFile;
	
	fin.close();

	testShaderProgram = glCreateProgramObjectARB();

	char *vs = NULL,*fs = NULL;

	testVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	testFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead(vertexFile);
	fs = textFileRead(fragmentFile);

	const char * vv = vs;
	const char * ff = fs;

	glShaderSourceARB(testVertexShader, 1, &vv,NULL);
	glCompileShaderARB(testVertexShader);
	glAttachObjectARB(testShaderProgram,testVertexShader);

	glShaderSourceARB(testFragmentShader, 1, &ff,NULL);
	glCompileShaderARB(testFragmentShader);
	glAttachObjectARB(testShaderProgram,testFragmentShader);

	glLinkProgramARB(testShaderProgram);

	free(vs);free(fs);

	char shaderCompilerMessage[2046];

	strcpy(shaderCompilerMessage,"");

	printInfoLog(testVertexShader,shaderCompilerMessage);
	printInfoLog(testFragmentShader,shaderCompilerMessage);
	printInfoLog(testShaderProgram,shaderCompilerMessage);

	MessageBox(NULL, shaderCompilerMessage, "Compile Results", MB_OK);

}

void CShaderManager::shaderOptionHandler(WPARAM &wParam)
{
	switch(LOWORD(wParam))
	{
		case ID_TOOLS_SHADERCOMPILER:
		{
			char* shaderFile = openFile("glsl shader","glsl");

			if(shaderFile == NULL) return;

			shaderCompiler(shaderFile);

		}
	}
}

GLhandleARB CShaderManager::getShaderProgram(int id)
{
	return shaderProgram[id];
}

void CShaderManager::setUniform(int shaderID, char* name, float value)
{
	glUniform1fARB(
		glGetUniformLocationARB(getShaderProgram(shaderID), 
		name),value);
}


void CShaderManager::setUniform(int shaderID, char* name, float value1, float value2)
{
	glUniform2fARB(
		glGetUniformLocationARB(getShaderProgram(shaderID), 
		name),value1,value2);
}

void CShaderManager::setUniform(int shaderID, char* name, int value)
{
	//set the uniform
	glUniform1iARB(
	//get the uniforms location by the variable name
	glGetUniformLocationARB(getShaderProgram(shaderID),name),
	//set the value
	value);
}


void CShaderManager::setUniform(int shaderID, char* name, SVector3 value)
{
	//set the uniform
	glUniform3fARB(
	//get the uniforms location by the variable name
	glGetUniformLocationARB(getShaderProgram(shaderID),name),
	//set the value
	value.x,value.x,value.z);

}

void CShaderManager::setUniform(int shaderID, char* name, SVector3 value, float w)
{
	//set the uniform
	glUniform4fARB(
	//get the uniforms location by the variable name
	glGetUniformLocationARB(getShaderProgram(shaderID),name),
	//set the value
	value.x,value.y,value.z,w);

}

void CShaderManager::setUniform(int shaderID, char* name, float *matrix)
{
	//set the uniform
	glUniformMatrix4fvARB(
	//get the uniforms location by the variable name
	glGetUniformLocationARB(getShaderProgram(shaderID),name),
	//set the value
	1,1,matrix);
}
