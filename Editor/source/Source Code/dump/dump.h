	/*SVector3 camPos = g_input.pos;

	if(g_editMode == 2) m_terrain->backBufferRender();

	quickClear();

	//m_terrain->renderPicking();

	CShaderManager::getInstance()->attachShader(g_shaderId.terrain);

	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.terrain), "lightPos"), lights.x, lights.y, lights.z, 1.0f);

	//TERRAIN
	//m_terrain->render();

	m_sceneObject.back()->render();

	CShaderManager::getInstance()->detachShader();

	//--------------------------------

	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "decal"), 0);
    glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "parallaxMap"), 2);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "normalMap"), 1);
	
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "lightPos"), 0,0,0, 1.0f);

	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "cameraPos"), camPos.x, camPos.y, camPos.z, 1.0f);

	glPushMatrix();

		testReflect->bindFBO();

			double eqr[4];

			eqr[0] = 0.0f;
			eqr[1] = -1.0f;
			eqr[2] = 0.0f;
			eqr[3] = 20;

			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, eqr);	

			glDisable(GL_CULL_FACE);

			glFrontFace(GL_CW);

			glDisable(GL_CULL_FACE);

			glTranslated(0,-40,0);

			glScaled(1,-1,1);

			glTranslated(m_center.x,m_center.y,m_center.z);

			glScaled(m_scale.x,m_scale.y,m_scale.z);

			glFrontFace(GL_CCW);
			
			CModelManager::getInstance()->render(pmon);

			glDisable(GL_CLIP_PLANE0);

		testReflect->unbindFBO();

		glPopMatrix();

		glPushMatrix();
			
			glTranslated(m_center.x,m_center.y,m_center.z);

			glScaled(m_scale.x,m_scale.y,m_scale.z);

			CModelManager::getInstance()->render(pmon);

		glPopMatrix();

	//Water
	CShaderManager::getInstance()->attachShader(g_shaderId.water);
	
	glActiveTextureARB(GL_TEXTURE7_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindCubeMap();
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindTexture(8);

	glActiveTextureARB(GL_TEXTURE2_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindTexture(8);

	glActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);

	testReflect->bindAsTexture();

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "cubeMap"), 7);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "normalMap"), 1);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "normalMap2"), 2);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "local"), 0);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "lightPos"), lights.x, lights.y, lights.z, 0.0f);
    glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.water), "cameraPos"), camPos.x, camPos.y, camPos.z, 1.0f);

	glNormal3f(0,1,0);

	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,0 + texOffseter[0]); 
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB,10 + texOffseter[1],10); 
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,0); 
		glVertex3f(-500,-20,-500);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,10,0 + texOffseter[0]); 
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB,0 + texOffseter[1],10);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0,1); 
		glVertex3f(500,-20,-500);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,10,10 + texOffseter[0]); 
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB,0 + texOffseter[1],0); 
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,1); 
		glVertex3f(500,-20,500);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0,10 + texOffseter[0]); 
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB,10 + texOffseter[1],0); 
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1,0); 
		glVertex3f(-500,-20,500);
	glEnd();

	CShaderManager::getInstance()->detachShader();

	texOffseter[1] += 0.03 * CTimer::getInstance()->animBaseSpeed();
	texOffseter[0] += 0.03 * CTimer::getInstance()->animBaseSpeed();*/


/*void CModelManager::checkAllCollision(CPenguin &player,int id,bool justHeight){

	int number = model[id].returnNumberFaces();

	//cout << "number of faces : " << number;

	SVector3 poly[4];
	SVector3 offset;

	for(int i = 0; i < number; i++)
	{
		model[id].getVerts(i,poly[0], poly[1], poly[2], poly[3]);

		SVector3 s(1,1,1);

		poly[0] = rotateAndScale(poly[0],s,90);
		poly[1] = rotateAndScale(poly[1],s,90);
		poly[2] = rotateAndScale(poly[2],s,90);
		poly[3] = rotateAndScale(poly[3],s,90);

		if(spherePolygonCollision(poly,player.posVector(),3,player.radiusVector().y,offset,true,player.radiusVector(),justHeight))
		{
			if(justHeight)
			{
				offset.x =0;
				offset.z =0;
			}

			player.moveVector(offset);
		}

	}

}*/

