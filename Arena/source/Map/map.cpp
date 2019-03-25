#include "map.h"
#include "modelManager.h"
#include "shader manager/shaderManager.h"
#include "timerSingleton.h"
#include "soundSingleton.h"
#include "HCI.h"

int blurCounter = 0;
float light0_position[] = {400,400,500,1.0};
int blurCount = 0;

void CMap::loadingMessage(int progress)
{
	loaded = false;

	COutput loadingProgress;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspectRatio, 1.0f, 300000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	glClearDepth(1);
	glClearColor (0,0,0,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	OrthoMode(0,0,xScreenSize,yScreenSize);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);


	glColor4f(0,0,0,1);

	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(xScreenSize,0);
		glVertex2f(xScreenSize,yScreenSize);
		glVertex2f(0,yScreenSize);
	glEnd();

	glColor4f(0.7,0.7,0.7,1);

	int ytext = yScreenSize / 2 - 100;
	loadingProgress.output(50,ytext,"Loading Game.....","",18);

	if(progress > 0) loadingProgress.output(50,ytext += 20,"Cleaning up","");
	if(progress >= 1) loadingProgress.output(50,ytext += 20,"Opening map file","");
	if(progress >= 3) loadingProgress.output(50,ytext += 20,"Loading map mesh","");
	if(progress >= 4) loadingProgress.output(50,ytext += 20,"Loading floor mesh","");
	if(progress >= 5) loadingProgress.output(50,ytext += 20,"Loading collision mesh","");
	if(progress >= 6) loadingProgress.output(50,ytext += 20,"Loading bullet mesh","");
	if(progress >= 7) loadingProgress.output(50,ytext += 20,"Adding map items","");
	if(progress >= 8) loadingProgress.output(50,ytext += 20,"Building display lists","");
	if(progress >= 9) loadingProgress.output(50,ytext += 20,"Complete.......","");

	glutSwapBuffers();

	loaded = true;

}

