#include "scene.h"
#include "helpers.h"
#include "input.h"
#include "timerSingleton.h"
#include "shaderManager.h"
#include "lighting.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "skyBox.h"
#include "octree.h"
#include "fonts.h"

CFrustum g_frustumCulling;

int g_objectCounter = 0;

CScene::CScene()
{
	skyBox = new CSkyBox();
	shadowTexture = new CFBO(1,2048);
	ball = new CBall();

	//Calculate & save matrices for the shadow casting light
	glPushMatrix();
	
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 50.0f, 3000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
	
	glLoadIdentity();
	glRotated(32,1,0,0);
	glRotated(180,0,1,0);
	glTranslated(0,-560,930);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	octree = new COctreeNode(SVector3(0,0,0),SVector3(1000,1000,1000));
}

void CScene::shadowMapRender()
{
		//SHADOW DEPTH TEXTURE
		shadowTexture->bindFBO();

		CTextureMan::getInstance()->disableAllUnits();
		
		//First pass - from light's point of view
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(lightProjectionMatrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(lightViewMatrix);

		glPolygonOffset(1,-1);
		glEnable(GL_POLYGON_OFFSET_FILL);
		//Disable color writes, and use flat shading for speed
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		//glDisable(GL_CULL_FACE);

		//Disable color writes, and use flat shading for speed
		glShadeModel(GL_FLAT);
		glColorMask(0, 0, 0, 0);
		
		mainRender();

		shadowTexture->unbindFBO();

		glDisable(GL_POLYGON_OFFSET_FILL);
		//restore states
		glShadeModel(GL_SMOOTH);
		glColorMask(1, 1, 1, 1);
		glEnable(GL_LIGHTING);
		glCullFace(GL_BACK);
}

void CScene::renderLitScene()
{
	glEnable(GL_LIGHTING);

	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 1.0f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	user::getInstance()->setCameraPos();

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	shadowTexture->bindAsTexture();

	buildShadowTextureMatrix();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_GEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	glAlphaFunc(GL_GEQUAL, 0.5f);
	glEnable(GL_ALPHA_TEST);

	float lightCol[4] = {0.0f,0.0f,0.0f,1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightCol);

	mainRender();
	floorPlane(0.5);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	glDisable(GL_ALPHA_TEST);
}

void CScene::buildOctree()
{
	octree->clear();

	for(int i = 0; i < blocks.size(); i++)
	{
		if(OBBVSOBB(blocks.at(i)->newPosition(),blocks.at(i)->scale(),blocks.at(i)->rotation(),
			octree->nodePos,octree->nodeScale,SVector3(0,0,0)))
		{
			octree->addObject(blocks.at(i));
		}
	}

	octree->subdivide();

	if(user::getInstance()->input.key['V'] && !user::getInstance()->input.keyLocked['V'])
	{
		MessageBox(g_hWnd, "", "Starting output", MB_OK);
		user::getInstance()->input.key['V'] = true;
		octree->enumerate();
	}
}

void CScene::buildShadowTextureMatrix()
{
	//bias matrix transforms from world space
	//(-1 - 1) (four quadrants) to texture space (0 - 1) one quadrant
	SMatrix4x4 biasMatrix;

	biasMatrix.entries[0] = 0.5f;
	biasMatrix.entries[1] = 0.0f;
	biasMatrix.entries[2] = 0.0f;
	biasMatrix.entries[3] = 0.0f;

	biasMatrix.entries[4] = 0.0f;
	biasMatrix.entries[5] = 0.5f;
	biasMatrix.entries[6] = 0.0f;
	biasMatrix.entries[7] = 0.0f;

	biasMatrix.entries[8] = 0.0f;
	biasMatrix.entries[9] = 0.0f;
	biasMatrix.entries[10] = 0.5f;
	biasMatrix.entries[11] = 0.0f;

	biasMatrix.entries[12] = 0.5f;
	biasMatrix.entries[13] = 0.5f;
	biasMatrix.entries[14] = 0.5f;
	biasMatrix.entries[15] = 1.0f;

	//perform matrix multiplication with view, projection and bias to get a texture matrix
	SMatrix4x4 lightViewProj = matrixMultiplication(biasMatrix,lightProjectionMatrix);
	SMatrix4x4 textureMatrix = matrixMultiplication(lightViewProj,lightViewMatrix);

	//Set up texture coordinate generation. using our texture matrix

	float row1[4] = {textureMatrix.entries[0],textureMatrix.entries[4],textureMatrix.entries[8],textureMatrix.entries[12]};
	float row2[4] = {textureMatrix.entries[1],textureMatrix.entries[5],textureMatrix.entries[9],textureMatrix.entries[13]};
	float row3[4] = {textureMatrix.entries[2],textureMatrix.entries[6],textureMatrix.entries[10],textureMatrix.entries[14]};
	float row4[4] = {textureMatrix.entries[3],textureMatrix.entries[7],textureMatrix.entries[11],textureMatrix.entries[15]};

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, row1);
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, row2);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, row3);
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, row4);
	glEnable(GL_TEXTURE_GEN_Q);
}

void CScene::backBufferRender()
{
	glDisable (GL_LIGHTING);

	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->renderBackBuffer();
	}

	int selectedID = -1;

	if((user::getInstance()->input.leftMouse) && (user::getInstance()->input.key['R']))
	{
		colorByte col = user::getInstance()->getPixelAtMouse();
		selectedID = col.r + col.g + col.b;
	}

	if(selectedID >= 0 && selectedID < blocks.size())
	{
		for(int i = 0; i < blocks.size(); i++)
		{
			blocks.at(i)->select(false);
		}

		blocks.at(selectedID)->select(true);
	}



	quickClear();

}

