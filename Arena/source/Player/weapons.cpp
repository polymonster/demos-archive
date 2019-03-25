#include "weapons.h"
#include "timerSingleton.h"
#include "textureSingleton.h"
#include "outputText.h"
#include "soundSingleton.h"

void CWeapons::timer(){
	
	timeBasedSpeed = CTimer::getInstance()->animBaseSpeed();
	if(m_fireTimer > 0) m_fireTimer -= timeBasedSpeed;
}

CWeapons::CWeapons()
{
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].alive = false;
	}

	m_range = 4000;
	
	//timer based things
	m_fireRate = 0.1;
	m_fireTimer = 0;

	m_bulletSpeed = 60;

	m_shotType = 2;

	m_flashTimer = 0;
}

CWeapons::CWeapons(const char *weaponData)
{

	for(int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].alive = false;
	}

	ifstream fin;

	fin.open(weaponData);
	
	string word;
	char model[64];
	char modeldir[256];
	char muzzleTexture[64];

	int type;

	fin >> word;
	fin >> model;
	fin >> word;
	fin >> modeldir;

	gunModel.LoadWeapon(modeldir,model);

	fin >> word;
	fin >> m_range;
	
	//timer based things
	fin >> word;
	fin >> m_fireRate;

	fin >> word;
	fin >> m_fireTimer;

	fin >> word;
	fin >> m_bulletSpeed;

	fin >> word;
	fin >> m_reloadRate;

	fin >> word;
	fin >> m_magazineSize;

	fin >> word;
	fin >> m_maxMagazines;

	fin >> word;
	fin >> m_pickupClipAmmount;

	fin >> word;
	fin >> type;
	m_ricochetType = type;
	m_shotType = type;

	fin >> word;
	fin >> m_maximumDamage;

	fin >> word;
	fin >> muzzleTexture;

	fin.close();

	m_muzzleSpriteId = CTextureMan::getInstance()->add(muzzleTexture);

	m_currentMagazines = 0;
	m_currentAmmo = 0;

	m_flashTimer = 0;
}

void CWeapons::displayAmmo(){

	COutput out;

	out.output(900,600,"AMMO: ",m_currentAmmo);
	out.output(900,610,"CLIPS: ",m_currentMagazines);
}

bool CWeapons::shoot(SVector3 shotline[], bool forceShot)
{

	//force shot, will not take into account the fire rate of a gun
	//this is due to problems caused with network latency
	//when multiple bullets must be spawn simultaneously

	if(m_currentAmmo <= 0) CSound::getInstance()->play(0);


	if( ((m_fireTimer <= 0) && (m_currentAmmo > 0)) || (forceShot))
	{
		int i;
		SVector3 bulletEnd = normalize(shotline[1] - shotline[0]);
		int pellets = 0;
		float incrm = 0.1;

		switch(m_shotType)
		{
			case 0:
			//MACHINE GUN
			//find an empty bullet and initialise it.
			for(i = 0; i < MAX_BULLETS; i++)
			{
				if(!bullets[i].alive)
				{
					bullets[i].alive = true;
					bullets[i].shotVector = bulletEnd;
					bullets[i].line[0] = shotline[0];
					bullets[i].line[1] = shotline[0];
					bullets[i].life = 0;

					i = MAX_BULLETS;
				}
			}
			CSound::getInstance()->gunshot(0);
			break;
			case 3:
			//PLASMA GUN
			for(i = 0; i < 1; i++)
			{
					bullets[i].alive = true;
					bullets[i].shotVector = bulletEnd;
					bullets[i].line[0] = shotline[0];
					bullets[i].line[1] = shotline[0];
					bullets[i].life = 0;
			}
			CSound::getInstance()->gunshot(2);
			break;
			case 2:
			//SHOTGUN
			for(i = 0; i < MAX_BULLETS; i++)
			{

				SVector3 bulletEnd = normalize(shotline[1] - shotline[0]);
				bulletEnd.y += float ((rand()%10) - 5) / 100;
				bulletEnd.x += float ((rand()%10) - 5) / 100;
				bulletEnd.z += float ((rand()%10) - 5) / 100;

				if(!bullets[i].alive)
				{
					bullets[i].alive = true;
					bullets[i].shotVector = bulletEnd;
					bullets[i].line[0] = shotline[0];
					bullets[i].line[1] = shotline[0];
					bullets[i].life = 0;

					pellets++;
					if(pellets == 8) i = MAX_BULLETS;

				}
			}
			CSound::getInstance()->gunshot(1);
			break;
			case 4:
			//LAZER
			for(i = 0; i < MAX_BULLETS; i++)
			{
				if(!bullets[i].alive)
				{
					bullets[i].alive = true;
					bullets[i].shotVector = bulletEnd;
					bullets[i].line[0] = shotline[0];
					bullets[i].line[1] = shotline[0];
					bullets[i].life = 0;

					i = MAX_BULLETS;
				}
			}
			CSound::getInstance()->gunshot(2);
			break;
		}

		m_flashTimer = 2;
		m_fireTimer = m_fireRate;
		m_currentAmmo--;
		magazine[m_currentMagazines -1] = m_currentAmmo;
		
		return true;
	}


	return false;
}

