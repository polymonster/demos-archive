#ifndef _WEAPONS_H
#define _WEAPONS_H

#include "globals.h"
#include "myMath.h"
#include "Md3.h"

#define MAX_BULLETS 20
#define MAX_MAGAZINES 25

typedef struct{

	bool alive;
	SVector3 line[2];
	SVector3 shotVector;
	float life;
	int type;

}SBullet;

class CWeapons{

private:
	
	int m_muzzleSpriteId;
	//defines how the bullets will be shot, shotgun, machine gun plasma ect
	int m_shotType;

	//weapon characteristics
	int m_bulletType; //visual style of the bullet
	int m_ricochetType; //visual style of the ricochet

	int m_maxMagazines; //max number of clips
	int m_magazineSize; // clip size

	int m_range; //range of the weapon
	bool m_scope; //has a scope?
	float m_recoil; //amount of recoil
	int m_maximumDamage; //max damage in 1 hit
	
	//timer based things
	float m_fireRate;
	float m_reloadRate;

	float m_fireTimer;
	float m_reloadTimer;

	float m_bulletSpeed;

	float currentTime;
	float lastTime;

	CModelMD3 gunModel;

	float timeBasedSpeed;
	float timeBasedLast;

	float m_flashTimer;

public:

	CWeapons();
	CWeapons(const char *weaponData);

	void addAmmo(int clips);
	void timer();
	bool shoot(SVector3 shotline[],bool forceShot);

	void update();
	void draw();

	void linkGunModel(CModelMD3 &character);

	int bulletType(){return m_bulletType;};
	int ricochetType(){return m_ricochetType;};
	int returnDamage(){return m_maximumDamage;};

	void displayAmmo();

	bool pickUp();

	void muzzleFlare(float muzzleMatrix[]);

	SBullet bullets[MAX_BULLETS];

	//current weapon status
	int m_currentMagazines;
	int m_currentAmmo;

	int m_activeMagazine;
	int magazine[MAX_MAGAZINES];
	int m_pickupClipAmmount;

	int reload();
	void initialAmmo();
	void emptyAmmo();
};


#endif