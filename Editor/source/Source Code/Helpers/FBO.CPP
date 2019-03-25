#include "FBO.h"

void pingPong(CFBO *ping, CFBO *pong)
{
	/*CShaderManager::getInstance()->attachShader(g_shaderId.bloom);

	glPushMatrix();

	orthoMode(0,1024,1024,0);

	for(int i = 0; i < 1; i ++)
	{
		ping->bindFBO();
					
		ping->bindAsTexture(0);
		ping->bindAsTexture(1);

		glDisable(GL_LIGHTING);

		glUniform1iARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
		"decal"), 0);

		glUniform2fARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
		"u_scale"), 0,0.000976562);

		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glTranslated(1024 / 2,1024 / 2,0);
		glScaled(1024 / 2,1024 / 2,1);
		texturedQuad();
		glPopMatrix();

		shadowMap2->unbindFBO();

		shadowMap->bindFBO();
					
		shadowMap2->bindAsTexture(0);
		shadowMap2->bindAsTexture(1);

		glDisable(GL_LIGHTING);

		glUniform1iARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
		"decal"), 0);

		glUniform2fARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
		"u_scale"), 0.000976562,0);

		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glTranslated(1024 / 2,1024 / 2,0);
		glScaled(1024 / 2,1024 / 2,1);
		texturedQuad();
		glPopMatrix();

		shadowMap->unbindFBO();
	}

	CShaderManager::getInstance()->detachShader();

	perspectiveMode();

	glPopMatrix();*/
}

unsigned int CFBO::initFbo(int wrapmode)
{
	//generate frame buffer storage
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glGenRenderbuffersEXT(1, &depthbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_fboSize, m_fboSize);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

	//generate a storage image
	GLuint img;
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  m_fboSize, m_fboSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);

	return img;
}

unsigned int CFBO::initDepthFbo(){

	//Create the shadow map texture
	GLuint img;
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_fboSize, m_fboSize, 0,
					GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );

	return img;
}

CFBO::CFBO(int type,int size,int wrapmode)
{
	//create a new empty texture
	storageTexture = new unsigned int;

	//set the size of the fbo
	m_fboSize = size;
	
	if(type == 0) 
	{
		//colour FBO
		//create the storage
		*storageTexture = initFbo(wrapmode);
		//create the buffer object
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *storageTexture, 0);
	}
	else if(type == 1) 
	{
		//Depth FBO
		*storageTexture = initDepthFbo();

		//create the buffer object
		glGenFramebuffersEXT(1, &fbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glDrawBuffer(GL_FALSE);
		glReadBuffer(GL_FALSE);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, *storageTexture, 0);
	}

	//if there are any errors creating, terminate the program
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) PostQuitMessage(0);

	//bind the frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

CFBO::~CFBO()
{
	glDeleteFramebuffersEXT(1, &fbo);
	glDeleteRenderbuffersEXT(1, &depthbuffer);
}

void CFBO::bindAsTexture(int unit)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,*storageTexture);
}

void CFBO::bindFBO()
{
	//bind the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	//clear the screen like usual
	glClearColor (0,0,0,0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//save the old viewport size
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//resize the viewport to the size of the FBO
	glViewport(0,0,m_fboSize,m_fboSize);
}

void CFBO::unbindFBO()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFBO::copyScreenBuffer()
{
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	//glClearColor (0,0,0,0);
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glLoadIdentity();
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glViewport(0,0,m_fboSize,m_fboSize);

	bindAsTexture(0);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, 512, 512 , 0);

	//glPopAttrib();
}