void CScene::mainRender()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	skyBox->render();

	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);
	
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "decal"), 1);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "normalMap"), 2);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "lightPos"), 0.0f, 0.0f, 0.0f, 1.0f);

	octree->clearRenderFlags();

	g_frustumCulling.CalculateFrustum();

	octree->render();

	CShaderManager::getInstance()->detachShader();

	ball->render();

	CModelManager::getInstance()->render(0);

}

void CScene::auxRender()
{
	glDisable(GL_LIGHTING);

	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->renderAux();
	}

	glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(4);

	if(user::getInstance()->input.key['O']) octree->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();

	orthoMode(0,0,1024,768);

	glColor3f(0.9,0.9,0.9);

	CText::getInstance()->output(10,15,"Press 'H' For Help");

	if(user::getInstance()->input.key['H'])
	{
		glEnable(GL_BLEND);

		//CTextureMan::getInstance()->disableAllUnits();

		glColor4f(0,0,0,0.7);

		glPushMatrix();

			glTranslated(512,384,0);

			glScaled(256,256,1);

			quad();

		glPopMatrix();

		glColor4f(1,1,1,1);

		CTextureMan::getInstance()->bindTexture(3);

		glPushMatrix();

			glTranslated(512,384,0);

			glScaled(256,256,1);

			texturedQuad();

		glPopMatrix();

		glDisable(GL_BLEND);

	}

	perspectiveMode();

	glPopMatrix();
}

void CScene::beginReflectionStencil()
{
	//create a stencil and a clip plane of the floor
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 

	glDisable(GL_DEPTH_TEST);

	floorPlane(1.0);
	
	//get ready to render
	glColorMask(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//clip plane
	double eqr[4] = { 0.0, -1.0, 0.0, 0.0};
	glEnable(GL_CLIP_PLANE0);						
	glClipPlane(GL_CLIP_PLANE0, eqr);	
	glDisable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	glPushMatrix();
	glScaled(1,-1,1);

}

void CScene::endReflectionStencil()
{
	//reset states that were set in begin();
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CLIP_PLANE0);
	glCullFace(GL_BACK);
}

void CScene::floorPlane(float lightness)
{
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	float alpha = 0.9f;
	if(lightness == 0.5) alpha = 0.5f;

	glColor4f(lightness,lightness,lightness,alpha);

	CTextureMan::getInstance()->bindTexture(2,1);

	glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,5,5); glVertex3f(1000,0,1000);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,-5,5); glVertex3f(-1000,0,1000);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,-5,-5); glVertex3f(-1000,0,-1000);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,5,-5); glVertex3f(1000,0,-1000);
	glEnd();

	CTextureMan::getInstance()->disableAllUnits();

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
}

void CScene::render()
{
	//double eqr[4] = { 0.0, -1.0, 0.0, -1000.0};
	//glEnable(GL_CLIP_PLANE1);						
	//glClipPlane(GL_CLIP_PLANE1, eqr);	

	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 1.0f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	quickClear();
	user::getInstance()->setCameraPos();

	setLights();

	backBufferRender();

	//mirrored
	beginReflectionStencil();
	mainRender();
	endReflectionStencil();

	float lightCol[4] = {1.0f,1.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightCol);

	mainRender();
	floorPlane(0.7);
	
	shadowMapRender();
	renderLitScene();

	auxRender();
}


void CScene::checkCollisions()
{
	//first recursion of collision, movement of blocks
	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->updateThisRecursion(true);
		blocks.at(i)->collisionBelow(false,0);

		//ignore the y axis for this recursion
		//concentrate on blocking moves
		blocks.at(i)->ignoreYAxis(true);
	}

	//use the octree to optimise collision checks
	octree->checkCollisions();

	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->updatePosition(true);
	}

	//second recursion gravity
	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->gravity();

		blocks.at(i)->updateThisRecursion(true);
		blocks.at(i)->collisionBelow(false,0);

		//this time we check the y axis and offset blocks ontop of one
		//another nicely so they can move freely and dont feel "sticky"
		blocks.at(i)->ignoreYAxis(false);
	}

	octree->checkCollisions();

	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->updatePosition(true);
	}



	//ball collisions
	//recursion #1 do the tops, and bottoms so the ball is aligned in the y plane
	ball->onEdge(false);

	for(int i = 0; i < blocks.size(); i++)
	{	
		SVector3 offsets[2];
		bool edge;

		SPHEREVSBOXTOPS(blocks.at(i)->newPosition(),blocks.at(i)->scale(),blocks.at(i)->rotation(),
						ball->getRadius(),ball->getPosition(),offsets,&edge);

		ball->onEdge(edge);
		ball->offset(offsets[0]);
	}

	//recursion #2 do the sides and offset in the x-z plane
	for(int i = 0; i < blocks.size(); i++)
	{	
		SVector3 offsets[4];

		SPHEREVSBOXSIDES(blocks.at(i)->newPosition(),blocks.at(i)->scale(),blocks.at(i)->rotation(),
						ball->getRadius(),ball->getPosition(),offsets);

		SVector3 offsetTotal = offsets[0] + offsets[1] + offsets[2] + offsets[3];

		ball->offset(offsetTotal);
	}		
}

void CScene::update()
{
	for(int i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->execute();
	}

	ball->control();

	checkCollisions();

	user::getInstance()->updatePos();
	user::getInstance()->mouseLook();
	CTimer::getInstance()->update();

	if(user::getInstance()->input.key['1'] && !user::getInstance()->input.keyLocked['1']) 
	{
		user::getInstance()->input.keyLocked['1'] = true;
		addBlock();
	}

}

void CScene::addBlock()
{
	int newSize = blocks.size() + 1;
	blocks.resize(newSize);

	blocks.at(newSize - 1) = new CToyBlock(newSize - 1);
}
