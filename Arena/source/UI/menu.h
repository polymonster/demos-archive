#ifndef _MENU_H
#define _MENU_H

#include "globals.h"
#include "player.h"


typedef enum
{
	BUTTON = 0,
	EDIT_BOX,
	CHECK_BOX,
	DROPDOWN_BOX,
	GROUP_BUTTON,

}eElementTypes;

typedef enum
{
	BLANK = 0,
	MOUSE_OVER,
	SELECTED,
	GROUP_SELECTED,
	DROPDOWN,

}eStates;

typedef enum
{
	MENU_LOOP = 0,
	GAME_LOOP,
	EDITOR_LOOP,
	LOAD_LOOP,
	EXIT_GAME,
	NETWORK_JOIN,
	NETWORK_HOST,
	NETWORK_SUB = 10,
	PROFILE_SUB,
	SETTINGS_SUB,
	CREDITS_SUB,
	NULL_SUB,
	WRITE_SETTINGS,
	NETWORK_GAME_START,
	CONTROLS_SUB,
	WAIT_FOR_PLAYERS_MESSAGE,
	SETTINGS_APPLIED_MESSAGE,
	PROFILE_APPLIED_MESSAGE,
	SETTINGS_REVERT_MESSAGE,
	CLOSE_MESSAGE,

}eLoopModes;

typedef struct
{
	SVector2 topRight;
	SVector2 topLeft;
	SVector2 bottomRight;
	SVector2 bottomLeft;

}SElementRect;

class CFormElement
{
	public :

	SElementRect m_rect;
	int m_type;
	int m_state;
	bool m_selectedState;
	int m_iconID;
	string m_callbackID; 
	char *m_contents;
	int m_chosenItem;

	CFormElement(){};
	CFormElement(string callbackID,int x, int y, int width, int height, bool center, int iconID);
	~CFormElement(){};

	bool m_active;

	virtual void draw();
	virtual bool check();
	virtual void addItem(const char *name);
	virtual void setInitialValue(int id);
	void setText(char *text);

	int callBack();

	bool dontCheck;

	protected :

	void calculateRect(int x, int y, int width, int height, bool center);
};

class CStaticText : public CFormElement
{
	public :

	CStaticText(string callbackID,int x, int y, int width, int height, bool center, int iconID);

	void draw();
	void setText(char *text);
	bool CStaticText::check(){	return false; };
};

class CNormalButton
{
	public :

	void draw();
	void check();
};

class CStaticBox
{
	public :

	void draw();
};

class CGroupButton
{
	public :

	int m_groupID;

	void draw();
	void check();
	void deselectGroup();
};

class CEditBox : public CFormElement
{
	public :

	CEditBox(string callbackID,int x, int y, int width, int height, bool center,int arraySize,int iconID);
	~CEditBox();

	int m_characterPos;
	int m_nextCharacter;
	int m_arraySize;
	
	void draw();
	bool check();

	protected :

	void getUserInput();
};

class CDropDownBox : public CFormElement
{
	public :

	int m_boxID;
	int m_buttonID;
	int m_dropID;

	char* menuItems[10];
	int m_numberOfItems;
	int m_mouseOverItem;

	void addItem(const char *name);
	void draw();
	bool check();
	void setInitialValue(int id);

	CDropDownBox(string callbackID,int x, int y, int width, int height, bool center,int arraySize,int bgId, int buttonId, int dropId);

};


class CMenu
{
	public :

	CFormElement **m_element;

	int m_numberOfElements;
	int m_activeGameMode;

	char *computerName;

	bool active;

	CMenu();
	CMenu(int menuType);
	~CMenu();

	virtual void draw();
	virtual void check();
	virtual int callback();

	int m_menuMode;

	void elementProc();
	void outputCursorPos();

	void setActiveStates();
};

class CProfileEditor : public CMenu
{

public:

	CProfileEditor(CPlayer *player);
	~CProfileEditor();

	char *m_playerNameBox;
	void renderModel(CPlayer *player);
	int m_currentCharacter;

	int callback();
};

class CMainMenu
{
public :

	CPlayer *m_player;

	CMainMenu();
	~CMainMenu();

	bool messageDisplay;

	bool networkConnected;
	bool inNetworkLobby;

	int m_activeSubMenu;
	int m_activeGameMode;
	int m_numberOfSubMenus;

	char *computerName;

	virtual void draw();
	virtual void check();
	void registerPlayer(CPlayer *player);
	int callback();
	void addMessage(char *messageLine1, char *messageLine2);

	void setNetworkHostName(const char *name);
	void setPlayerData(const char *playerName);

	void addToNetworkPlayerList(CPlayer *player);
	void clearPlayerList();

	void drawCursor();

	CMenu *mainBar;
	CProfileEditor *m_profileMenu;
	CMenu *networkMenu;
	CMenu *settingsMenu;
	CMenu *creditsMenu;
	CMenu *controlsMenu;
	CMenu *messageBox;

	void setInLobby(bool b); 
};



#endif