#include "modelManager.h"
#include "TimerSingleton.h"

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

void CModelManager::drawDebug(int id)
{
	model[id].drawNormals();
	model[id].drawTangents();
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
}

int CModelManager::open(const char *filename){

	if(filename != NULL)
	{
		//cout << "\nloading mesh : " << m_numberOfModels;

		char file[1024];

		strcpy(file,filename);

		model[m_numberOfModels].Init_3ds(file);

		//filename = parseString(filename,"data/model/");

		//m_filename[m_numberOfModels] = filename;

		m_polyCounter += model[m_numberOfModels].m_polyCount;

		m_numberOfModels++;
		return m_numberOfModels - 1;
	}

	return - 1;
}

int CModelManager::load(char *file){

	//cout << "\nloading mesh : " << m_numberOfModels;

	//m_filename[m_numberOfModels] = file;
	
	model[m_numberOfModels].Init_3ds(file);

	m_polyCounter += model[m_numberOfModels].m_polyCount;

	m_numberOfModels++;

	return m_numberOfModels - 1;
	
	//return - 1;
}

void CModelManager::deleteVBO(int id)
{
	model[id].deleteVBO();
}

void CModelManager::render(int id){

	model[id].drawVBO();
	//model[id].Render_3ds();

}

void CModelManager::renderNoTex(int id){

  model[id].RenderNoTexture();

}

/*void CModelManager::printName(ofstream &fout,int model){

	fout << m_filename[model];

}*/

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

	//if(number != 12) cout << "this is not a fucking box you douche";

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
	for(int i = 1; i < number; i++)
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

int CModelManager::loadCharacter(const char *filename){

	//cout << "\nnumber of models " << m_numberOfModels;

	//int temp = m_numberOfModels;

	//character[m_numberOfCharacters++] = new CAnimatedModel(filename);

	//cout << "number of chars " << m_numberOfCharacters;

	//m_numberOfModels = temp;

	//cout << "\nnumber of models " << m_numberOfModels;

	//return m_numberOfCharacters -1;

	return 0;

}

void CModelManager::renderCharacter(int id){

	//character[id]->renderCal(0);
}

void CModelManager::updateCharacter(int id){

	//character[id]->update(0);
}


void CModelManager::destroyCharacters(){
	
	//for(int i = 0; i < m_numberOfCharacters; i++)
	//{
		//delete character[i];
	//}
	//m_numberOfCharacters = 0;
}

