#include "ShaderManager.h"
#include "Helpers.h"
#include "Input.h"

GLhandleARB vertexShader[6],fragmentShader[6],shaderProgram[6];

GLhandleARB testVertexShader,testFragmentShader,testShaderProgram;

CShaderManager* CShaderManager::m_instance = NULL;

HWND hSb;

SShaderId g_shaderId;

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
	glUseProgram(0);
}

void CShaderManager::attachShader(int shaderId)
{
	glUseProgram(shaderProgram[shaderId]);
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

		/*hSb = GetDlgItem(g_hWnd, IDC_MAIN_STATUS);

		SendMessage(hSb, SB_SETTEXT, 0, (LPARAM)infoLog);*/

		if(shaderCompilerMessage != NULL) strcat(shaderCompilerMessage,infoLog);
		
        free(infoLog);
    }
}

int CShaderManager::loadShader(char *shaderConfigFile) {

	std::ifstream fin;
	std::string word;
	char vertexFile[32];
	char fragmentFile[32];

	fin.open(shaderConfigFile);

		fin >> word;

		if(word != "shader_config") 
		{
			MessageBox(NULL, "Not A Valid Shader File", "Error!", MB_OK);
			return -1;
		}

		fin >> word;
		if(word == "vertex") fin >> vertexFile;

		fin >> word;
		if(word == "fragment") fin >> fragmentFile;
	
	fin.close();

	shaderProgram[m_numberOfShaders] = glCreateProgramObjectARB();

	char *vs = NULL,*fs = NULL;

	vertexShader[m_numberOfShaders] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShader[m_numberOfShaders] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead(vertexFile);
	fs = textFileRead(fragmentFile);

	const char * vv = vs;
	const char * ff = fs;

	glShaderSourceARB(vertexShader[m_numberOfShaders], 1, &vv,NULL);
	glCompileShaderARB(vertexShader[m_numberOfShaders]);
	glAttachObjectARB(shaderProgram[m_numberOfShaders],vertexShader[m_numberOfShaders]);

	glShaderSourceARB(fragmentShader[m_numberOfShaders], 1, &ff,NULL);
	glCompileShaderARB(fragmentShader[m_numberOfShaders]);
	glAttachObjectARB(shaderProgram[m_numberOfShaders],fragmentShader[m_numberOfShaders]);

	glLinkProgramARB(shaderProgram[m_numberOfShaders]);


	free(vs);free(fs);

	printInfoLog(vertexShader[m_numberOfShaders],NULL);
	printInfoLog(fragmentShader[m_numberOfShaders],NULL);
	printInfoLog(shaderProgram[m_numberOfShaders],NULL);

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
	/*switch(LOWORD(wParam))
	{
		case ID_TOOLS_SHADERCOMPILER:
		{
			char* shaderFile = openFile("glsl shader","glsl");
			shaderCompiler(shaderFile);

		}
	}*/
}

GLhandleARB CShaderManager::getShaderProgram(int id)
{
	return shaderProgram[id];
}
