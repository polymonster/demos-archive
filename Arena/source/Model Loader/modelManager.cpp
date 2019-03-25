#include "modelManager.h"
#include "textureSingleton.h"
#include "timerSingleton.h"

CModelManager* CModelManager::m_instance = NULL; 

C3dsLoader model[1024];


void CModelManager::create(){

	if(m_instance == NULL)
	{
		m_instance = new CModelManager;
	}
}

void CModelManager::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CModelManager::destroyObjects(){

	for(int i = 0; i < m_numberOfModels; i++){

		model[i].Unload_3ds();
	}

	m_numberOfModels = 0;
	//m_deletedModelStart = m_numberOfModels;

}

void CModelManager::destroyModel(int id){

	model[id].Unload_3ds();
	//m_numberOfModels--;
}

int CModelManager::loadMd3(char *filename,char *characterName)
{
	md3MeshModel[m_numberOfMd3].LoadModel(filename,characterName);

	return m_numberOfMd3++;
}

void CModelManager::linkMd3(CModelMD3 &character, int id)
{
	t3DModel *pUpper ,*pLower, *pHead;
	t3DModel *toUpper, *toLower, *toHead;

	pUpper = md3MeshModel[id].GetModel(0);
	pLower = md3MeshModel[id].GetModel(1);
	pHead = md3MeshModel[id].GetModel(2);

	toUpper = character.GetModel(0);
	toLower = character.GetModel(1);
	toHead = character.GetModel(2);


}

void CModelManager::destroyMd3(int id)
{

}

int CModelManager::open(const char *filename){

	if(filename != NULL)
	{
		model[m_numberOfModels].Init_3ds(parseString(filename,"data/model/"));

		filename = parseString(filename,"data/model/");

		//m_filename[m_numberOfModels] = filename;

		m_polyCounter += model[m_numberOfModels].m_polyCount;

		m_numberOfModels++;
		return m_numberOfModels - 1;
	}

	return - 1;
}

int CModelManager::load(char *file){

	cout << "\nloading mesh : " << m_numberOfModels;

	//m_filename[m_numberOfModels] = file;
	
	model[m_numberOfModels].Init_3ds(file);

	m_polyCounter += model[m_numberOfModels].m_polyCount;

	m_numberOfModels++;

	return m_numberOfModels - 1;
	
	//return - 1;
}

void CModelManager::render(int id){

  model[id].Render_3ds();

}

void CModelManager::renderNoTex(int id){

  model[id].RenderNoTexture();

}

void CModelManager::printName(ofstream &fout,int model){

	//fout << m_filename[model];

}

void CModelManager::translateToVertex(int i, int id){

	model[id].translateToVert(i);
}

void CModelManager::renderLightmap(int id){

	model[id].renderMappedQuad();
}

bool CModelManager::checkFrustum(int id,CFrustum frustum)
{
	//Do all this bullshit because of max's stupid z-up system
	int number = model[id].returnNumberFaces();

	if(number != 12) return false;

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

	return false;
}

void CModelManager::checkAllCollision(CPlayer *player,int id,bool justHeight){

	int number = model[id].returnNumberFaces();

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

		if(spherePolygonCollision(poly,player->posVector(),3,player->radiusVector().y,offset,true,player->radiusVector(),justHeight))
		{
			if(justHeight)
			{
				offset.x =0;
				offset.z =0;
			}

			player->moveVector(offset);
		}
	}

}

bool CModelManager::lineCollision(SVector3 line[],int id,SVector3 *result, STriangle *tri, SVector3 *normal)
{
	int number = model[id].returnNumberFaces();
	

	SVector3 poly[4];
	SVector3 offset;

	SVector3 resultLocation(0,0,0);

	for(int i = 0; i < number; i++)
	{
		model[id].getVerts(i,poly[0], poly[1], poly[2], poly[3]);

		SVector3 s(1,1,1);

		for(int j = 0; j < 3; j++) 
		{
			tri->point[j] = poly[j];
		}

		poly[0] = rotateAndScale(poly[0],s,90);
		poly[1] = rotateAndScale(poly[1],s,90);
		poly[2] = rotateAndScale(poly[2],s,90);

		SVector3 returnNormal = model[id].getModelsNormal(i);

		if(intersectedPolygon(poly,line,3,*result))
		{
			SVector3 norm = getNormal(poly);

			normal->x = returnNormal.x;
			normal->y = returnNormal.z;
			normal->z = returnNormal.y;

			return true;
		}
	}

	return false;

}