void CModelManager::setCollisionGrid(int id){

	/*surroundingColsest[0] = 0;
	surroundingColsest[1] = 1;
	surroundingColsest[2] = 2;
	surroundingColsest[3] = 3;
	surroundingColsest[4] = 4;
	surroundingColsest[5] = 5;

	//int grid = sqrt((model[id].returnNumberFaces() * 2));
	int row = 1;

	for(int i = 6; i < 32; i+= 10)
	{

		for(int j = 0; j < 5; j++)
		{
			surroundingColsest[i + j] = (grid * row) + j;

		}

		for(int k = 0; k < 5; k++)
		{
			surroundingColsest[i + 5 + k] = (grid * row) - k;
		}
		row++;
	}*/
}

/*int CModelManager::checkCollision(CPenguin &player,int id){

	glDisable(GL_LIGHTING);

	int closest = model[id].returnClosestVert(player.posVector());

	SVector3 poly[4];

	for(int l = 0; l < 31; l++)
	{

		int face = closest + surroundingColsest[l];

		model[id].getVerts(face,poly[0], poly[1], poly[2], poly[3]);

		SVector3 s(1,1,1);

		poly[0] = rotateAndScale(poly[0],s,90);
		poly[1] = rotateAndScale(poly[1],s,90);
		poly[2] = rotateAndScale(poly[2],s,90);
		poly[3] = rotateAndScale(poly[3],s,90);

		SVector3 offset;

		if(spherePolygonCollision(poly,player.posVector(),3,player.radiusVector().y,offset,true,player.radiusVector(),true))
		{
			offset.x =0;
			offset.z =0;
			player.moveVector(offset);
		}

		for(int i = 0; i < 4; i++)
		{
			int r = 0,g = 0,b = 0;

			if(i == 0) r = 1;
			if(i == 1) g = 1;
			if(i == 2) b = 1;
			if(i == 3) r =1,b = 1;
			glPushMatrix();	

			glColor3f(r,g,b);
			glTranslated(poly[i].x, poly[i].y, poly[i].z);
			glutSolidCube(5);
			glPopMatrix();
		}

		face = closest - surroundingColsest[l];

		model[id].getVerts(face,poly[0], poly[1], poly[2], poly[3]);

		poly[0] = rotateAndScale(poly[0],s,90);
		poly[1] = rotateAndScale(poly[1],s,90);
		poly[2] = rotateAndScale(poly[2],s,90);
		poly[3] = rotateAndScale(poly[3],s,90);

		if(spherePolygonCollision(poly,player.posVector(),3,player.radiusVector().y,offset,true,player.radiusVector(),true))
		{
			offset.x =0;
			offset.z =0;
			player.moveVector(offset);
		}
	}

	return 0;
}*/

void CAnimatedModel::update(int id){
	
	model->getMixer()->blendCycle(m_animID[id], 0.8f, 0.3f);

	model->update(CTimer::getInstance()->animBaseSpeed());
}

void CAnimatedModel::setState(int id){

	//calModel[0]->getMixer()->executeAction(animID[0][id], 0.8f, 0.3f);
	model->getMixer()->blendCycle(m_animID[id], 0.8f, 0.3f);
}

void CAnimatedModel::clear(int id){

	model->getMixer()->clearCycle(m_animID[id], 0.3f);
}

void CAnimatedModel::executeState(int id){

	model->getMixer()->executeAction(m_animID[id], 0.3f, 1.5f);

}

