#ifndef _MAP_H
#define _MAP_H

#include "globals.h"
#include "player.h"
#include "FBO.h"
#include "weapons.h"
#include "GLee/tempMath/maths.h"
#include "pickUpNode.h"
#include "particles.h"
#include "weapons.h"
#include "menu.h"

typedef enum{

	RENDER_MESH = 0,
	COLLISION_BLOCKING,
	COLLISION_OFFSET,
	COLLISION_BULLETS,

}eMeshes;

typedef enum{

	NONE = 0,
	PICKUP,
	SPAWNPOINT,
	LIGHTPOS,
	COLLISION_VIEW,

}eEditModes;

typedef enum{

	MGUN = 0,
	SGUN,
	LGUN,
	PGUN,
	SPAWNP,
	DELETE_EDIT,
	PICKMODE,
	SPAWNMODE,

}eEditOptions;

#define NUMBER_OF_MESHES 5
#define MAX_BULLET_HOLES 50
#define MAX_SPAWNS 6
#define MAX_PICKUPS 6

typedef struct
{
	bool active;
	SVector3 position;
	int type;
	SVector3 normal;
	float size;
	SVector3 poly[4];
	SVector3 rotation;

}SBulletHole;

class CMap{

public :

	CMap();
	CMap(bool load);
	~CMap();

	void loadMap(char *filename);
	void loadingMessage(int progress);
	void deleteMap();
	virtual int callback();
	void resetMap();
	void initialiseMap();

	void buildDisplayList();
	
	void renderPasses();
	virtual void renderScene();
	void shadowPass();
	void darkenedScene();
	void litScene();
	void renderShadowStencil();
	void renderBulletHoles();

	void buildTextureMatrix();
	void shadowMapBlur();

	void motionBlur();
	void motionBlurQuad(int x, int y);

	void collisionHandler(CPlayer *player);
	void checkCollision(CPlayer *player);
	void bulletCollision(SBullet *bullet);
	void bulletVsPlayer(CPlayer *collider, CPlayer *collidee,bool reduce);
	void playerVsPlayer(CPlayer *collider, CPlayer *collidee);
	void newBulletHole(SVector3 position, STriangle collisionTri, SVector3 polysNormal, int bulletType);

	void setRespawn(CPlayer *player, bool setImmedietly);
	void setRespawnBoth(CPlayer *player1,CPlayer *player2, bool setImmedietly);

	void deleteData();

	void registerPlayer(CPlayer *player);

	void endStats();

	bool disconnectWish();
	void disconnectCompleted();
	bool returnToMenu();

	CFormElement *m_formElement[2];

	bool inStats;
	bool loaded;

protected :

	int m_meshID[NUMBER_OF_MESHES];

	CPlayer *m_player[4];
	int m_numberOfPlayers;

	unsigned int displayList;

	SVector3 lightPos;

	SVector3 spawnPoint[MAX_SPAWNS];
	int m_numberOfSpawnPoints;


	CFBO *fbo[2];
	CFBO *shadowDepthFBO;
	CFBO *shadowTextureFBO;

	CFBO *gaussFBO[2];
	int fboSize;

	SBulletHole bulletHole[MAX_BULLET_HOLES];
	int m_numberOfBulletHoles;

	MATRIX4X4 lightProjectionMatrix, lightViewMatrix;

	CPNode pickups[MAX_PICKUPS];

	CParticles particleSystem[10];
	CParticles bloodParticles[10];

	int m_numberOfPickups;
	int m_callback;

	bool m_disconnectWish;
	bool m_disconnectCompleted;
	bool m_menuReturnWish;

	CWeapons *weaponPickups[4];
};

class CEditorMap : public CMap
{
	public:

	CEditorMap();

	void renderScene();
	void translateWidget();
	
	void saveMap();

	void createMenu();

	void build();

	void addSpawnPoint();
	void drawSpawnPoints();

	void addPickup(int type);
	void drawPickups();

	void widgetDepthTests();

	void detachMenu();

	private:

	int m_selectedSpawnPoint;
	int m_selectedPickup;
	int m_editMode;

	SVector3 m_selectedTranslateAxis;
	SVector3 m_translateCenter;

};

#endif