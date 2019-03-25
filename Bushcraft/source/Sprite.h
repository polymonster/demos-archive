#ifndef INC_SPRITE_H
#define INC_SPRITE_H
#define UNSET		0
#define SET			1
#define CAUGHT		3
#define FAIL		2

#include <allegro.h>
#include "Background.h"

class CSprite {
protected:
	
	BITMAP*** m_spriteImage;
	int* m_totalAnimationframes;
	int* m_spriteWidths;
	int* m_spriteHeights;
	int m_xpos, m_ypos;
	int m_currentAnimationframe;
	int m_currentState;
	int m_numberofState;
	bool m_binitialised;
	double m_dx, m_dy;
	bool m_bdisplaytext;
	char m_text[256];
	void displayTextPopup(BITMAP* buffer, CBackground *bkground);
	int m_lifespan;
	bool m_alive;
	int m_speedx, m_speedy;
	int m_numloot;
	int m_newstate;
	int m_roamtimer;
	int timefluctuator;
	int m_health;
	bool m_meat;
	bool m_bone;
	bool m_sinew;
	int m_gazLoot;
	bool m_lootgazell;
	bool m_treeloot1;
	bool m_treeloot2;
	bool m_treeloot3;
	bool m_treeloot4;
	bool m_looted;
	int	m_treeLoot;
	double m_cooktimer;
	bool m_cooking;
	int m_returnTimer;
	int m_deactiveM;
	int m_deactiveH;
	bool m_trapHasCaught;
	int m_trapLoot;
	int m_runtimer;
	int m_lootitem[6];
	char m_spriteName[25];

	int colour_white;
	int colour_grey;
	int colour_yellow;
	int colour_green;
	int colour_dkred;
	int colour_blue;


public:
	CSprite();
	~CSprite();

	bool initialise(const char filename[]);
	void drawSprite(BITMAP* buffer,CBackground *bkgrond);
	int setSpriteState(int state);
	int getSpriteState() { return m_currentState; };
	void setGazellHealth(int health);
	int getGazellHealth() { return m_health; };
	int getGazellLoot() { return m_gazLoot; };
	int getTreeLoot() { return m_treeLoot; };
	double cooktime() { return m_cooktimer; };
	bool gazlooted() { return m_lootgazell; };
	bool treelooted() { return m_looted; };
	void incrementAnimationframe();
	void setpos(int xpos, int ypos);
	void getpos(int* xpos, int* ypos);
	void setRoam(int roamtimer);
	bool checkCollisionwithBackground(CBackground *bkground, int xpos, int ypos);
	bool checkCollisionwithSprite(CSprite *sprite, int xpos, int ypos);
	bool hasCollidedwithSprite(CSprite* sprite);
	bool getVertices(int* xvert, int *yvert);
	void drawGazellInfo(BITMAP *buffer);
	void switchstates();
	void setPopupText(const char* text);
	void togglePopupText();
	void spawn(int state, int lifespan, int xpos, int ypos, int speedx, int speedy);
	void updatepos();
	void despawn();
	void GazellRoam();
	bool isAlive() { return m_alive; };
	void GazellOutsideBound(int backgroundwidth, int backgroundheight);
	void cookingFire(BITMAP *buffer,BITMAP *cookingbg, int fx, int fy, bool cooking);
	void getspeed(int* xspeed, int* yspeed);
	void setSpeed(int xpseed, int yspeed);
	void startCooking(int timer);

	void drawStaticInfo(BITMAP *buffer, int mode);

	int timerReturn() {return m_returnTimer; };
	void timerCount();

	void SetTrap(int hours, int minutes, int traplvl);
	void CheckTrapDeactive(int hours, int minutes, BITMAP *buffer, int *state);
	void LootTrap(BITMAP *buffer, BITMAP *lootTable, BITMAP *rabbit,int mousex, int mousey, bool spaceAvail, int trapType);
	int trapLootReturn() {return m_trapLoot;};

	void drawTrapInfo(BITMAP *buffer,int trapType);

	void setLifeSpan(int lifespan);

	void detectPlayer(int animaltype, int playerx, int playery, int playerStealthMode);
	void runTimerCount();

	void runAway();

	void setLootObjects(int loot1, int loot2, int loot3, int loot4, int loot5, int loot6);

	void getLootObjects(int *loot1, int *loot2, int *loot3, int *loot4, int *loot5, int *loot6);


};

#endif /* INC_SPRITE_H */
	