void CMap::loadMap(char *filename)
{
	loaded = false;

	loadingMessage(0);

	m_numberOfSpawnPoints = 0;
	m_numberOfPickups = 0;

	ifstream fin;

	fin.open(filename);

	string word;
	char file[64];

	fin >> word;

	loadingMessage(1);

	if(word != "arena_map_file") 
	{
		cout << "\nError : Not a valid map file";
		fin.close();
	}

	loadingMessage(2);

	while(!fin.eof())
	{
		fin >> word;

		if(word == "render_mesh")
		{
			fin >> file;
			loadingMessage(3);
			m_meshID[RENDER_MESH] = CModelManager::getInstance()->load(file);
		}
		else if(word == "collision_offset")
		{
			fin >> file;
			loadingMessage(4);
			m_meshID[COLLISION_OFFSET] = CModelManager::getInstance()->load(file);

		}
		else if(word == "collision_blocking")
		{
			fin >> file;
			loadingMessage(5);
			m_meshID[COLLISION_BLOCKING] = CModelManager::getInstance()->load(file);
		}
		else if(word == "collision_bullets")
		{
			fin >> file;
			loadingMessage(6);
			m_meshID[COLLISION_BULLETS] = CModelManager::getInstance()->load(file);
		}
		else if(word == "spawn_point")
		{
			fin >> spawnPoint[m_numberOfSpawnPoints].x >> 
				spawnPoint[m_numberOfSpawnPoints].y >> 
				spawnPoint[m_numberOfSpawnPoints].z;

			m_numberOfSpawnPoints++;
		}
		else if(word == "pickup_node")
		{
			SVector3 pickupPos;
			int type;

			fin >> pickupPos.x >> 
				pickupPos.y >> 
				pickupPos.z >> 
				type;

			pickups[m_numberOfPickups].setPos(pickupPos);
			pickups[m_numberOfPickups].setType(type);
			
			m_numberOfPickups++;
		}

	}

	fin.close();

	loadingMessage(7);


	float light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	float light0_ambient[] = {1.0, 1.0, 1.0, 1.0};
	float light0_specular[] ={1.0, 1.0, 1.0, 1.0};

	float light1_diffuse[] = {0.4, 0.2, 0.2, 0.5};
	float light1_ambient[] = {0.2, 0.3, 0.2, 0.5};
	float light1_specular[] ={1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	buildDisplayList();

	loadingMessage(8);

	CTimer::getInstance()->startGameTimer(2);
	inStats = false;
	m_numberOfBulletHoles = 0;

	user::getInstance()->input.leftMouse = false;

	//CSound::getInstance()->startAtmosphere();

	loadingMessage(9);

	m_player[0]->clearMessages();

	m_disconnectWish = false;
	m_disconnectCompleted = false;
	m_menuReturnWish = false;
}

void CMap::initialiseMap()
{
	CTimer::getInstance()->startGameTimer(2);

	inStats = false;
	m_numberOfBulletHoles = 0;

	user::getInstance()->input.leftMouse = false;

	CSound::getInstance()->startAtmosphere();

	loadingMessage(9);

	m_player[0]->clearMessages();

	m_disconnectWish = false;
	m_disconnectCompleted = false;
	m_menuReturnWish = false;
}

CMap::CMap(){};

CMap::CMap(bool load)
{
	if(load)
	{

		weaponPickups[0] = new CWeapons("data/configs/weapons/machinegun.txt");
		weaponPickups[1] = new CWeapons("data/configs/weapons/shotgun.txt");
		weaponPickups[2] = new CWeapons("data/configs/weapons/plasma.txt");
		weaponPickups[3] = new CWeapons("data/configs/weapons/lazer.txt");

		for(int i = 0; i < NUMBER_OF_MESHES; i++)
		{
			m_meshID[i] = -1;
		}

		m_numberOfPlayers = 0;
		m_numberOfBulletHoles = 0;

		if(user::getInstance()->settings.shadowQualityID == 0)
		{
			fboSize = 1024;
			shadowDepthFBO = new CFBO(1,512);
			shadowTextureFBO = new CFBO(0,fboSize);
			gaussFBO[0] = new CFBO(0,fboSize);
			gaussFBO[1] = new CFBO(0,fboSize);
		}
		else if(user::getInstance()->settings.shadowQualityID == 1)
		{
			fboSize = 512;
			shadowDepthFBO = new CFBO(1,1024);
			shadowTextureFBO = new CFBO(0,fboSize);
			gaussFBO[0] = new CFBO(0,fboSize);
			gaussFBO[1] = new CFBO(0,fboSize);
		}

		//Calculate & save matrices
		glPushMatrix();
		
		glLoadIdentity();
		gluPerspective(120.0f, 1.0f, 30.0f, 4000.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
		
		glLoadIdentity();
		glRotated(20,1,0,0);
		glTranslated(-100,-450,-900);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
		
		glPopMatrix();

		m_callback = GAME_LOOP;

		m_formElement[0] = new CFormElement("close",xScreenSize / 2 - 50,yScreenSize / 2 + 40,100,50,0,
			CTextureMan::getInstance()->addMenuTexture("data/menu/menu_return.bmp"));

		//leave this for when i add more maps so the game can continue
		/*m_formElement[1] = new CFormElement("again",xScreenSize / 2 + 50,yScreenSize / 2 + 40,100,50,0,
			CTextureMan::getInstance()->addMenuTexture("data/menu/play_again.bmp"));*/

		m_disconnectWish = false;
		m_disconnectCompleted = false;
		m_menuReturnWish = false;

	}

}

CMap::~CMap(){};

void CMap::registerPlayer(CPlayer *player)
{
	if(m_numberOfPlayers > 3) return;

	m_player[m_numberOfPlayers++] = player;
}


void CMap::buildDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList,GL_COMPILE);

	int meshID = m_meshID[RENDER_MESH];

	if(meshID != -1)
	{
		glPushMatrix();

		glRotated(-90,1,0,0);

		CModelManager::getInstance()->render(meshID);

		glPopMatrix();
	}

	glEndList();
}

