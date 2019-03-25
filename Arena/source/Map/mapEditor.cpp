#include "map.h"
#include "modelManager.h"
#include "shader manager/shaderManager.h"
#include "HCI.h"

int m_menuOption = -1;

CEditorMap::CEditorMap()
{
	m_selectedSpawnPoint = 0;
	m_numberOfSpawnPoints = 0;
	m_selectedPickup = 0;
	m_editMode = 0;
	m_numberOfPickups = 0;

	createMenu();

	weaponPickups[0] = new CWeapons("data/configs/weapons/machinegun.txt");

	weaponPickups[1] = new CWeapons("data/configs/weapons/shotgun.txt");

	weaponPickups[2] = new CWeapons("data/configs/weapons/plasma.txt");

	//weaponPickups[3] = new CWeapons("data/configs/weapons/plasma.txt");


	m_translateCenter.z = 0;
	m_translateCenter.y = 0;
	m_translateCenter.x = 0;

	m_selectedSpawnPoint = -1;

	m_callback = EDITOR_LOOP;
}


void CEditorMap::renderScene()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);

	int meshID = m_meshID[RENDER_MESH];

	glPushMatrix();

		glRotated(-90,1,0,0);

		if(meshID != -1)
		{
			CModelManager::getInstance()->render(meshID);
		}

		if(m_editMode == COLLISION_VIEW)
		{
			meshID = m_meshID[COLLISION_BLOCKING];

			if(meshID != -1)
			{
				CModelManager::getInstance()->render(meshID);
			}

			meshID = m_meshID[COLLISION_OFFSET];

			if(meshID != -1)
			{
				CModelManager::getInstance()->render(meshID);
			}
		}

	glPopMatrix();


	glDisable(GL_LIGHTING);

	translateWidgetNew(m_translateCenter,m_selectedTranslateAxis);

	drawSpawnPoints();


}

