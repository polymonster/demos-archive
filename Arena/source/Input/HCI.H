#ifndef _HCI_H
#define _HCI_H
#include "globals.h"

//obtain input
void readKeyboard(unsigned char keyID, int x, int y);
void readKeyUp(unsigned char keyID, int x, int y);
void readPassiveMouse(int xpos, int ypos);
void readActiveMouse(int xpos, int ypos);
void readMouseButton(int button, int state, int xpos, int ypos);
void specialFunc(int key, int x, int y);
void specialUpFunc(int key, int x, int y);

typedef struct 
{
	int resolutionID;
	int shadowQualityID;
	int textureQualityID;

	int screenWidth;
	int screenHeight;

}SUserSettings;

class user{

public:

	SControls input;
	SUserSettings settings;

	static void create();
	static void destroy();
	static user* getInstance(){return m_instance;};

	void clearStates();

	void viewMode();
	void gameMode();
	void cursorPos();

private:

	user(){};
	~user(){};

	static user *m_instance;
};

class CHCI {	

private:


public:

	CHCI();

	void createUserSingleton();
	void updateUserSingleton();
	void resyncUserSingleton();

	//obtain input
	static void readKeyboard(unsigned char keyID, int x, int y);
	static void readKeyUp(unsigned char keyID, int x, int y);
	static void readPassiveMouse(int xpos, int ypos);
	static void readActiveMouse(int xpos, int ypos);
	static void readMouseButton(int button, int state, int xpos, int ypos);
	static void specialFunc(int key, int x, int y);
	static void specialUpFunc(int key, int x, int y);

	//control systems
	void viewMode();
	void editMode();
	void gameMode();
	void cursorPos();

	//sync
	void alignMouseToAspect();
	void setStart(float y, float yrot);

	//debug
	void outputVars();
	
};

#endif