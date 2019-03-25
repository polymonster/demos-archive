#ifndef _PLAYER_H
#define _PLAYER_H

#include "globals.h"

//#include"data/resource/model loader/md2.h"
//#include"data/resource/model loader/texture.h"

#include "myMath.h"
#include "particles.h"
#include "outputText.h"
#include "Md3.h"
#include "weapons.h"

#define SIDLE 0
#define SIDLECR 8
#define SWALKF 1
#define SWALKCR 7
#define SWALKB 2
#define SJUMP 3
#define SJUMPB 11
#define SATTACK 4
#define SHIT 5
#define SLAND 6
#define SATTACKA 9
#define SATTACKB 10

typedef struct{

	SVector3 pos;
	SVector3 lineAim;

	float yrot;
	float xrot;

	int state;
	bool shoot;

	int health;
	int gun;

	int shotLock;
	bool shotsRecieved;
	bool discard;

	int kills;
	int deaths;

	int connected;
	int letLeave;

	int magazines;
	int ammo;

}sendStruct;

typedef struct{

	bool active;
	float life;
	SVector3 position[2];
	SVector3 texCoords[2];

}SDamageIndicator;

typedef struct{

	float timer;
	bool active;
	char message[128];

}SMessageSystem;

class CPlayer{
	
private:

	SVector3 nextSpawnPoint;
	SVector3 p2p;
	//position
	float xpos,ypos,zpos; //pos
	float newx,newy,newz; //new pos to flag collisions
	float xrot,yrot; //rotation
	float dv; //forward, strafe, terminal and downward velocities

	int newState;
	int state;

	float currentTime;
	float lastTime;

	//stats
	int score;

	//equip
	int equipSlots[4];

	int m_aimMode;
	int crosshairId[4];
	int hudId[4];

	CModelMD3 character[3];
	CModelMD3 guns[6];

	float jump;
	bool m_canJump;
	bool m_canStandUp;
	float secondsTimer;

	float camx;
	float camy;
	float camz;

	int m_jumpCounter;

	float camrx,camry;

	SVector3 m_muzzlePos;
	SVector3 m_radiusVector;
	SVector3 m_crosshair;

	bool m_shot;
	int m_chair;

	sendStruct playerData;

	bool m_crouched;

	int m_loadWait;

	float timeBasedSpeed;

	float m_respawnTimer;

	float gravityBalance;

	bool gotGun[4];

	bool m_overShoulder;

	int m_kills;
	int m_deaths;

	SVector3 cursorPos;	

	SDamageIndicator m_damageIndicator[4];

	SMessageSystem m_messageArray[4];

	float m_reloadTimer;

	float m_footstepTimer;
	int m_footstepChannel;
	int m_footstepRate;

	

public:

	CPlayer();
	~CPlayer();

	CWeapons *myGun[4];

	int m_equippedGun;

	SVector3 shotline[2];
	SVector3 camline[2];
	SVector3 camCross[4];
	SVector3 playerToFloor[2];
	SVector3 playerToHead[2];

	SVector3 posVector();
	SVector3 radiusVector();

	int m_health;
	int m_armour;
	bool m_crosshairOverPlayer;

	void draw();
	void drawBB();
	void drawInterface(int offset);

	void load();
	void controlPlayer();
	void updatePos();
	void centreCamera();
	void setState(int s);
	void setCrosshair(SVector3 crosshair);
	void setStartPos(SVector3 startPos);
	void forceCamPos(SVector3 newpos);
	void shoot();
	void kill();
	void setCamPos();
	void debugvars();
	void moveVector(SVector3 direct);
	void reload();
	void melee();
	void updateViaNetwork(sendStruct player2);
	void canJump(bool jump);
	void canStandUp(bool stand);
	bool checkForHit(SVector3 line[],SVector3 *shotPos);
	bool shot();
	void increaseCharacterAnimation(CModelMD3 *pCharacter, int whichPart);
	void reloadTimer();
	void pickUp();
	void setStanding();
	void reduceHealth(int ammount);
	void timeBasedMovement();
	void assignRespawnPos(SVector3 respawnAt);
	bool moveLine(SVector3 &timeLine0,SVector3 &timeLine1,SVector3 &timeLine2);
	void forcePos(SVector3 newPos,bool x, bool y, bool z);
	void setMiniMapData(SVector3 opponent);
	SVector3 camPosVector();
	SVector3 camRotVector();
	SVector3 getReadyForCollision();
	bool run;
	void increaseHealth(int ammount);
	void increaseArmour(int ammount);
	void setPositionData();
	sendStruct getPositionData();
	void drawGun(int gun);

	void unlockShot();

	void addGunAmmo(int gun, int ammount);

	bool checkForDeath();

	bool readyToSend;

	bool lockShot;

	void awardKill(){m_kills++;};

	void updateOpponent();

	void updateBullets();

	void checkCollision(SVector3 pos, SVector3 radiusVector);

	SVector3 velocityToSpend;

	void renderMuzzleFlare();
	void renderGun();

	void get3dCursor();

	float yRotation(){return yrot;};

	void drawHealth();
	void drawMiniMap();
	void drawGunInfo();
	void drawCrosshairs();
	void drawMessages();
	void drawDamage(int side);
	void drawScore();
	void setupDamageIndicator();
	void initDamageIndicator(float shooterRotation);
	bool checkNetworkForDeath();
	void changeCharacter(int id);
	void loadProfile();
	void messageDisplay();
	void addMessage(char *message);
	void disconnect();
	void stayConnected();
	void resetGameData();
	void resetAfterDeath();
	void clearMessages();

	char m_playerName[64];
	int m_activeChar;
	bool m_alive;

	void renderCharacterChooser();
	void shadowVolumeBB();

	int kills(){return m_kills;};

	void setIdle();

	void manageFootsteps();
};

#endif