void CAnimatedModel::renderCal(int id)
{
	//update(id);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	int m_vertexCount = 0;
	int m_faceCount = 0;

	  // get the renderer of the model
	CalRenderer *pCalRenderer;
	pCalRenderer = model->getRenderer();

    // begin the rendering loop
  if(pCalRenderer->beginRendering())
  {
	//glEnable(GL_COLOR_MATERIAL);
	//glColor4f(0.2,0.2,0.2,1);
	//glDisable(GL_COLOR_MATERIAL);
    // set global OpenGL states
    //glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

    // we will use vertex arrays, so enable them
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);

    // get the number of meshes
    int meshCount;
    meshCount = pCalRenderer->getMeshCount();

    // render all meshes of the model
    int meshId;
    for(meshId = 0; meshId < meshCount; meshId++)
    {
      // get the number of submeshes
      int submeshCount;
      submeshCount = pCalRenderer->getSubmeshCount(meshId);

      // render all submeshes of the mesh
      int submeshId;
      for(submeshId = 0; submeshId < submeshCount; submeshId++)
      {
        // select mesh and submesh for further data access
        if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
        {
          //unsigned char meshColor[4];
          //GLfloat materialColor[4];

          // set the material ambient color
          //pCalRenderer->getAmbientColor(&meshColor[0]);
          //materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          //glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

          // set the material diffuse color
          //pCalRenderer->getDiffuseColor(&meshColor[0]);
          //materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          //glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

          // set the material specular color
          //pCalRenderer->getSpecularColor(&meshColor[0]);
          //materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
          //glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

          // set the material shininess factor
          //float shininess;
          //shininess = 50.0f; //pCalRenderer->getShininess();
          //glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

          // get the transformed vertices of the submesh
          static float meshVertices[30000][3];
          int vertexCount;
          vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

          // get the transformed normals of the submesh
          static float meshNormals[30000][3];
          pCalRenderer->getNormals(&meshNormals[0][0]);

          // get the texture coordinates of the submesh
          static float meshTextureCoordinates[30000][2];
          int textureCoordinateCount;
          textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

          // get the faces of the submesh
          static CalIndex meshFaces[50000][3];
          int faceCount;
          faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

          // set the vertex and normal buffers
          glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
          glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

          // set the texture coordinate buffer and state if necessary
          //if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          //{
            glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            //glEnable(GL_COLOR_MATERIAL);

            // set the texture id we stored in the map user data
			//CTextureMan::getInstance()->bindNpc(m_textureID[meshId]);

            // set the texture coordinate buffer
            glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
            //glColor3f(1.0f, 1.0f, 1.0f);
          //}

		  //glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, faceCount * 3, meshVertices, GL_DYNAMIC_DRAW_ARB);
		  //glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
		  
		  if(sizeof(CalIndex)==2)
			  glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
		  else
			  glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);

          // disable the texture coordinate state if necessary
          if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
          {
            glDisable(GL_COLOR_MATERIAL);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
          }

          // adjust the vertex and face counter
          m_vertexCount += vertexCount;
          m_faceCount += faceCount;

// DEBUG-CODE //////////////////////////////////////////////////////////////////
/*
glDisable(GL_LIGHTING);

glBegin(GL_LINES);
glColor3f(1.0f, 1.0f, 1.0f);
int vertexId;
for(vertexId = 0; vertexId < vertexCount; vertexId++)
{
const float scale = 0.3f;
  glVertex3f(meshVertices[vertexId][0], meshVertices[vertexId][1], meshVertices[vertexId][2]);
  glVertex3f(meshVertices[vertexId][0] + meshNormals[vertexId][0] * scale, meshVertices[vertexId][1] + meshNormals[vertexId][1] * scale, meshVertices[vertexId][2] + meshNormals[vertexId][2] * scale);
}
glEnd();

glEnable(GL_LIGHTING);
*/
////////////////////////////////////////////////////////////////////////////////
        }
      }
    }

    // clear vertex array state
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // clear light
    glDisable(GL_LIGHTING);
    //glDisable(GL_LIGHT0);
    //glDisable(GL_DEPTH_TEST);

    // end the rendering
    pCalRenderer->endRendering();
  }


  // draw the bone lines
  /*float lines[1024][2][3];
  int nrLines;
  nrLines =  calModel[0]->getSkeleton()->getBoneLines(&lines[0][0][0]);
//  nrLines = m_calModel.getSkeleton()->getBoneLinesStatic(&lines[0][0][0]);

  glLineWidth(3.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
    int currLine;
    for(currLine = 0; currLine < nrLines; currLine++)
    {
      glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
      glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
    }
  glEnd();
  glLineWidth(1.0f);

  // draw the bone points
  float points[1024][3];
  int nrPoints;
  nrPoints =  calModel[0]->getSkeleton()->getBonePoints(&points[0][0]);
//  nrPoints = m_calModel.getSkeleton()->getBonePointsStatic(&points[0][0]);

  glPointSize(4.0f);
  glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 1.0f);
    int currPoint;
    for(currPoint = 0; currPoint < nrPoints; currPoint++)
    {
      glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
    }

  glEnd();
  glPointSize(1.0f);*/

}