void CEditorMap::drawSpawnPoints()
{
	glDisable(GL_LIGHTING);
	glDepthMask(0);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glColor4f(1,0,1,0.5);

	for(int i = 0; i < m_numberOfSpawnPoints; i++)
	{
		glPushMatrix();
		
		glTranslated(spawnPoint[i].x,spawnPoint[i].y,spawnPoint[i].z);

		glScaled(70,100,70);

		glutSolidCube(1);

		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDepthMask(1);
	glEnable(GL_CULL_FACE);
}


void CEditorMap::addSpawnPoint()
{
	spawnPoint[m_numberOfSpawnPoints].x = 0;
	spawnPoint[m_numberOfSpawnPoints].y = 0;
	spawnPoint[m_numberOfSpawnPoints].z = 0;

	m_selectedSpawnPoint = m_numberOfSpawnPoints;

	m_numberOfSpawnPoints++;

	m_editMode = 1;
}

void CEditorMap::drawPickups()
{
	glDisable(GL_BLEND);

	for(int i = 0; i < m_numberOfPickups; i++)
	{
		pickups[i].render(weaponPickups);
	}
}

void CEditorMap::addPickup(int type)
{
	SVector3 origin(0,0,0);
	pickups[m_numberOfPickups].setType(type);
	pickups[m_numberOfPickups].setPos(origin);

	m_selectedPickup = m_numberOfPickups;

	m_numberOfPickups++;

	m_editMode = 2;

}
	
void CEditorMap::saveMap()
{
	ofstream fout;

	fout.open("data/configs/maps/map1.dat");

	for(int i = 0; i < m_numberOfSpawnPoints; i++)
	{
		fout << "spawn_point " << spawnPoint[i].x << " "
			 << spawnPoint[i].y << " " << spawnPoint[i].z << "\n";
	}

	for(i = 0; i < m_numberOfPickups; i++)
	{
		fout << "pickup_node " << pickups[i].pos().x << " "
		     << pickups[i].pos().y << " "
			 << pickups[i].pos().z << " " << pickups[i].type() << "\n";
	}

	fout.close();

}

static void processOptions(int option){
	
	m_menuOption = option;

}

void CEditorMap::createMenu(){

	int menu,pickup_sub;

	pickup_sub = glutCreateMenu(processOptions);
	glutAddMenuEntry("machine gun",MGUN);
	glutAddMenuEntry("shotgun",SGUN);
	glutAddMenuEntry("lazer gun",LGUN);
	glutAddMenuEntry("plasma gun",PGUN);

	menu = glutCreateMenu(processOptions);
	glutAddMenuEntry("pickup mode",PICKMODE);
	glutAddMenuEntry("spawn mode",SPAWNMODE);
	glutAddMenuEntry("spawn point",SPAWNP);
	glutAddSubMenu("pickup item",pickup_sub);
	glutAddMenuEntry("Delete",DELETE_EDIT);

	//glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void CEditorMap::detachMenu()
{
	//glutDetachMenu(GLUT_RIGHT_BUTTON);
}

void CEditorMap::build()
{
	if(user::getInstance()->input.key['1'])
	{
		m_editMode = 0;
	}
	else if(user::getInstance()->input.key['2'])
	{
		m_editMode = 1;
	}
	else if(user::getInstance()->input.key['3'])
	{
		m_editMode = 2;
	}
	else if(user::getInstance()->input.key['4'])
	{
		saveMap();
	}

	if(m_editMode == 1) //spawn point mode
	{
		if(m_selectedSpawnPoint != -1) m_translateCenter = spawnPoint[m_selectedSpawnPoint];

		if(user::getInstance()->input.key['='])
		{
			m_selectedSpawnPoint++;
			if(m_selectedSpawnPoint > m_numberOfSpawnPoints - 1) m_selectedSpawnPoint = 0;

			user::getInstance()->input.key['='] = false;

		}
		else if(user::getInstance()->input.key['-'])
		{
			m_selectedSpawnPoint--;
			if(m_selectedSpawnPoint < 0) m_selectedSpawnPoint = m_numberOfSpawnPoints - 1;

			user::getInstance()->input.key['-'] = false;
		}
	}

	if(m_editMode == 2) //spawn point mode
	{
		if(m_selectedPickup != -1) m_translateCenter = pickups[m_selectedPickup].pos();

		if(user::getInstance()->input.key['='])
		{
			m_selectedPickup++;
			if(m_selectedPickup > m_numberOfPickups - 1) m_selectedPickup = 0;

			user::getInstance()->input.key['='] = false;

		}
		else if(user::getInstance()->input.key['-'])
		{
			m_selectedPickup--;
			if(m_selectedPickup < 0) m_selectedPickup = m_numberOfPickups - 1;

			user::getInstance()->input.key['-'] = false;
		}
	}

	if(m_menuOption == -1) return;

	switch(m_menuOption)
	{

		case SPAWNP :

			addSpawnPoint();
			m_menuOption = -1;

		break;

		case MGUN :

			addPickup(GUN_MACHINE);
			m_menuOption = -1;

		break;

		case SGUN :

			addPickup(GUN_SHOT);
			m_menuOption = -1;

		break;

		case LGUN :

			addPickup(GUN_LAZER);
			m_menuOption = -1;

		break;

		case PGUN :

			addPickup(GUN_PLASMA);
			m_menuOption = -1;

		break;

	}
}

SVector3 translateWidgetDepthTest(SVector3 center)
{
	bool resultBool[3] = {0,0,0};

	//draw some boxes to depth test against for widget input.
	
	//work out ots overall scale dependant on view distance
	

	SVector3 worldSpacePos;
	worldSpacePos.x = user::getInstance()->input.xpos;
	worldSpacePos.y = user::getInstance()->input.ypos;
	worldSpacePos.z = user::getInstance()->input.zpos;

	float scale = distance(center,worldSpacePos) / 100;

	//translate to centre
	glPushMatrix();
	glTranslated(center.x,center.y,center.z);
	glScaled(scale,scale,scale);
		
		//center box
		glPushMatrix();
		glScaled(2,2,2);
		glutSolidCube(1);
		glPopMatrix();
		//x axis
		glPushMatrix();
		glTranslated(10,0,0);
		glScaled(10,1,1);
		glutSolidCube(1);
		glPopMatrix();
		//y axis
		glPushMatrix();
		glTranslated(0,10,0);
		glScaled(1,10,1);
		glutSolidCube(1);
		glPopMatrix();
		//z axis
		glPushMatrix();
		glTranslated(0,0,10);
		glScaled(1,1,10);
		glutSolidCube(1);
		glPopMatrix();

	glPopMatrix();

	//user::getInstance()->get3dMouseCoords();

	SVector3 pos = get3dCursorPos(user::getInstance()->input.mousex,user::getInstance()->input.mousey);

	//check collisions
	SVector3 boxCenter[3];
	SVector3 boundries[3];

	float offset = 10 * scale;
	float size = 12 * scale;
	float scalePadding = 3 * scale;

	/*float offset = 7;
	float size = 12;
	float scalePadding = 2;*/

	boxCenter[0].x = center.x + offset;
	boxCenter[0].y = center.y;
	boxCenter[0].z = center.z;

	boxCenter[1].x = center.x;
	boxCenter[1].y = center.y + offset;
	boxCenter[1].z = center.z;

	boxCenter[2].x = center.x;
	boxCenter[2].y = center.y;
	boxCenter[2].z = center.z + offset;

	boundries[0].x = size;
	boundries[0].y = scalePadding;
	boundries[0].z = scalePadding;

	boundries[1].x = scalePadding;
	boundries[1].y = size;
	boundries[1].z = scalePadding;

	boundries[2].x = scalePadding;
	boundries[2].y = scalePadding;
	boundries[2].z = size;
	
	/*CText::getInstance()->output(10,200,"bounds :%f,%f,%f", boundries[0].x, 
														   boundries[0].y,
														  boundries[0].z);*/

	for(int i = 0; i < 3; i++)
	{

		if((pos.x > boxCenter[i].x - boundries[i].x) && (pos.x < boxCenter[i].x + boundries[i].x) 
			&& (pos.y > boxCenter[i].y - boundries[i].y) && (pos.y < boxCenter[i].y + boundries[i].y) 
			&& (pos.z > boxCenter[i].z - boundries[i].z) && (pos.z < boxCenter[i].z + boundries[i].z))
		{
			resultBool[i] = true;
		}

	}

	SVector3 result(resultBool[0],resultBool[1],resultBool[2]);
	return result;
}

void CEditorMap::widgetDepthTests()
{
	glEnable(GL_DEPTH_TEST);

	if(!user::getInstance()->input.leftMouse)
	{
		m_selectedTranslateAxis = translateWidgetDepthTest(m_translateCenter);

	}
	else
	{
		float incx = user::getInstance()->input.incy / 5;
		float incy = user::getInstance()->input.incx / 5;

		SVector3 worldSpacePos;
		worldSpacePos.x = user::getInstance()->input.xpos;
		worldSpacePos.y = user::getInstance()->input.ypos;
		worldSpacePos.z = user::getInstance()->input.zpos;

		float scale = distance(m_translateCenter,worldSpacePos);

		incx *= (scale / 50);
		incy *= (scale / 50);

		double xrot = degreesToRadians(user::getInstance()->input.xrot);
		double yrot = degreesToRadians(user::getInstance()->input.yrot);

		m_translateCenter.x += m_selectedTranslateAxis.x * float( (cos(yrot) * incx) + (-(sin(xrot) * sin(yrot)) * incy ) );
		m_translateCenter.y -= m_selectedTranslateAxis.y * float( cos(xrot) * incy );
		m_translateCenter.z += m_selectedTranslateAxis.z * float( (sin(yrot) * incx) + ((sin(xrot) * cos(yrot)) * incy ) );

		if(m_editMode == 1)
		{
			if(m_selectedSpawnPoint != -1) spawnPoint[m_selectedSpawnPoint] = m_translateCenter;
		}
		else if(m_editMode == 2)
		{
			if(m_selectedPickup != -1) pickups[m_selectedPickup].setPos(m_translateCenter);
		}

	}

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
}