void CMap::renderBulletHoles()
{
	glColor4f(1,1,1,1);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
	CTextureMan::getInstance()->bindParticle(6);   //bullet hole texture

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	for(int i = 0; i < MAX_BULLET_HOLES; i++)
	{
		if(bulletHole[i].active)
		{
			glPushMatrix();

				glTranslated(bulletHole[i].position.x,bulletHole[i].position.y,bulletHole[i].position.z);
				
				glRotated(bulletHole[i].rotation.y,0,1,0);
				glRotated(bulletHole[i].rotation.x,1,0,0);
				
				glTranslated(0,0,0.25);
				
				glBegin(GL_QUADS);
					glMultiTexCoord2f(GL_TEXTURE1_ARB,1,1); 
					glVertex3f(-3,-3,0);
					glMultiTexCoord2f(GL_TEXTURE1_ARB,1,0); 
					glVertex3f(3,-3,0);
					glMultiTexCoord2f(GL_TEXTURE1_ARB,0,0); 
					glVertex3f(3,3,0);
					glMultiTexCoord2f(GL_TEXTURE1_ARB,0,1); 
					glVertex3f(-3,3,0);
				glEnd();
			
			glPopMatrix();

		}
	}

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(i = 0 ; i < 10; i++)
	{
		if(particleSystem[i].inUse)
		{
			particleSystem[i].ricochetParticles();
			particleSystem[i].updateRicochet();
		}

		if(bloodParticles[i].inUse)
		{
			bloodParticles[i].bloodParticles();
			bloodParticles[i].updateBlood();
		}
	}


}

void CMap::renderPasses()
{
	//SHADOW PASSES
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);

	if((user::getInstance()->settings.shadowQualityID == 0)
	  || (user::getInstance()->settings.shadowQualityID == 1))
	{
		shadowPass();
		shadowMapBlur();
		renderShadowStencil();

		darkenedScene();
		litScene();
	}
	else
	{
		m_player[0]->setCamPos();
		renderScene();
	}

	for(int i = 0; i < m_numberOfPlayers; i++)
	{
		m_player[i]->draw();
	}

	m_player[0]->get3dCursor();

	renderBulletHoles();

	for(i = 0; i < m_numberOfPickups; i++)
	{
		pickups[i].render(weaponPickups);
	}

	for(i = 0; i < m_numberOfPlayers; i++)
	{
		m_player[i]->renderGun();

		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		m_player[i]->renderMuzzleFlare();

		glPopMatrix();	
	}

	//motionBlur();

}


void CMap::renderScene()
{	
	glEnable(GL_CULL_FACE);
	glCallList(displayList);
}

void CMap::motionBlur()
{

	if(blurCount <= 0)
	{
		//POST_PROCESSING
		fbo[0]->bindFBO();

		glClearColor(0,0,0,0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_player[0]->setCamPos();

			for(int i = 0; i < m_numberOfPlayers; i++)
			{
				m_player[i]->draw();

			}

		glPushMatrix();
		glColor4f(0,0,0,0.5);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		fbo[1]->bindAsTexture();
		OrthoMode(0,0,1024,1024);
		motionBlurQuad(1024,1024);
		PerspectiveMode();
		glPopMatrix();

		fbo[0]->unbindFBO();

		fbo[1]->bindFBO();

		glClearColor(0,0,0,0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glColor4f(0,0,0,1);
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		fbo[0]->bindAsTexture();
		OrthoMode(0,0,xScreenSize,yScreenSize);
		motionBlurQuad(xScreenSize,yScreenSize);

		fbo[1]->unbindFBO();

		blurCount = 3;
	}
	else
	{
		blurCount--;
	}



	glViewport(0,0,xScreenSize,yScreenSize);

	glEnable(GL_BLEND);
	glAlphaFunc(GL_ONE,GL_ONE);
	glColor4f(1,1,1,0.7);	
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	fbo[0]->bindAsTexture();
	OrthoMode(0,0,xScreenSize,yScreenSize);
	motionBlurQuad(xScreenSize,yScreenSize);
}

void CMap::motionBlurQuad(int x, int y)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE);
	glDisable(GL_LIGHTING);
	glColor4f(1,1,1,0.7);

	glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,0,1); glVertex2f(0,0);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,0,0); glVertex2f(0,y);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,1,0); glVertex2f(x,y);
		glMultiTexCoord2f(GL_TEXTURE1_ARB,1,1); glVertex2f(x,0);
	glEnd();
	
	//PerspectiveMode();

}

