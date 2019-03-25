#ifndef _FBO_H
#define _FBO_H

#include "Globals.h"



class CFBO
{

private:

	int m_fboSize;
	unsigned int  fbo;
	unsigned int  depthbuffer;
	unsigned int  *storageTexture;
	
public:

	CFBO(int type,int size,int wrapmode);
	~CFBO();

	unsigned int initFbo(int wrapmode);
	unsigned int initDepthFbo();

	void bindFBO();
	void bindAsTexture(int unit);
	void unbindFBO();

	void copyScreenBuffer();
	
	int fboSize(){return m_fboSize;};
};

void pingPong(CFBO *ping, CFBO *pong);

#endif