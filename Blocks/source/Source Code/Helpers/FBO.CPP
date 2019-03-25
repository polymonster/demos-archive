#include "FBO.h"

unsigned int CFBO::initFbo()
{
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glGenRenderbuffersEXT(1, &depthbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_fboSize, m_fboSize);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

	GLuint img;
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  m_fboSize, m_fboSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

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

	return img;
}

CFBO::CFBO(int type,int size)
{
	storageTexture = new unsigned int;

	m_fboSize = size;

	if(type == 0) 
	{
		*storageTexture = initFbo();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *storageTexture, 0);
	}
	else if(type == 1) 
	{
		*storageTexture = initDepthFbo();

		glGenFramebuffersEXT(1, &fbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glDrawBuffer(GL_FALSE);
		glReadBuffer(GL_FALSE);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, *storageTexture, 0);
	}

	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		//cout << "\n\nFBO FAILED";
		PostQuitMessage(0);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

CFBO::~CFBO()
{

}

void CFBO::bindAsTexture()
{
	glBindTexture(GL_TEXTURE_2D,*storageTexture);
}

void CFBO::bindFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glClearColor (0,0,0,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glViewport(0,0,m_fboSize,m_fboSize);
}

void CFBO::unbindFBO()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}