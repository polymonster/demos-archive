#include "Game.h"
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "windowsThings.h"
#include "player.h"
#include "ModelManager.h"
#include "map.h"
#include "networking.h"
#include "soundSingleton.h"
#include "game.h"
#include "shader manager/shaderManager.h"
#include "HCI.h"

int serverSocket;
int clientSocket;

bool quit = false;

CGame::CGame()
{	
	initialiseSingletons();

	menu = new CMainMenu();

	SVector3 origin(0,0,0);

	player[0] = new CPlayer();
	player[0]->setStartPos(origin);

	player[1] = new CPlayer();
	player[1]->setStartPos(origin);

	map = new CMap(true);
	
	map->registerPlayer(player[USER_PLAYER]);
	map->registerPlayer(player[1]);

	menu->registerPlayer(player[USER_PLAYER]);
	menu->registerPlayer(player[1]);

	editorMap = new CEditorMap();

	CTextureMan::getInstance()->addParticle("data/textures/particles/myParticle.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/particle.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/smoke.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/minis.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/fire.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/part2.bmp");
	CTextureMan::getInstance()->addParticle("data/textures/particles/bulletole.bmp");

	CShaderManager::getInstance()->create();
	CShaderManager::getInstance()->loadShader("shaders/gaussian.glsl");
	CShaderManager::getInstance()->setup();
	
	CTextureMan::getInstance()->addCrosshair("data/HUD/machineGun.bmp");
	CTextureMan::getInstance()->addCrosshair("data/HUD/shotgun_crosshair.bmp");
	CTextureMan::getInstance()->addCrosshair("data/HUD/laser_crosshair.bmp");
	CTextureMan::getInstance()->addCrosshair("data/HUD/laser_crosshair.bmp");

	CTextureMan::getInstance()->addParticle("data/HUD/radar.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/damage.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/ammo_bar.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/health_indicator.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/health_bar.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/score_indicator.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/numbers.bmp");

	CTextureMan::getInstance()->addParticle("data/HUD/machine_gun_icon.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/shot_gun_icon.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/plasma_gun_icon.bmp");
	CTextureMan::getInstance()->addParticle("data/HUD/laser_gun_icon.bmp");

	m_previousLoopMode = -100;
	m_loopMode = MENU_LOOP;

	manageStateChange();

	//glutSetCursor(GLUT_CURSOR_NONE);

	startNetwork = -1;

	m_load = true;
}

void CGame::initNewGame()
{
	player[OPPONENT_PLAYER]->lockShot = false;
	player[USER_PLAYER]->lockShot = false;
	player[USER_PLAYER]->m_health = 100;

	editorMap->detachMenu();

	if(m_load) map->loadMap("data/configs/maps/warehouse.dat");
	else map->initialiseMap();

	map->setRespawn(player[USER_PLAYER],true);

	if(startNetwork == -1)
	{
		int plyr = rand()%100;

		if(plyr <= 50) plyr = 0;
		if(plyr > 50) plyr = 2;
		if(plyr > 70) plyr = 1;

		strcpy(player[OPPONENT_PLAYER]->m_playerName,"dummyPlayer");
		player[OPPONENT_PLAYER]->changeCharacter(plyr);

		map->setRespawnBoth(player[OPPONENT_PLAYER],player[USER_PLAYER],true);
	}

}

void CGame::initialiseSingletons()
{
	CTextureMan::getInstance()->create();
	CTimer::getInstance()->create();
	CModelManager::getInstance()->create();
	CSound::getInstance()->create();
}

void CGame::gameUpdateLoop()
{
	if(quit) exit(0);

	switch(m_loopMode)
	{
	case GAME_LOOP :

		mapUpdateLoop();

	break;

	case MENU_LOOP :

		menuUpdateLoop();

	break;

	case EDITOR_LOOP :

		editorUpdateLoop();

	break;

	case EXIT_GAME :

		exit(0);

	break;

	}
}

void CGame::gameRenderLoop()
{
	switch(m_loopMode)
	{
	case GAME_LOOP :

		mapRenderLoop();

	break;

	case MENU_LOOP :

		menuRenderLoop();

	break;

	case EDITOR_LOOP :

		editorRenderLoop();

	break;

	case LOAD_LOOP :

		loadingScreen();

	break;

	case EXIT_GAME :

		exit(0);

	break;
	}

}

void CGame::loadingScreen()
{
	initNewGame();
	m_load = false;
}

void CGame::manageStateChange()
{
	if(m_loopMode != m_previousLoopMode)
	{
		if(m_previousLoopMode == MENU_LOOP) CSound::getInstance()->endAtmosphere();

		switch(m_loopMode)
		{

		case GAME_LOOP :

			//m_load = true;
			glutSetCursor(GLUT_CURSOR_NONE);
			initNewGame();

		break;

		case MENU_LOOP :

			//going into menu
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			closeSockets();
			CSound::getInstance()->startMenuMusic();

		break;

		case EDITOR_LOOP :

			//going into editor
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			editorMap->createMenu();

		break;

		case NETWORK_JOIN :

			if(startNetwork != 1)
			{
				closeSockets();
				startNetwork = 1;
				m_loopMode = MENU_LOOP;
				menu->addToNetworkPlayerList(player[USER_PLAYER]);
			}
			else
			{
				closeSockets();
				menu->clearPlayerList();
				m_loopMode = MENU_LOOP;
				menu->addMessage("Disconnected from host",menu->computerName);
			}

		break;

		case NETWORK_HOST :

			closeSockets();
			startNetwork = 0;
			m_loopMode = MENU_LOOP;
			menu->addToNetworkPlayerList(player[USER_PLAYER]);

		
		break;

		case NETWORK_GAME_START :

			//m_load = true;
			initNewGame();

		break;

		case EXIT_GAME :

			//quitting
			//CTextureMan::getInstance()->freeAll();
			//CSound::getInstance()->kill();
			//closeSockets();
			exit(0);

		break;
		}

	}

	m_previousLoopMode = m_loopMode;
}