CAnimatedModel::~CAnimatedModel(){

	delete model;
	delete coreModel;

	model = NULL;
	coreModel = NULL;

}



void testgrass()
{
	glDisable(GL_CULL_FACE);

	glPushMatrix();

	glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindTexture(3);
	glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindTexture(3);

	glEnable(GL_ALPHA_TEST);

	glRotated(180,0,0,1);

	glPushMatrix();
	glScaled(10,10,1);
	grassQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(90,0,1,0);
	glScaled(10,10,1);
	grassQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(45,0,1,0);
	glScaled(10,10,1);
	grassQuad();
	glPopMatrix();

	glPushMatrix();
	glRotated(-45,0,1,0);
	glScaled(10,10,1);
	grassQuad();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);

	glPopMatrix();
}

void CScene::bloom()
{
	//initial render
	/*glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, fboSize, fboSize);

		glDisable(GL_LIGHTING);

		//orthoMode(0,fboSize,fboSize,0);

		glPushMatrix();
			glTranslated(0,25,0);
			glScaled(50,50,1);
			glColor4f(0,0,0,0);
			quad();
		glPopMatrix();

		glPushMatrix();
			glTranslated(50,25,-50);
			glScaled(50,50,1);
			glColor3f(1,1,1);
			quad();
		glPopMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	int blurSamples = 3;

	CShaderManager::getInstance()->attachShader(g_shaderId.bloom);

	for(int i = 0 ; i < blurSamples; i++)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, fboSize, fboSize);

		orthoMode(0,fboSize,fboSize,0);

		glDisable(GL_CULL_FACE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,blurTexture);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,blurTexture);

		glUniform1iARB(glslDecal, 0);
		glUniform2fARB(glslScale, 0.00390625,0);
		
		glPushMatrix();
		glTranslated(fboSize / 2,fboSize / 2,0);
		glScaled(fboSize / 2,fboSize / 2,1);
		texturedQuad();
		glPopMatrix();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, fboSize, fboSize);

		orthoMode(0,fboSize,fboSize,0);

		glDisable(GL_CULL_FACE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,blurTexture2);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,blurTexture2);

		glUniform1iARB(glslDecal, 0);
		glUniform2fARB(glslScale, 0,0.00390625);
		
		glPushMatrix();
		glTranslated(fboSize / 2,fboSize / 2,0);
		glScaled(fboSize / 2,fboSize / 2,1);
		texturedQuad();
		glPopMatrix();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	CShaderManager::getInstance()->detachShader();*/
}

		/*glDisable(GL_LIGHTING);

		glPushMatrix();
			glTranslated(0,25,0);
			glScaled(50,50,1);
			glColor3f(0,0,0);
			quad();
		glPopMatrix();

		glPushMatrix();
		glTranslated(50,25,-50);
		glScaled(50,50,1);
		glColor3f(1,0.7,1);
		quad();
		glPopMatrix();

		bloom();

		glViewport(0, 0,g_screenWidth,g_screenHeight);

		glDepthMask(0);
		glDisable(GL_DEPTH_TEST);

		orthoMode(0,1024,1024,0);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D,blurTexture);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D,blurTexture);
		glDisable(GL_TEXTURE_2D);

		glBlendFunc(GL_ONE,GL_ONE);
		glEnable(GL_BLEND);

		glDisable(GL_LIGHTING);
		
		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glTranslated(1024 / 2,1024 / 2,0);
		glScaled(1024 / 2,1024 / 2,1);
		texturedQuad();
		glPopMatrix();

		glDisable(GL_BLEND);

		glDepthMask(1);
		glEnable(GL_DEPTH_TEST);

		glLoadIdentity();
		perspectiveMode();
		user::getInstance()->positionCamera();*/

	//GRASS
	/*CShaderManager::getInstance()->attachShader(g_shaderId.grass);

	int x;
	int z;

	x = -10;
	z = -10;

	int rot;

	offsetBlade += CTimer::getInstance()->animBaseSpeed() * offsetFactor;
	if(offsetBlade > 2)  offsetFactor = -1;
	if(offsetBlade < -2) offsetFactor = 1;

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.grass), "decal"), 0);
	glUniform1fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.grass), "vertexOffset"), offsetBlade);

	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			glPushMatrix();
			glTranslated(x * 15,10,z * 15);
			glRotated(rot,0,1,0);
			testgrass();
			glPopMatrix();

			z++;
			rot += 20;
		}

		z = -10;
		x++;
		rot += 65;
	}

	CShaderManager::getInstance()->detachShader();*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//										RENDER 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Render_3ds()
{
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);

	/*for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DObject *pObject = &m3DModel.pObject[i];	
		
		//CTextureMan::getInstance()->bindIcon(1);
		
		if(pObject->bHasTexture)									
		{		
			glActiveTextureARB(GL_TEXTURE0_ARB);
			
			//glColor3ub(255, 255, 255);								
			CTextureMan::getInstance()->bindTexture(textureArrayManager[i]);

			//glColor3f(0.8,0.8,0);
		} 
		else 
		{
			//glDisable(GL_TEXTURE_2D);								
			//glColor3ub(255, 255, 255);								
		}

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		
		glBegin(GL_TRIANGLES);

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glColor3f(vboTangents[index].x,vboTangents[index].y,vboTangents[index].z);
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				if(pObject->bHasTexture) {
					
					if(pObject->pTexVerts) 
					{
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(m3DModel.pMaterials.size() < pObject->materialID) 
					{
						BYTE *pColor = m3DModel.pMaterials[pObject->materialID].color;
						
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}

				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}
		
		glEnd();
	}

	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glDisable(GL_TEXTURE_2D);*/
}