void CWeapons::draw(){

	for(int i = 0; i < MAX_BULLETS; i++)
	{
			if(bullets[i].alive)
			{
					bullets[i].type = m_shotType;
					float r,g,b,a,size;

					if(m_shotType == 0) r = 0.7, g = 0.7, b = 0.1, a = 0.8, size = 1;
					else if(m_shotType == 3) r = 0.1, g = 0.7, b = 0.2, a = 0.8, size = 12;
					else if(m_shotType == 4) r = 0.5, g = 0.1, b = 0.1, a = 0.8, size = 5;
					else if(m_shotType == 2) r = 0.8, g = 0.1, b = 0.1, a = 0.8, size = 2;

					glDisable(GL_CULL_FACE);
					//glDepthMask(GL_FALSE);
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);

					glBlendFunc(GL_SRC_ALPHA,GL_ONE);

					glActiveTextureARB(GL_TEXTURE0_ARB);
					glDisable(GL_TEXTURE_2D);
		
					glActiveTextureARB(GL_TEXTURE1_ARB);
					glEnable(GL_TEXTURE_2D);

					CTextureMan::getInstance()->bindParticle(5);

						glPushMatrix();
						glTranslated(bullets[i].line[1].x,bullets[i].line[1].y,bullets[i].line[1].z);
						billboardBegin();
						glColor4f(r,g,b,a); 
						glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
							glMultiTexCoord2f(GL_TEXTURE1_ARB,1,1); glVertex2f(size,size); // Top Right
							glMultiTexCoord2f(GL_TEXTURE1_ARB,0,1); glVertex2f(-size,size); // Top Left
							glMultiTexCoord2f(GL_TEXTURE1_ARB,1,0); glVertex2f(size,-size); // Bottom Right
							glMultiTexCoord2f(GL_TEXTURE1_ARB,0,0); glVertex2f(-size,-size); // Bottom Left
						glEnd();
						billboardEnd();
						glPopMatrix();

					glDisable(GL_TEXTURE_2D);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glEnable(GL_LIGHTING);
					glEnable(GL_CULL_FACE);

					glActiveTextureARB(GL_TEXTURE1_ARB);
					glDisable(GL_TEXTURE_2D);


					glActiveTextureARB(GL_TEXTURE0_ARB);
					glEnable(GL_TEXTURE_2D);
				}
		}

}

void CWeapons::muzzleFlare(float muzzleMatrix[])
{
	if(m_flashTimer > 0)
	{
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glColor4f(1,1,1,0.6);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindTexture(m_muzzleSpriteId);

		glLoadMatrixf(muzzleMatrix);

		glBegin(GL_QUADS);
			glMultiTexCoord2f(GL_TEXTURE1_ARB,0,0); glVertex3f(1,-1,-1);
			glMultiTexCoord2f(GL_TEXTURE1_ARB,0,1); glVertex3f(1,-1,1);
			glMultiTexCoord2f(GL_TEXTURE1_ARB,1,1); glVertex3f(1,1,1);
			glMultiTexCoord2f(GL_TEXTURE1_ARB,1,0); glVertex3f(1,1,-1);
		glEnd();
							
		glPopMatrix();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_LIGHTING);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
	}
}

void CWeapons::update(){

		for(int i = 0; i < MAX_BULLETS; i++)
		{
			if(bullets[i].alive)
			{
				bullets[i].line[0] = bullets[i].line[1];
				bullets[i].line[1] = bullets[i].line[1] + (bullets[i].shotVector * m_bulletSpeed * 35 * timeBasedSpeed);

				bullets[i].life += CTimer::getInstance()->animBaseSpeed() * 1000; 

				if(bullets[i].life > m_range)
				{
					bullets[i].alive = false;
				}
			}
		}

		m_flashTimer -= CTimer::getInstance()->animBaseSpeed() * 10;

	timer();

}

void CWeapons::linkGunModel(CModelMD3 &character){

	t3DModel *pModel ,*pUpper;

	pModel = gunModel.GetModel(3);
	pUpper = character.GetModel(1);
	character.LinkModel(pUpper,pModel,"tag_weapon");

}

bool CWeapons::pickUp(){

	t3DModel *pModel = gunModel.GetModel(3);

	glCullFace(GL_FRONT);
	glPushMatrix();
	glRotated(-90,1,0,0);
	glScaled(2,2,2);
	gunModel.RenderModel(pModel);
	glPopMatrix();
	glCullFace(GL_BACK);

	return false;
}

void CWeapons::addAmmo(int clips){

	int prev = m_currentMagazines;

	m_currentMagazines += m_pickupClipAmmount;

	if(m_currentMagazines > m_maxMagazines) m_currentMagazines = m_maxMagazines;

	for(int i = prev; i < m_currentMagazines - 1; i++)
	{
		magazine[i] = m_magazineSize;
	}

	if(prev == 0) m_currentAmmo = 0;
}

void CWeapons::initialAmmo()
{
	m_currentAmmo = m_magazineSize;
	m_currentMagazines--;	
}

int CWeapons::reload()
{
	if(m_currentMagazines > 0)
	{
		m_currentMagazines--;
		m_currentAmmo = m_magazineSize;
		return m_reloadRate;

	}
	else
	{
		CSound::getInstance()->play(0);
		return -1;
	}

	return -1;

}

void CWeapons::emptyAmmo()
{
	m_currentAmmo = 0;
	m_currentMagazines = 0;

	for(int i = 0; i < MAX_MAGAZINES; i++)
	{	
		magazine[i] = 0;
	}
}