void CGame::mapRenderLoop()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspectRatio, 1.0f, 300000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	glClearDepth(1);
	//glClearColor (0.6,0.6,0.6,1);
	glClearColor (1,0,1,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	map->renderPasses();
	
	if(!map->inStats) player[USER_PLAYER]->drawInterface(0);
	else 
	{
		map->endStats();
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		glDisable(GL_BLEND);
	}

	glutSwapBuffers();
}

void CGame::mapUpdateLoop()
{
	m_loopMode = map->callback();
	manageStateChange();

	bool reduceP2 = false;
	if(startNetwork == -1) reduceP2 = true;

	if(map->inStats)
	{
		user::getInstance()->viewMode();	
	}
	else
	{
		user::getInstance()->gameMode();

		player[USER_PLAYER]->controlPlayer();

		if(player[OPPONENT_PLAYER]->lockShot)
		{
			player[OPPONENT_PLAYER]->shoot();
			player[OPPONENT_PLAYER]->lockShot = false;
		}

		if(reduceP2) 
		{
			if(player[OPPONENT_PLAYER]->checkForDeath()) 
			{
				player[USER_PLAYER]->awardKill();
				char message[128];

				strcpy(message,"You Killed ");
				strcat(message, player[OPPONENT_PLAYER]->m_playerName);

				player[USER_PLAYER]->addMessage(message);

				map->setRespawn(player[OPPONENT_PLAYER],false);
			}
		}
		else
		{
			if(player[OPPONENT_PLAYER]->checkNetworkForDeath()) 
			{
				player[USER_PLAYER]->awardKill();
				char message[128];

				strcpy(message,"You Killed ");
				strcat(message, player[OPPONENT_PLAYER]->m_playerName);

				player[USER_PLAYER]->addMessage(message);
			}
		}
		
		if(player[USER_PLAYER]->checkForDeath())
		{
			char message[128];

			strcpy(message, player[OPPONENT_PLAYER]->m_playerName);
			strcat(message," Killed You");

			map->setRespawn(player[USER_PLAYER],false);

			player[USER_PLAYER]->addMessage(message);

		}

		map->checkCollision(player[USER_PLAYER]);

		map->collisionHandler(player[USER_PLAYER]);
		map->collisionHandler(player[OPPONENT_PLAYER]);

		map->bulletVsPlayer(player[USER_PLAYER],player[OPPONENT_PLAYER],reduceP2);

		map->bulletVsPlayer(player[OPPONENT_PLAYER],player[USER_PLAYER],true);

		map->playerVsPlayer(player[USER_PLAYER],player[OPPONENT_PLAYER]);

		player[USER_PLAYER]->setMiniMapData(player[OPPONENT_PLAYER]->posVector());
	}

	if(startNetwork == -1) map->disconnectCompleted();

}

void CGame::menuRenderLoop()
{
	if(m_load) 
	{
		loadingScreen();
		return;
	}

	user::getInstance()->viewMode();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspectRatio, 1.0f, 300000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	glClearDepth(1);
	//glClearColor (0.6,0.6,0.6,1);
	glClearColor (1,0,1,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	OrthoMode(0,0,xScreenSize,yScreenSize);

	menu->draw();

	glutSwapBuffers();
};

void CGame::menuUpdateLoop()
{
	menu->check();

	m_loopMode = menu->callback();
	manageStateChange();
};

void CGame::editorCamera()
{
	float x = user::getInstance()->input.xpos;
	float y = user::getInstance()->input.ypos;
	float z = user::getInstance()->input.zpos;

	float xrot = user::getInstance()->input.xrot;
	float yrot = user::getInstance()->input.yrot;

	glRotated(xrot,1,0,0);
	glRotated(yrot,0,1,0);
	glTranslated(-x,-y,-z);

}

void CGame::editorUpdateLoop()
{
	m_loopMode = editorMap->callback();

	manageStateChange();

	editorMap->build();
}

void CGame::editorRenderLoop()
{
	user::getInstance()->viewMode();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspectRatio, 1.0f, 300000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear the buffers before drawing
	glClearDepth(1);
	glClearColor (1,0,1,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	editorCamera();
	editorMap->widgetDepthTests();

	editorCamera();

	glDisable(GL_BLEND);
		
	editorMap->renderScene();

	glEnable(GL_LIGHTING);

	editorMap->drawPickups();

	glutSwapBuffers();
}

void CGame::revertSettingsMessage()
{
	menu->addMessage("Your new settings could not be applied","go to settings to select a new configuration");
}