/*======================= F U R ==================================*/

	/*glDisable(GL_LIGHTING);

	glPushMatrix();
	glScaled(20,20,20);
	CTextureMan::getInstance()->disableAllUnits();
	CTextureMan::getInstance()->bindTexture(0,0);
	CTextureMan::getInstance()->bindTexture(0,1);
	texturedQuad();
	glPopMatrix();

	float s = 0;

	//glDisable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);

	for(int i = 0; i < 30; i++)
	{
		s += (i * 0.005f);

		glPushMatrix();
		glTranslated(0,0,s);
		glScaled(20,20,20);
		CTextureMan::getInstance()->disableAllUnits();
		CTextureMan::getInstance()->bindTexture(11,1);
		CTextureMan::getInstance()->bindTexture(11,0);
		texturedQuad();
		glPopMatrix();
	}*/

CAnimatedModel::CAnimatedModel(const char *filename){
	
	std::ifstream fin;

	m_numberOfMeshes = 0;
	m_numberOfAnimations = 0;

	fin.open(filename);

	std::string word;

	char file[1024];

	coreModel = new CalCoreModel("yes");

	char strMessage[255] = {0};

	while(!fin.eof())
	{
		fin >> word;

		if(word == "skeleton")
		{
			fin >> file;

			if(!coreModel->loadCoreSkeleton(file))
			{
				//cout << filename << "skeleton failed to load\n";
				sprintf(strMessage, "Skeleton Failed to Load: %s!", filename);
				MessageBox(NULL, strMessage, "Error", MB_OK);
			}

		}
		else if(word == "mesh")
		{
			fin >> file;

			m_meshID[m_numberOfMeshes] = coreModel->loadCoreMesh(file);

			if(m_animID[m_numberOfMeshes] == -1) 
			{

			sprintf(strMessage, "Mesh Failed to Load: %s!", filename);
			MessageBox(NULL, strMessage, "Error", MB_OK);
			//fin >> file;
			//m_textureID[m_numberOfMeshes++] = CTextureMan::getInstance()->loadNpc(file);

			}

			m_numberOfMeshes++;
		}
		else if(word == "animation")
		{
			fin >> file;

			m_animID[m_numberOfAnimations++] = coreModel->loadCoreAnimation(file);

			if(m_animID[m_numberOfAnimations - 1] == -1) 
			{
				sprintf(strMessage, "Anim Failed to Load: %s!", filename);
				MessageBox(NULL, strMessage, "Error", MB_OK);
			}
		}
	}

	//CTextureMan::getInstance()->loadNpc("data/newModels/Head [0].bmp");
	//CTextureMan::getInstance()->loadNpc("data/newModels/Torso [1].bmp");

	model = new CalModel(coreModel);

	for(int i = 0; i < m_numberOfMeshes; i++)
	{
		if(!model->attachMesh(m_meshID[i]))
		{
			//cout << "mesh " << i << " did not attach";
			sprintf(strMessage, "meshes didn't attach: %s!", filename);
			MessageBox(NULL, strMessage, "Error", MB_OK);
		}
	}

	model->setLodLevel(0.1f);

	HWND hSb = GetDlgItem(g_hWnd, IDC_MAIN_STATUS);
	SendMessage(hSb, SB_SETTEXT, 0, (LPARAM)"Loaded");

	//glGenBuffersARB(1, &vbo);
	//glBindBufferARB(GL_STREAM_DRAW_ARB, vbo);  
}

