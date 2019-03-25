#include "player.h"
#include "timerSingleton.h"
#include "HCI.h"

void CPlayer::drawInterface(int offset){

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	OrthoMode(0,0,xScreenSize,yScreenSize);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE);
		
		//loop through elments and draw them
		drawHealth();

		drawDamage(0);

		drawMiniMap();

		drawGunInfo();

		drawScore();

		if(m_overShoulder) drawCrosshairs();		

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	messageDisplay();

	PerspectiveMode();

	glPopMatrix();
}

void CPlayer::drawHealth()
{

	float xstart = 10;
	float healthBarPos = m_health * 2;
	if(healthBarPos < 0) healthBarPos = 0;

	float healthTex = m_health * 0.01;
	

	// HEALTH FASCIA 

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	CTextureMan::getInstance()->bindParticle(11);

	glPushMatrix();

		glTranslated(110,yScreenSize - 60,0);
	
		glColor4f(0,0.39,0.56,0.1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-100,-50);
			glTexCoord2f(1,0);glVertex2f(100,-50);
			glTexCoord2f(1,1);glVertex2f(100,50);
			glTexCoord2f(0,1);glVertex2f(-100,50);
		glEnd();

	glPopMatrix();


	//HEALTH INDICATOR

	CTextureMan::getInstance()->bindParticle(10);

	glPushMatrix();

		glTranslated(0,yScreenSize - 60,0);

		glColor4f(0,0.39,0.56,0.1);

		glBegin(GL_QUADS);

			glTexCoord2f(0,0);
			glVertex2f(xstart,-50);

			glTexCoord2f(healthTex,0);
			glVertex2f(xstart + healthBarPos,-50);

			glTexCoord2f(healthTex,1);
			glVertex2f(xstart + healthBarPos,50);

			glTexCoord2f(0,1);
			glVertex2f(xstart,50);

		glEnd();

	glPopMatrix();
}

void CPlayer::drawMiniMap()
{
	if(p2p.x > 75) p2p.x = 75;
	if(p2p.y > 75) p2p.y = 75;

	glPushMatrix();

		glTranslated(85,85,0);

		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindParticle(7);
	
		glColor4f(0,0.39,0.56,0.1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-75,-75);
			glTexCoord2f(1,0);glVertex2f(75,-75);
			glTexCoord2f(1,1);glVertex2f(75,75);
			glTexCoord2f(0,1);glVertex2f(-75,75);
		glEnd();

		glRotated(180,0,0,1);
		glRotated(-yrot,0,0,1);
		
		glDisable(GL_TEXTURE_2D);
		CTextureMan::getInstance()->bindParticle(1);

		glColor4f(0,0.4,0.4,0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-5,-5);
			glTexCoord2f(1,0);glVertex2f(-5,5);
			glTexCoord2f(1,1);glVertex2f(5,5);
			glTexCoord2f(0,1);glVertex2f(5,-5);
		glEnd();

		glTranslated(p2p.x,p2p.z,0);
		glColor4f(0.5,0,0.1,0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-5,-5);
			glTexCoord2f(1,0);glVertex2f(-5,5);
			glTexCoord2f(1,1);glVertex2f(5,5);
			glTexCoord2f(0,1);glVertex2f(5,-5);
		glEnd();

	glPopMatrix();

}

void CPlayer::drawGunInfo()
{
	glPushMatrix();

		glTranslated(xScreenSize - 110,yScreenSize - 60,0);

		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindParticle(9);

		glColor4f(0,0.39,0.56,0.1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-100,-50);
			glTexCoord2f(1,0);glVertex2f(100,-50);
			glTexCoord2f(1,1);glVertex2f(100,50);
			glTexCoord2f(0,1);glVertex2f(-100,50);
		glEnd();

	glPopMatrix();

	glPushMatrix();

		glTranslated(xScreenSize - 110,yScreenSize - 60,0);

		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindParticle(14 + m_equippedGun);

		glColor4f(0.6,0.6,0.6,0.1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-100,-50);
			glTexCoord2f(1,0);glVertex2f(100,-50);
			glTexCoord2f(1,1);glVertex2f(100,50);
			glTexCoord2f(0,1);glVertex2f(-100,50);
		glEnd();



	glPopMatrix();

	//DRAW THE AMMO INFO NUMBERS

	COutput out;

	SVector2 dims;
	dims.x = 64;
	dims.y = 36;

	CTextureMan::getInstance()->bindParticle(13);

	//CLIPS

	glPushMatrix();
	
	glTranslated(xScreenSize - 42,yScreenSize - 40,0);

	glScaled(0.35,0.5,1);

	out.bigNums(100,100,myGun[m_equippedGun]->m_currentMagazines,dims);

	glPopMatrix();

	//AMMO

	glPushMatrix();
	
	glTranslated(xScreenSize - 97,yScreenSize - 32,0);

	glScaled(0.3,0.3,1);
	out.bigNums(100,100,myGun[m_equippedGun]->m_currentAmmo,dims);

	glPopMatrix();
}

