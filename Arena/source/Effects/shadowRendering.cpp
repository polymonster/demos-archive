#include "map.h"
#include "modelManager.h"
#include "shader manager/shaderManager.h"
#include "timerSingleton.h"
#include "HCI.h"

void CMap::shadowPass()
{
	glDisable(GL_LIGHTING);

	//SHADOW TEXTURE TO BLUR
	shadowTextureFBO->bindFBO();

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		
		//First pass - from light's point of view
		glClearDepth(1);
		glClearColor (0,0,0,0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(lightProjectionMatrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(lightViewMatrix);

		//Disable color writes, and use flat shading for speed
		glShadeModel(GL_FLAT);
		glColorMask(0, 0, 0, 1);

		for(int i = 0; i < m_numberOfPlayers; i++)
		{
			m_player[i]->draw();
		}

	shadowTextureFBO->unbindFBO();

	//SHADOW DEPTH TEXTURE
	shadowDepthFBO->bindFBO();

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		
		//First pass - from light's point of view
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(lightProjectionMatrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(lightViewMatrix);

		//Disable color writes, and use flat shading for speed
		glCullFace(GL_BACK);

		//Disable color writes, and use flat shading for speed
		glShadeModel(GL_FLAT);
		glColorMask(0, 0, 0, 0);

		for(i = 0; i < m_numberOfPlayers; i++)
		{
			m_player[i]->shadowVolumeBB();
		}

	shadowDepthFBO->unbindFBO();

	//restore states
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);
}

void CMap::buildTextureMatrix()
{
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.5f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.5f, 0.0f,
								0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);
}

void CMap::renderShadowStencil()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);						
	glStencilFunc(GL_ALWAYS, 1, 1);				
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	

	glEnable(GL_LIGHTING);
	//glColor4f(1,1,1,1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspectRatio, 1.0f, 6000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, xScreenSize, yScreenSize);
	m_player[0]->setCamPos();

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	shadowDepthFBO->bindAsTexture();

	buildTextureMatrix();

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_GEQUAL);
	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	glDisable(GL_LIGHTING);
	glColor4f(0,0,0,1);

	glShadeModel(GL_FLAT);
	glColorMask(1, 1, 1, 1);

	renderScene();

	glActiveTextureARB(GL_TEXTURE0);
	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	glDisable(GL_ALPHA_TEST);

	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

}

void CMap::shadowMapBlur()
{
	int blurSamples = 10;

	float blurPixelSize = 1.0f / fboSize;

	CShaderManager::getInstance()->attachShader(0);

		gaussFBO[0]->bindFBO();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, fboSize, fboSize);

		OrthoMode(0,fboSize,fboSize,0);

		glDisable(GL_CULL_FACE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		shadowTextureFBO->bindAsTexture();
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		shadowTextureFBO->bindAsTexture();

		glUniform1iARB(glslDecal, 0);
		glUniform2fARB(glslScale, blurPixelSize,0);
		
		glPushMatrix();
		glTranslated(fboSize / 2,fboSize / 2,0);
		glScaled(fboSize / 2,fboSize / 2,1);
		texturedQuad();
		glPopMatrix();

		gaussFBO[0]->unbindFBO();

		gaussFBO[1]->bindFBO();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, fboSize, fboSize);

		OrthoMode(0,fboSize,fboSize,0);

		glDisable(GL_CULL_FACE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		gaussFBO[0]->bindAsTexture();
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		gaussFBO[0]->bindAsTexture();

		glUniform1iARB(glslDecal, 0);
		glUniform2fARB(glslScale, 0,blurPixelSize);
		
		glPushMatrix();
		glTranslated(fboSize / 2,fboSize / 2,0);
		glScaled(fboSize / 2,fboSize / 2,1);
		texturedQuad();
		glPopMatrix();

		gaussFBO[1]->unbindFBO();

	CShaderManager::getInstance()->detachShader();

}


void CMap::litScene()
{
	glStencilFunc(GL_EQUAL, 1, 1);											
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		

	buildTextureMatrix();

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glEnable(GL_STENCIL_TEST);

	glColor4f(1,1,1,0.9);

	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_ALPHA_TEST);

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	gaussFBO[1]->bindAsTexture();

	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double eqr[4] = {0,1,0,-(m_player[0]->posVector().y - 120)};

	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqr);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1,1);

	renderScene();

	glDisable(GL_CLIP_PLANE0);

	glDisable(GL_POLYGON_OFFSET_FILL);

	glActiveTextureARB(GL_TEXTURE0);
	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	glDisable(GL_ALPHA_TEST);

	glDisable(GL_STENCIL_TEST);


}

void CMap::darkenedScene()
{	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspectRatio, 1.0f, 6000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, xScreenSize, yScreenSize);
	m_player[0]->setCamPos();

	glActiveTextureARB(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);

	renderScene();
}