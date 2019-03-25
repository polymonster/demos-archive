#ifndef _FBO_H
#define _FBO_H

#include "globals.h"

class CFBO
{

private:

	int m_fboSize;
	unsigned int  fbo;
	unsigned int  depthbuffer;
	unsigned int  *storageTexture;
	
public:

	CFBO(int type,int size);
	~CFBO();
	unsigned int initFbo();
	unsigned int initDepthFbo();

	void setupTexture();

	void bindFBO();
	void unbindFBO();

	void bindAsTexture();

	unsigned int  emptyTexture();

	int fboSize(){return m_fboSize;};
};

#endif