int CModelManager::loadCharacter(const char *filename){

	//cout << "\nnumber of models " << m_numberOfModels;

	//int temp = m_numberOfModels;

	character[m_numberOfCharacters++] = new CAnimatedModel(filename);

	//cout << "number of chars " << m_numberOfCharacters;

	//m_numberOfModels = temp;

	//cout << "\nnumber of models " << m_numberOfModels;


	return m_numberOfCharacters -1;

}

void CModelManager::renderCharacter(int id){

	//character[id]->renderCal(0);
}

void CModelManager::updateCharacter(int id){

	//character[id]->update(0);
}


void CModelManager::destroyCharacters(){
	
	for(int i = 0; i < m_numberOfCharacters; i++)
	{
		//delete character[i];
	}

	m_numberOfCharacters = 0;
}

class CAnimatedModel{

private:

	int m_meshID[32];
	int m_animID[32];
	int m_textureID[32];

	int m_numberOfMeshes;
	int m_numberOfAnimations;

	CalCoreModel *coreModel;
	CalModel *model;

	GLuint vbo;

public:

	CAnimatedModel(const char *filename);
	~CAnimatedModel();
	//void renderCal(int id);

	//void update(int id);
	void setState(int id);
	void executeState(int id);
	void clear(int id);
};

bool CModelManager::checkFrustum(int id,CFrustum frustum)
{
	//Do all this bullshit because of max's stupid z-up system
	/*int number = model[id].returnNumberFaces();

	SVector3 verts[12][3];
	SVector3 offset;

	SVector3 s(1,1,1);

	for(int i = 0; i < number; i++)
	{
		SVector3 tempVerts[3];
		SVector3 blank;

		model[id].getVerts(i,tempVerts[0], tempVerts[1], tempVerts[2], blank);

		SVector3 s(1,1,1);

		tempVerts[0] = rotateAndScale(tempVerts[0],s,90);
		tempVerts[1] = rotateAndScale(tempVerts[1],s,90);
		tempVerts[2] = rotateAndScale(tempVerts[2],s,90);

		verts[i][0] = tempVerts[0];
		verts[i][1] = tempVerts[1];
		verts[i][2] = tempVerts[2];
	}

	//now onto the real stuff, use the vertex data to get a cube nicely in this form
	//center x, center y, center z, size

	//first get the boudries size will be the biggest.

	SVector3 boundryMin, boundryMax;

	boundryMin = verts[0][0];
	boundryMax = verts[0][0];

	//for the number of tri's
	for(i = 1; i < number; i++)
	{
		//loop each vert of the tri
		for(int j = 0; j < 3; j++)
		{
			if(verts[i][j].x < boundryMin.x) boundryMin.x = verts[i][j].x;
			else if(verts[i][j].x > boundryMax.x) boundryMax.x = verts[i][j].x;

			if(verts[i][j].y < boundryMin.y) boundryMin.y = verts[i][j].y;
			else if(verts[i][j].y > boundryMax.y) boundryMax.y = verts[i][j].y;

			if(verts[i][j].z < boundryMin.z) boundryMin.z = verts[i][j].z;
			else if(verts[i][j].z > boundryMax.z) boundryMax.z = verts[i][j].z;
		}
	
	}

	//with the boundries get the size and center

	SVector3 center;

	center.x = boundryMin.x + ((boundryMax.x - boundryMin.x) / 2);
	center.y = boundryMin.y + ((boundryMax.y - boundryMin.y) / 2);
	center.z = boundryMin.z + ((boundryMax.z - boundryMin.z) / 2);

	float size = float(boundryMax.x - boundryMin.x);

	if(frustum.CubeInFrustum(center.x,center.y,center.z,size / 2))
	{
		return true;
	}

	return false;*/

	return false;
}