int CMap::callback()
{
	if(!CTimer::getInstance()->m_timerActive) 
	{
		for(int i = 0; i < m_numberOfPlayers; i++)
		{
			if(!inStats) m_player[i]->setIdle();

		}

		inStats = true;
	}

	if(inStats)
	{
		for(int i = 0; i < 1; i++)
		{
			if(m_formElement[i]->callBack() == SELECTED)
			{

				if(m_formElement[i]->m_callbackID == "close")
				{
					m_formElement[i]->m_state = 0;
					m_formElement[i]->m_selectedState = false;

					m_disconnectWish = true;
					m_disconnectCompleted = false;
					m_menuReturnWish = true;

					m_player[0]->disconnect();

				}
				/*else if(m_formElement[i]->m_callbackID == "again")
				{
					m_formElement[i]->m_state = 0;
					m_formElement[i]->m_selectedState = false;

					inStats = false;

					m_player[0]->resetGameData();
					m_player[1]->resetGameData();
					CTimer::getInstance()->m_timerActive = true;
					CTimer::getInstance()->startGameTimer(1);
					resetMap();
					//respawn load another map etc
				}*/
			}
		}
	}

	if(user::getInstance()->input.key[27])
	{
		m_disconnectWish = true;
		m_disconnectCompleted = false;
		m_menuReturnWish = true;

		m_player[0]->disconnect();
	}

	if(returnToMenu()) 
	{
		return MENU_LOOP;
	}

	return m_callback;
}

void CMap::resetMap()
{
	for(int i = 0; i < MAX_BULLET_HOLES; i++)
	{
		bulletHole[i].active = false;
	}

	for(i = 0; i < 4; i++)
	{
		pickups[i].m_alive = true;
	}

	inStats = false;

	m_disconnectWish = false;
	m_disconnectCompleted = false;
	m_menuReturnWish = false;
}

void CMap::deleteData()
{
	CModelManager::getInstance()->destroyModel(m_meshID[RENDER_MESH]);
	CModelManager::getInstance()->destroyModel(m_meshID[COLLISION_OFFSET]);
	CModelManager::getInstance()->destroyModel(m_meshID[COLLISION_BLOCKING]);
	CModelManager::getInstance()->destroyModel(m_meshID[COLLISION_BULLETS]);

	m_meshID[COLLISION_BLOCKING] = -1;
	m_meshID[COLLISION_OFFSET] = -1;
	m_meshID[RENDER_MESH] = -1;
	m_meshID[COLLISION_BULLETS] = -1;

	CSound::getInstance()->endAtmosphere();
}

void CMap::setRespawn(CPlayer *player, bool setImmedietly)
{
	unsigned int seed = CTimer::getInstance()->animBaseSpeed() * 1000;
	srand ( seed );
	int point = rand()%m_numberOfSpawnPoints;

	cout << point;
	
	if(setImmedietly) player->setStartPos(spawnPoint[point]);
	else player->assignRespawnPos(spawnPoint[point]);
}

void CMap::setRespawnBoth(CPlayer *player1,CPlayer *player2, bool setImmedietly)
{
	unsigned int seed = CTimer::getInstance()->animBaseSpeed() * 1000;
	srand ( seed );
	int point = rand()%m_numberOfSpawnPoints;

	cout << point;
	
	if(setImmedietly) player1->setStartPos(spawnPoint[point]);
	else player1->assignRespawnPos(spawnPoint[point]);

	point++;

	if(point >= m_numberOfSpawnPoints) point = 0;
	
	if(setImmedietly) player2->setStartPos(spawnPoint[point]);
	else player2->assignRespawnPos(spawnPoint[point]);

}

bool CMap::disconnectWish()
{
	return m_disconnectWish;
}

void CMap::disconnectCompleted()
{
	m_disconnectCompleted = true;
}

bool CMap::returnToMenu()
{
	bool okToReturn = false;
	//if the user wants to go back to the menu
	if(m_menuReturnWish)
	{
		//if they were connected they must disconect cleanly first
		if(m_disconnectWish)
		{
			//if the connection is cleanly quit
			if(m_disconnectCompleted)
			{
				m_disconnectWish = false;
				okToReturn = true;
			}
		}
		else
		{
			okToReturn = true;
		}
	}

	if(okToReturn)
	{
		m_player[0]->resetGameData();
		m_player[1]->resetGameData();
		inStats = false;
		CTimer::getInstance()->m_timerActive = true;
		resetMap();
		//deleteData();

		return true;

	}

	return false;
}