void CModelManager::setCollisionGrid(int id){

	surroundingColsest[0] = 0;
	surroundingColsest[1] = 1;
	surroundingColsest[2] = 2;
	surroundingColsest[3] = 3;
	surroundingColsest[4] = 4;
	surroundingColsest[5] = 5;

	int grid = sqrt((model[id].returnNumberFaces() * 2));
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
	}
}

int CModelManager::checkCollision(CPlayer *player,int id){

	glDisable(GL_LIGHTING);

	int closest = model[id].returnClosestVert(player->posVector());

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

		if(spherePolygonCollision(poly,player->posVector(),3,player->radiusVector().y,offset,true,player->radiusVector(),true))
		{
			offset.x =0;
			offset.z =0;
			player->moveVector(offset);
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

		if(spherePolygonCollision(poly,player->posVector(),3,player->radiusVector().y,offset,true,player->radiusVector(),true))
		{
			offset.x =0;
			offset.z =0;
			player->moveVector(offset);
		}
	}

	return 0;
}

CAnimatedModel::CAnimatedModel(const char *filename){
	
	ifstream fin;

	m_numberOfMeshes = 0;
	m_numberOfAnimations = 0;

	fin.open(filename);

	string word;

	char file[1024];

	coreModel = new CalCoreModel("yes");

	while(!fin.eof())
	{
		fin >> word;

		if(word == "skeleton")
		{
			fin >> file;

			if(!coreModel->loadCoreSkeleton(file))
			{
				cout << filename << "skeleton failed to load\n";
			}

		}
		else if(word == "mesh")
		{
			fin >> file;

			m_meshID[m_numberOfMeshes] = coreModel->loadCoreMesh(file);

			if(m_animID[m_numberOfMeshes] == -1) cout << filename << "mesh "<< m_numberOfMeshes - 1 << " failed to load\n";

			fin >> file;

			//m_textureID[m_numberOfMeshes++] = CTextureMan::getInstance()->loadNpc(file);
		}
		else if(word == "animation")
		{
			fin >> file;

			m_animID[m_numberOfAnimations++] = coreModel->loadCoreAnimation(file);

			if(m_animID[m_numberOfAnimations - 1] == -1) cout << filename << "animation "<< m_numberOfAnimations - 1 << " failed to load\n";
		}
	}

	//CTextureMan::getInstance()->loadNpc("data/newModels/Head [0].bmp");
	//CTextureMan::getInstance()->loadNpc("data/newModels/Torso [1].bmp");

	model = new CalModel(coreModel);

	for(int i = 0; i < m_numberOfMeshes; i++)
	{
		if(!model->attachMesh(m_meshID[i]))
		{
			cout << "mesh " << i << " did not attach";
		}
	}

	model->setLodLevel(0.1f);

	fin.close();
	//glGenBuffersARB(1, &vbo);
	//glBindBufferARB(GL_STREAM_DRAW_ARB, vbo);  
}

int CModelManager::loadCharacter(const char *filename){


	character[m_numberOfCharacters++] = new CAnimatedModel(filename);

	return m_numberOfCharacters -1;

}

void CModelManager::renderCharacter(int id){

	character[id]->renderCal(0);
}

void CModelManager::updateCharacter(int id){

	character[id]->update(0);
}


void CModelManager::destroyCharacters(){
	
	for(int i = 0; i < m_numberOfCharacters; i++)
	{
		//delete character[i];
	}
	
	//CTextureMan::getInstance()->freeNpc();

	m_numberOfCharacters = 0;
}


void CAnimatedModel::update(int id){
	
	model->getMixer()->blendCycle(m_animID[0], 0.8f, 0.3f);

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