void CScene::testBloom()
{
	//TEST BLUR
	m_blur[0]->bindFBO();

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::string name = m_sceneObject.at(i)->nameIdentifier;

		if(name== "water" || name== "terrain" || name == "atmosphere")
		{
			m_sceneObject.at(i)->render();
		}
	}

	renderProps();

	m_blur[0]->unbindFBO();

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	CShaderManager::getInstance()->attachShader(g_shaderId.brightPass);

	m_blur[1]->bindFBO();

		glPushMatrix();
		
		orthoMode(0,1024,1024,0);

		m_blur[0]->bindAsTexture(0);
		m_blur[0]->bindAsTexture(1);

		glDisable(GL_LIGHTING);

		glUniform1iARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.brightPass), 
		"decal"), 0);

		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glTranslated(1024 / 2,1024 / 2,0);
		glScaled(1024 / 2,1024 / 2,1);
		texturedQuad();
		glPopMatrix();

	m_blur[1]->unbindFBO();

	CShaderManager::getInstance()->detachShader();

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	CShaderManager::getInstance()->attachShader(g_shaderId.bloom);

	for(int i = 0; i < m_atmosphere->m_blurPasses; i ++)
	{
		m_blur[0]->bindFBO();
					
			m_blur[1]->bindAsTexture(0);
			m_blur[1]->bindAsTexture(1);

			glDisable(GL_LIGHTING);

			glUniform1iARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"decal"), 0);

			glUniform2fARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"u_scale"), 0,1.0f / 1024.0f);

			glPushMatrix();
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glTranslated(1024 / 2,1024 / 2,0);
			glScaled(1024 / 2,1024 / 2,1);
			texturedQuad();
			glPopMatrix();

		m_blur[0]->unbindFBO();

		m_blur[1]->bindFBO();
					
			m_blur[0]->bindAsTexture(0);
			m_blur[0]->bindAsTexture(1);

			glDisable(GL_LIGHTING);

			glUniform1iARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"decal"), 0);

			glUniform2fARB(
			glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.bloom), 
			"u_scale"), 1.0f / 1024.0f,0);

			glPushMatrix();
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glTranslated(1024 / 2,1024 / 2,0);
			glScaled(1024 / 2,1024 / 2,1);
			texturedQuad();
			glPopMatrix();

		m_blur[1]->unbindFBO();
	}

	CShaderManager::getInstance()->detachShader();

	CTextureMan::getInstance()->disableAllUnits();

	m_blur[1]->bindAsTexture(1);
	m_blur[1]->bindAsTexture(0);

	glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_BLEND);
				
	glPushMatrix();
	glColor4f(1.0,0.7,0.3,0.3);
	glTranslated(1024 / 2,1024 / 2,0);
	glScaled(1024 / 2,1024 / 2,1);
	texturedQuad();
	glPopMatrix();

	perspectiveMode();

	glPopMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

}


CSingleton *singleton 