void CPlayer::drawCrosshairs()
{
	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	CTextureMan::getInstance()->bindCrosshair(m_equippedGun);

	if(m_crosshairOverPlayer) glColor4f(1,0,0,0.1);
	else glColor4f(0,0.39,0.56,0.1);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex2f(xScreenSize / 2 - 25,yScreenSize / 2 - 25);
		glTexCoord2f(1,0);glVertex2f(xScreenSize / 2 + 25,yScreenSize / 2 - 25);
		glTexCoord2f(1,1);glVertex2f(xScreenSize / 2 + 25,yScreenSize / 2 + 25);
		glTexCoord2f(0,1);glVertex2f(xScreenSize / 2 - 25,yScreenSize / 2 + 25);
	glEnd();

}

void CPlayer::drawMessages()
{

}

void CPlayer::setupDamageIndicator()
{
	SVector2 center;
	center.x = xScreenSize / 2;
	center.y = yScreenSize / 2;
			
	SVector3 dimsH(xScreenSize / 5,yScreenSize / 5,0);
	SVector3 dimsV(xScreenSize / 10,yScreenSize / 10,0);

	m_damageIndicator[0].position[0].x = center.x - dimsH.x;
	m_damageIndicator[0].position[0].y = 0;
	m_damageIndicator[0].position[1].x = center.x + dimsH.x;
	m_damageIndicator[0].position[1].y = dimsV.y;

	m_damageIndicator[0].texCoords[0].x = 1;
	m_damageIndicator[0].texCoords[0].y = 0;
	m_damageIndicator[0].texCoords[1].x = 0;
	m_damageIndicator[0].texCoords[1].y = 1;

	m_damageIndicator[1].position[0].x = center.x - dimsH.x;
	m_damageIndicator[1].position[0].y = yScreenSize;
	m_damageIndicator[1].position[1].x = center.x + dimsH.x;
	m_damageIndicator[1].position[1].y = yScreenSize - dimsV.y;

	m_damageIndicator[1].texCoords[0].x = 1;
	m_damageIndicator[1].texCoords[0].y = 0;
	m_damageIndicator[1].texCoords[1].x = 0;
	m_damageIndicator[1].texCoords[1].y = 1;

	m_damageIndicator[2].position[0].x = 0;
	m_damageIndicator[2].position[0].y = center.y - dimsH.y;
	m_damageIndicator[2].position[1].x = dimsV.x;
	m_damageIndicator[2].position[1].y = center.y + dimsH.y;

	m_damageIndicator[2].texCoords[0].x = 0;
	m_damageIndicator[2].texCoords[0].y = 1;
	m_damageIndicator[2].texCoords[1].x = 1;
	m_damageIndicator[2].texCoords[1].y = 0;

	m_damageIndicator[3].position[0].x = xScreenSize;
	m_damageIndicator[3].position[0].y = center.y - dimsH.y;
	m_damageIndicator[3].position[1].x = xScreenSize - dimsV.x;
	m_damageIndicator[3].position[1].y = center.y + dimsH.y;

	m_damageIndicator[3].texCoords[0].x = 0;
	m_damageIndicator[3].texCoords[0].y = 1;
	m_damageIndicator[3].texCoords[1].x = 1;
	m_damageIndicator[3].texCoords[1].y = 0;
}

