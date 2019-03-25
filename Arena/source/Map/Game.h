#ifndef _GAME_H
#define _GAME_H

#include "globals.h"
#include "map.h"
#include "player.h"
#include "menu.h"

#define USER_PLAYER 0
#define OPPONENT_PLAYER 1

class CGame
{

public:

	CGame();
	~CGame();
	void initialiseSingletons();

	void gameUpdateLoop();
	void gameRenderLoop();

	void mapUpdateLoop();
	void mapRenderLoop();

	void menuUpdateLoop();
	void menuRenderLoop();

	void editorUpdateLoop();
	void editorRenderLoop();
	void editorCamera();

	void manageStateChange();

	//networking
	void networkChoice();
	void hostThread();
	void clientThread();
	void closeSockets();
	bool leaveGame();
	bool checkLeave();

	int startNetwork;

	void initNewGame();

	void revertSettingsMessage();

	void loadingScreen();

	bool m_load;

private:

	CMap *map;
	CEditorMap *editorMap;
	CPlayer *player[2];
	CMainMenu *menu;

	int m_loopMode;
	int m_previousLoopMode;

	//networking
	int networkMode;

};

#endif