void CPlayer::drawScore()
{
	glPushMatrix();

		glTranslated(xScreenSize - 110,60,0);

		glActiveTextureARB(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindParticle(12);

		glColor4f(0,0.39,0.56,0.1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex2f(-100,-50);
			glTexCoord2f(1,0);glVertex2f(100,-50);
			glTexCoord2f(1,1);glVertex2f(100,50);
			glTexCoord2f(0,1);glVertex2f(-100,50);
		glEnd();

	glPopMatrix();

	//DRAW THE SCORE NUMBERS
	COutput out;

	SVector2 dims;
	dims.x = 64;
	dims.y = 36;

	CTextureMan::getInstance()->bindParticle(13);

	//MY KILLS

	glPushMatrix();
	
	glTranslated(xScreenSize - 162,25,0);

	glScaled(0.6,0.6,1);

	out.bigNums(0,0,m_kills,dims);

	glPopMatrix();

	//MY DEATHS / OPPONENT DEATHS

	glPushMatrix();
	
	glTranslated(xScreenSize - 60,25,0);

	glScaled(0.6,0.6,1);

	glColor4f(1,0,0,0.1);

	out.bigNums(0,0,m_deaths,dims,true);

	glPopMatrix();

	//TIMER

	glPushMatrix();
	
	glTranslated(xScreenSize - 52,75,0);

	glScaled(0.6,0.6,1);
 
	glColor4f(0,0.39,0.56,0.1);

	out.bigNums(0,0,CTimer::getInstance()->m_gameTimer[3],dims,true);

	glTranslated(-dims.x,0,0);

	out.bigNums(0,0,CTimer::getInstance()->m_gameTimer[2],dims,true);

	glTranslated(-dims.x * 1.2,0,0);

	out.bigNums(0,0,CTimer::getInstance()->m_gameTimer[1],dims,true);

	glTranslated(-dims.x,0,0);

	out.bigNums(0,0,CTimer::getInstance()->m_gameTimer[0],dims,true);

	glPopMatrix();
}

void CPlayer::drawDamage(int side)
{
	setupDamageIndicator();

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	CTextureMan::getInstance()->bindParticle(8);
	
	for(int i = 0 ; i < 4; i++)
	{
		if(m_damageIndicator[i].active)
		{
			glColor4f(1,0,0,m_damageIndicator[i].life);

			glBegin(GL_QUADS);

				glTexCoord2f(0,0); 
				glVertex2f(m_damageIndicator[i].position[0].x,m_damageIndicator[i].position[0].y);

				glTexCoord2f(m_damageIndicator[i].texCoords[0].x,m_damageIndicator[i].texCoords[0].y); 
				glVertex2f(m_damageIndicator[i].position[1].x,m_damageIndicator[i].position[0].y);

				glTexCoord2f(1,1); 
				glVertex2f(m_damageIndicator[i].position[1].x,m_damageIndicator[i].position[1].y);

				glTexCoord2f(m_damageIndicator[i].texCoords[1].x,m_damageIndicator[i].texCoords[1].y);  
				glVertex2f(m_damageIndicator[i].position[0].x,m_damageIndicator[i].position[1].y);

			glEnd();

			m_damageIndicator[i].life -= CTimer::getInstance()->animBaseSpeed();
			if(m_damageIndicator[i].life < 0) m_damageIndicator[i].active = false;
		}
	}

	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE);
}

void CPlayer::initDamageIndicator(float shooterRotation)
{
	int side = -1;

	float quadrantRotation = yrot - shooterRotation;

	if(quadrantRotation > 180) quadrantRotation -=360;
	else if(quadrantRotation < -180) quadrantRotation +=360;

	if((quadrantRotation > -45) && (quadrantRotation < 45)) side = 1;
	if((quadrantRotation < -45) && (quadrantRotation > -135))side = 2;
	if((quadrantRotation > 45) && (quadrantRotation < 135))side = 3;
	if((quadrantRotation > 135) || (quadrantRotation < -135))side = 0;

	if(side != -1)
	{
		m_damageIndicator[side].active = true;
		m_damageIndicator[side].life = 1;
	}
}

void CPlayer::messageDisplay()
{
	COutput message;

	glColor4f(1,1,1,1);

	int baseY = yScreenSize - 110;

	for(int i = 0; i < 4; i++)
	{
		if(m_messageArray[i].active)
		{
			message.output(20,baseY,"",m_messageArray[i].message);
			baseY -= 12;
		}

		m_messageArray[i].timer -= CTimer::getInstance()->animBaseSpeed();

		if(m_messageArray[i].timer <= 0) m_messageArray[i].active = false;
	}
}

void CPlayer::addMessage(char *message)
{
	for(int i = 0; i < 4; i++)
	{
		if(!m_messageArray[i].active)
		{
			m_messageArray[i].active = true;
			m_messageArray[i].timer = 5;
			strcpy(m_messageArray[i].message,message);
			return;
		}
	}
}

void CPlayer::clearMessages()
{
	for(int i = 0; i < 4; i++)
	{
		m_messageArray[i].active = false;
	}

}