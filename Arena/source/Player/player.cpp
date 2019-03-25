#include "player.h"
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "HCI.h"
#include "outputText.h"
#include "modelManager.h"
#include "soundSingleton.h"

#pragma warning(disable: 4786)

int g_face = 0;

void CPlayer::timeBasedMovement(){

	timeBasedSpeed = CTimer::getInstance()->animBaseSpeed();
	m_reloadTimer -= CTimer::getInstance()->animBaseSpeed();
	m_footstepTimer -= CTimer::getInstance()->animBaseSpeed() * m_footstepRate;

	manageFootsteps();
}

void CPlayer::debugvars(){

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	COutput out;

	glMatrixMode(GL_PROJECTION);			//set projection matrix
	glLoadIdentity();						//reset the matrix
	gluOrtho2D(0, 1024, 768, 0);

    glColor3f(1,1,1);

	out.output(10,50,"XPOS : ",xpos);
	out.output(10,60,"YPOS : ",ypos);
	out.output(10,70,"ZPOS : ",zpos);

	out.output(10,80,"XROT : ",xrot);
	out.output(10,90,"YROT : ",yrot);

	out.output(10,100,"CAMXROT : ",camrx);
	out.output(10,110,"CAMYROT : ",camry);

	out.output(100,20,"HEALTH: ",m_health);

	out.output(100,10,"GRAVITY : ",gravityBalance);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity ();
	gluPerspective (60, (GLfloat)xScreenSize / (GLfloat)yScreenSize, 1.0, 1024.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

CPlayer::CPlayer(){

	xpos = 300, newx = 300;
	ypos = 60, newy = 60;
	zpos = 300, newz = 300;

	xrot = 0;
	yrot = 0;

	camrx = 0;
	camry = 0;

	m_equippedGun = 0;

	for(int i = 0; i < 4; i++)
	{
		gotGun[i] = false;
	}

	gotGun[m_equippedGun] = true;

	load();

	shotline[0] = SVector3(xpos,ypos,zpos);
	shotline[1] = SVector3(xpos,ypos,zpos);

	m_shot = false;

	m_health = 50;
	m_armour = 50;

	m_chair = 0;

	m_radiusVector = SVector3(20,50,20);

	m_crouched = false;

	m_alive = true;

	m_loadWait = 0;

	m_aimMode = 0;

	m_jumpCounter = 0;

	m_respawnTimer = 0;
	
	gravityBalance = 0;

	run = false;

	dv = 0;

	m_overShoulder = false;

	playerData.shoot = false;

	playerData.shotLock = 0;

	playerData.shotsRecieved = false;

	playerData.discard = false;

	m_kills = 0;
	m_deaths = 0;

	setupDamageIndicator();

	m_reloadTimer = 0;

	m_footstepTimer = 0;
	m_footstepChannel = 2;
	m_footstepRate = 1;


}

CPlayer::~CPlayer(){
	
	/*t3DModel *pModel;

	pModel = character.GetModel(1);
	character.DestroyModel(pModel);

	pModel = character.GetModel(0);
	character.DestroyModel(pModel);

	pModel = character.GetModel(2);
	character.DestroyModel(pModel);*/
}

void CPlayer::load()
{
	loadProfile();

	character[0].LoadModel("data/models/characters/OA/tommi","tommi");
	character[0].weaponPartID = 2;

	character[2].LoadModel("data/models/characters/OA/tommi","tommi");
	character[2].weaponPartID = 2;

	character[1].LoadModel("data/models/characters/OA/assassin","assassin");
	character[1].weaponPartID = 1;

	myGun[0] = new CWeapons("data/configs/weapons/machinegun.txt");
	myGun[1] = new CWeapons("data/configs/weapons/shotgun.txt");
	myGun[2] = new CWeapons("data/configs/weapons/plasma.txt");
	myGun[3] = new CWeapons("data/configs/weapons/lazer.txt");

	resetGameData();
}

void CPlayer::resetGameData()
{
	xpos = 0;
	ypos = 0;
	zpos = 0;

	yrot = 0;
	xrot = 0;

	m_kills = 0;
	m_deaths = 0;

	setPositionData();

	int i;

	for(i = 0; i < 4; i++)
	{
		m_messageArray[i].active = false;
	}

	for(i = 0; i < 4; i++)
	{
		gotGun[i] = false;
		myGun[i]->emptyAmmo();
	}

	m_equippedGun = 0;
	gotGun[m_equippedGun] = true;

	myGun[m_equippedGun]->addAmmo(0);
	myGun[m_equippedGun]->initialAmmo();

	myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);

	character[m_activeChar].SetTorsoAnimation("TORSO_STAND");
	character[m_activeChar].SetLegsAnimation("LEGS_IDLE");

	setState(LEGS_IDLE);

	m_overShoulder = true;
}

void CPlayer::loadProfile()
{
	strcpy(m_playerName,"");
	m_activeChar = 0;
	strcpy(m_playerName,"player 1");


	ifstream fin;

	fin.open("data/settings/profile.dat");

	string word;

	while(!fin.eof())
	{
		fin >> word;

		if(word == "character_model") fin >> m_activeChar;
		else if(word == "player_name") fin >> m_playerName;

	}

	fin.close();
}

void CPlayer::draw(){

	//drawBB();

	if(m_aimMode == 0)
	{
		glPushMatrix();

		int offset = 12;
		if(m_crouched) offset = 7;
		if(!m_canJump) offset = 35;

		glTranslated(xpos,ypos - offset,zpos);
		
		glFrontFace(GL_CW);
		
		glRotatef(90,0,1,0);
		glRotatef(-yrot,0,1,0);

		glScaled(1.6,1.6,1.6);

		//glDisable(GL_LIGHTING);
		glColor4f(1,1,1,1);
		character[m_activeChar].DrawModel(xrot);

		glPopMatrix();

		glFrontFace(GL_CCW);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_CULL_FACE);

	}
}

void CPlayer::shadowVolumeBB()
{
	glPushMatrix();
		glTranslated(xpos,ypos,zpos);
		glScaled(m_radiusVector.x + 100,m_radiusVector.y + 100,m_radiusVector.z + 100);
		glutSolidCube(1);
	glPopMatrix();
}

void CPlayer::renderCharacterChooser()
{
	int x,y;

	x = xScreenSize - 550;
	y = yScreenSize - 545;

	PerspectiveMode();

	glViewport(x,y,600,400);

	glFrontFace(GL_CW);

	glPushMatrix();

		//float position = 100 * (float) xScreenSize / 1024;
		//float positiony = 30 * (float) yScreenSize / 768;

		glTranslated(0,0,-150);
		
		glRotatef(-90,0,1,0);

		glScaled(1.6,1.6,1.6);

		glColor4f(1,1,1,1);

		character[m_activeChar].DrawModel(0);

	glPopMatrix();

	glFrontFace(GL_CCW);

	glViewport(0,0,xScreenSize,yScreenSize);
	
}

void CPlayer::renderGun()
{
	myGun[m_equippedGun]->draw();
}

void CPlayer::renderMuzzleFlare()
{
	myGun[m_equippedGun]->muzzleFlare(character[m_activeChar].muzzleMatrix);
}

void CPlayer::setMiniMapData(SVector3 opponent){

	p2p = posVector() - opponent;

	float d = (distance(posVector(),opponent) / 20);
	
	p2p = normalize(p2p);

	if(d > 100) d = 100;

	p2p = p2p * d;
}

void CPlayer::setCrosshair(SVector3 crosshair){

	m_crosshair = crosshair;
	m_chair = 0;
}

void CPlayer::setStartPos(SVector3 startPos){

	newx = startPos.x;
	newy = startPos.y;
	newz = startPos.z;

	xpos = startPos.x;
	ypos = startPos.y;
	zpos = startPos.z;

}

void CPlayer::controlPlayer()
{
	timeBasedMovement();

	SControls input = user::getInstance()->input;

	if(m_alive)
	{
		//set theplayer to idle every time
		m_radiusVector = SVector3(20,50,20);
		newState = LEGS_IDLE;

		m_crouched = false;
		run = false;

		m_footstepRate = 1;

		int div = 1;
		int siv = 1;

		if((input.key['a']) || (input.key['A']) || (input.key['d']) || (input.key['D'])) div = 2;
		if((input.key['w']) || (input.key['W']) || (input.key['s']) || (input.key['S'])) siv = 2;

		if((input.key['c']) || (!m_canStandUp) && (character[m_activeChar].jumpComplete))
		{
			m_crouched = true;
			m_radiusVector = SVector3(30,40,30);
			newState = LEGS_IDLECR;
		}

		//get input to increas or decrease velocity
		if((input.key['w']) || (input.key['W']))
		{
			//updated the position and rotation based on the above velocities
			//forwards
			float yrotrad;
			yrotrad = (yrot / 180 * PI);
			newx += float(sin(yrotrad)) * (200/div * timeBasedSpeed);
			newz -= float(cos(yrotrad)) * (200/div * timeBasedSpeed);

			newState = LEGS_WALK;
			if(m_crouched) newState = LEGS_WALKCR;

			if(input.key['q'])
			{
				//updated the position and rotation based on the above velocities
				//forwards
				float yrotrad;
				run = true;
				yrotrad = (yrot / 180 * PI);
				newx += float(sin(yrotrad)) * (250/div * timeBasedSpeed);
				newz -= float(cos(yrotrad)) * (250/div * timeBasedSpeed);

				newState = LEGS_RUN;
				if(m_crouched) newState = LEGS_WALKCR;

				m_footstepRate = 2;

			}

			if(m_footstepTimer <= 0) 
			{
				m_footstepTimer = 0.4;
				CSound::getInstance()->play(m_footstepChannel);
			}
		}



		if((input.key['s']) || (input.key['S']))
		{
			//updated the position and rotation based on the above velocities
			//forwards
			float yrotrad;
			yrotrad = (yrot / 180 * PI);
			newx += float(sin(yrotrad)) * (-100/div * timeBasedSpeed);
			newz -= float(cos(yrotrad)) * (-100/div * timeBasedSpeed);

			newState = LEGS_BACK;
			if(m_crouched) newState = LEGS_WALKCR;
			CSound::getInstance()->play(m_footstepChannel);

			m_footstepRate = 0.5;
		}

		if((input.key['a']) || (input.key['A']))
		{
			//strafe
			float yrotrad = (yrot / 180 * PI);
			newx += float(cos(yrotrad)) * (-200/siv * timeBasedSpeed);
			newz += float(sin(yrotrad)) * (-200/siv * timeBasedSpeed);

			newState = LEGS_WALK;
			if(m_crouched) newState = LEGS_WALKCR;
			CSound::getInstance()->play(m_footstepChannel);
		}

		if((input.key['d']) || (input.key['D']))
		{
			//strafe
			float yrotrad = (yrot / 180 * PI);
			newx += float(cos(yrotrad)) * (200/siv * timeBasedSpeed);
			newz += float(sin(yrotrad)) * (200/siv * timeBasedSpeed);

			newState = LEGS_WALK;
			if(m_crouched) newState = LEGS_WALKCR;
			CSound::getInstance()->play(m_footstepChannel);
		}

		//shooting
		m_shot = false;

		if(input.leftMouse)
		{
			shoot();
		}

		if(input.rightMouse) 
		{
			m_overShoulder = !m_overShoulder;
			user::getInstance()->input.rightMouse = false;
		}

		newy -= timeBasedSpeed * 400;
		
			float newxrot;

			float rotationAccuracy = 5;

			if(m_overShoulder) rotationAccuracy = 10;

			newxrot = xrot + input.incx / rotationAccuracy;
			yrot += input.incy / rotationAccuracy; 
			
			//set cam pos
			camx = xpos;
			camy = ypos;
			camz = zpos;

			if(yrot > 180) yrot -= 360;
			else if(yrot < -180) yrot += 360;

			if((newxrot > 50) || (newxrot < -30))
			{
				newxrot = xrot;
			}
			
			xrot = newxrot;

			if(xrot < camrx -30 ) camrx+= input.incx / rotationAccuracy;
			else if(xrot > camrx +20 )camrx+= input.incx / rotationAccuracy;

			if(yrot < camry -45) camry+= input.incy / rotationAccuracy;
			else if(yrot > camry +45 )camry+= input.incy / rotationAccuracy;

			if(camrx > 180) camrx -= 360;
			else if(camry < -180) camry += 360;

			float camxrad;
			float yrotrad = (camry / 180 * PI);
			camxrad = (camrx / 180 * PI);
			camx -= float(sin(yrotrad))*200;
			camz += float(cos(yrotrad))*200;
			camy += float(sin(camxrad)) * 150;

			camy += 50;

			setState(newState);
			

			if((input.key['1']) && (gotGun[0]) && (m_equippedGun != 0))
			{
				m_equippedGun = 0;
				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
			}
			else if((input.key['2']) && (gotGun[1]) && (m_equippedGun != 1)) 
			{
				m_equippedGun = 1;
				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
			}
			else if((input.key['3']) && (gotGun[2]) && (m_equippedGun != 2))
			{
				m_equippedGun = 2;
				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
			}
			else if((input.key['4']) && (gotGun[3]) && (m_equippedGun != 3))
			{
				m_equippedGun = 3;
				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
			}


			if(input.wheelUp)
			{
				do
				{
					m_equippedGun++;
					if(m_equippedGun > 3) m_equippedGun = 0;

				}while(!gotGun[m_equippedGun]);

				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
				
				input.wheelUp = false;
				
			}
			else if(input.wheelDown)
			{
				do
				{
					m_equippedGun--;
					if(m_equippedGun < 0) m_equippedGun = 3;

				}while(!gotGun[m_equippedGun]);

				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();

				input.wheelDown = false;
			}

			if(user::getInstance()->input.key['r']) 
			{
				if(m_reloadTimer <= 0)
				{
					user::getInstance()->input.key['r'] = false;
					reload();
				}
			}
		}		

}

void CPlayer::manageFootsteps()
{
	if(m_footstepTimer <= 0)
	{	
		//switch the channel
		if(m_footstepChannel == 2) m_footstepChannel = 9;
		else m_footstepChannel = 2;

		//reset the timer
		
	}
}

void CPlayer::changeCharacter(int id)
{
	m_activeChar = id;
	myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
	character[m_activeChar].SetTorsoAnimation("TORSO_STAND");
	state = -1;
	setState(LEGS_IDLE);
}

bool CPlayer::checkForDeath()
{
	if((m_health <= 0) && (m_alive))
	{
		kill();
		m_alive = false;
		return true;
	}

	if(!m_alive)
	{
		m_respawnTimer -= CTimer::getInstance()->animBaseSpeed();

		if(m_respawnTimer < 0)
		{
			resetAfterDeath();
		}
	}
	return false;
}



bool CPlayer::checkNetworkForDeath()
{
	if((m_health <= 0) && (m_alive) && (m_respawnTimer <= 0))
	{
		kill();
		m_alive = false;
		m_respawnTimer = 1;
		return true;
	}

	if(m_respawnTimer > 0)
	{
		m_respawnTimer -= CTimer::getInstance()->animBaseSpeed();
	}


	return false;
}

void CPlayer::resetAfterDeath()
{
	for(int i = 0; i < 4; i++)
	{
		gotGun[i] = false;
		myGun[i]->emptyAmmo();
	}

	m_equippedGun = 0;
	gotGun[m_equippedGun] = true;

	myGun[m_equippedGun]->addAmmo(0);
	myGun[m_equippedGun]->initialAmmo();

	myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);

	setIdle();

	m_overShoulder = true;

	setStartPos(nextSpawnPoint);
	m_alive = true;
	m_health = 100;

}

void CPlayer::reload(){

	m_reloadTimer = myGun[m_equippedGun]->reload();

	if(m_reloadTimer != -1)
	{
		//tell the weapon file to reload
		t3DModel *pModel, *pUpper, *pLower;

		// Here we store pointers to the legs and torso, so we can display their current anim name
		pLower = character[m_activeChar].GetModel(kLower);
		pUpper = character[m_activeChar].GetModel(kUpper);

		// This line gives us a pointer to the model that we want to change
		pModel = character[m_activeChar].GetModel(1);

		// Increase the current nimation and mod it by the max animations
		pModel->currentAnim = TORSO_DROP;

		// Set the current frame to be the starting frame of the new animation
		pModel->currentFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

		pModel->currentAnim = TORSO_STAND;

		// Set the current frame to be the starting frame of the new animation
		pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

		m_loadWait = 1;

		CSound::getInstance()->play(1);
	}

}

void CPlayer::pickUp(){

	//tell the weapon file to reload
	t3DModel *pModel;

	// This line gives us a pointer to the model that we want to change
	pModel = character[m_activeChar].GetModel(kUpper);

	// Increase the current nimation and mod it by the max animations
	pModel->currentAnim = TORSO_RAISE;

	// Set the current frame to be the starting frame of the new animation
	pModel->currentFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

	pModel->currentAnim = TORSO_STAND;

	// Set the current frame to be the starting frame of the new animation
	pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

//	CSound::getInstance()->play(1);

	m_loadWait = 1;
}

void CPlayer::setStanding(){

	//tell the weapon file to reload
	t3DModel *pModel;

	// This line gives us a pointer to the model that we want to change
	pModel = character[m_activeChar].GetModel(kUpper);

	// Increase the current nimation and mod it by the max animations
	pModel->currentAnim = TORSO_STAND;

	// Set the current frame to be the starting frame of the new animation
	pModel->currentFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

	m_loadWait = 1;
}

void CPlayer::get3dCursor()
{
	int mx = user::getInstance()->input.mousex;
	int my = user::getInstance()->input.mousey;

	cursorPos = get3dCursorPos(mx,my);
}

void CPlayer::setCamPos(){

	if(m_aimMode == 1)
	{
		glRotatef(xrot,1.0,0.0,0.0); 
		glRotatef(yrot,0.0,1.0,0.0);
		glTranslated(-m_muzzlePos.x,-m_muzzlePos.y,-m_muzzlePos.z);
	}
	else
	{
		if(!m_alive) glRotatef(10,1.0,0.0,0.0);

		glRotatef(camrx,1.0,0.0,0.0); 
		glRotatef(camry,0.0,1.0,0.0);
		glTranslated(-camx,-camy,-camz);

		if(!m_alive) glTranslated(0,0,-50);	  	   
	}
}

void CPlayer::centreCamera(){

	camry = yrot;
	camrx = xrot;
}

void CPlayer::setState(int s){

	if(s != state)
	{
		state = s;
		newState = s;

		switch(s)
		{
			case  LEGS_WALK:

					character[m_activeChar].SetLegsAnimation("LEGS_WALK");
			break;

			case  LEGS_BACK:
					
					character[m_activeChar].SetLegsAnimation("LEGS_BACK");
			break;

			case  LEGS_IDLE:
					
					character[m_activeChar].SetLegsAnimation("LEGS_IDLE");
			break;

			case  LEGS_JUMP:

					character[m_activeChar].SetLegsAnimation("LEGS_JUMP");

			break;

			case  LEGS_JUMPB:
					
					character[m_activeChar].SetLegsAnimation("LEGS_JUMPB");
			break;

			case  LEGS_LAND:
					
					character[m_activeChar].SetLegsAnimation("LEGS_LAND");
			break;

			case  LEGS_WALKCR:
					
					character[m_activeChar].SetLegsAnimation("LEGS_WALKCR");
			break;

			case  LEGS_IDLECR:
					
					character[m_activeChar].SetLegsAnimation("LEGS_IDLECR");
			break;

			case  LEGS_RUN:
					
					character[m_activeChar].SetLegsAnimation("LEGS_RUN");
			break;

		}
		
		state = s;
		newState = s;
	}
}

void CPlayer::kill(){

	m_aimMode = 0;

	int darray[3];

	darray[0] = 0;
	darray[1] = 2;
	darray[2] = 4;

	int deathAnim = rand()%3;

	CSound::getInstance()->play(20 + deathAnim);

	t3DModel *pModel, *pUpper, *pLower;

	// Here we store pointers to the legs and torso, so we can display their current anim name
	pLower = character[m_activeChar].GetModel(kLower);
	pUpper = character[m_activeChar].GetModel(kUpper);

	// This line gives us a pointer to the model that we want to change
	pModel = character[m_activeChar].GetModel(0);

	// Increase the current nimation and mod it by the max animations
	pModel->currentAnim = darray[deathAnim];

	// Set the current frame to be the starting frame of the new animation
	pModel->currentFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

	pModel->currentAnim = darray[deathAnim] + 1;

	// Set the current frame to be the starting frame of the new animation
	pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

	state = darray[deathAnim] + 1;

	m_alive = false;

	m_respawnTimer = 5;

	m_deaths++;
}

void CPlayer::drawBB(){

	glDisable(GL_LIGHTING);
	
	glColor3f(1,1,1);

	glBegin(GL_LINES);
		glVertex3f(shotline[0].x,shotline[0].y,shotline[0].z);
		glVertex3f(shotline[1].x,shotline[1].y,shotline[1].z);
	glEnd();

	glPushMatrix();
	glTranslated(xpos,ypos,zpos);

	glScaled(m_radiusVector.x,m_radiusVector.y,m_radiusVector.z);
	GLUquadricObj *pObj = gluNewQuadric();				// Get a Quadric off the stack#
	gluQuadricDrawStyle(pObj, GLU_LINE);				// Draw the sphere normally
	gluSphere(pObj,1, 15, 15);
	gluDeleteQuadric(pObj);								// Free the Quadric

	glPopMatrix();

	glEnable(GL_LIGHTING);

}

void CPlayer::updatePos(){
	
	//myGun[m_equippedGun]->update();

	xpos = newx;
	ypos = newy;
	zpos = newz;

	camx = xpos;
	camy = ypos;
	camz = zpos;

	//set the camera line, muzzlePos and shotline

	float camxrad, yrotrad,xrotrad;

	yrotrad = (camry / 180 * PI);
	camxrad = (camrx / 180 * PI);

	//SHOULDER VIEW
	if(m_overShoulder)
	{
		centreCamera();

		float y = (camry / 180 * PI);
		float x = (camrx / 180 * PI);

		camx -= float(sin(y)) * 90;
		camz += float(cos(y)) * 90;
		camy += float(sin(x)) * 50;
		camy += 40;

		float position = 30 * (float) xScreenSize / 1024;
		camx += float(cos(y)) * position;
		camz += float(sin(y)) * position;


		/*camx += float(-cos(x) * sin(y));
		camy += float(sin(x));
		camz += float(cos(x) * cos(y));

		camz *= 2;
		camy *= 2;
		camx *= 2;*/


	}
	else
	{
		camx -= float(sin(yrotrad))*200;
		camz += float(cos(yrotrad))*200;
		camy += float(sin(camxrad)) * 150;
		camy += 50;
	}

	camline[0].x = xpos; 
	camline[0].y = ypos; 
	camline[0].z = zpos; 

	camline[1].x = camx; 
	camline[1].y = camy; 
	camline[1].z = camz; 

	SVector3 scale = SVector3(1,1,1);
	m_muzzlePos = rotateAboutY(character[m_activeChar].bulletStartPos,scale,posVector(),yrot);

	//aim pos
	yrotrad = (yrot / 180 * PI);
	xrotrad = (xrot / 180 * PI);
	float lx = m_muzzlePos.x + (sin(yrotrad)) * 1000;
	float ly = m_muzzlePos.y - (sin(xrotrad)) * 1000;
	float lz = m_muzzlePos.z - (cos(yrotrad)) * 1000;

	shotline[1] = SVector3(lx,ly,lz);
	shotline[0] = SVector3(m_muzzlePos.x,m_muzzlePos.y,m_muzzlePos.z);

	if(m_overShoulder) shotline[1] = cursorPos;

	playerToFloor[0] = SVector3(xpos,ypos,zpos);
	playerToFloor[1] = SVector3(xpos,ypos - 70,zpos);

	playerToHead[0] = SVector3(xpos,ypos,zpos);
	playerToHead[1] = SVector3(xpos,ypos + 60,zpos);
}

void CPlayer::updateBullets()
{
	myGun[m_equippedGun]->update();
}

void CPlayer::forceCamPos(SVector3 newpos){

	camx = newpos.x;
	camy = newpos.y;
	camz = newpos.z;
}

void CPlayer::shoot()
{
	if(myGun[m_equippedGun]->shoot(shotline,false))
	{
		character[m_activeChar].doMuzzleFlash();
		//set shot to true so level knows
		//to do ricochet and bullet holes
		playerData.shoot = true;
		m_shot = true;
		character[m_activeChar].SetTorsoAnimation("TORSO_ATTACK");
		playerData.shotLock++;

	}
}

void CPlayer::unlockShot()
{
	playerData.shotLock = 0;
}

void CPlayer::moveVector(SVector3 direct)
{
	newx += direct.x;
	newy += direct.y;
	newz += direct.z;

	updatePos();
}

SVector3 CPlayer::posVector(){

	SVector3 position;

	position.x = newx;
	position.y = newy;
	position.z = newz;

	return position;
}

bool CPlayer::shot(){

	return m_shot;
}

SVector3 CPlayer::radiusVector(){

	return m_radiusVector;

};

sendStruct CPlayer::getPositionData()
{
	return playerData;
}

void CPlayer::setPositionData()
{
	playerData.pos.x = xpos;
	playerData.pos.y = ypos;
	playerData.pos.z = zpos;

	playerData.lineAim.x = m_crosshair.x;
	playerData.lineAim.y = m_crosshair.y;
	playerData.lineAim.z = m_crosshair.z;

	playerData.yrot = yrot;
	playerData.xrot = xrot;
	playerData.state = state;

	playerData.health = m_health;

	playerData.gun = m_equippedGun;

	playerData.kills = m_kills;
	playerData.deaths = m_deaths;

	playerData.magazines = myGun[m_equippedGun]->m_currentMagazines;
	playerData.ammo = myGun[m_equippedGun]->m_currentAmmo;
}

void CPlayer::canJump(bool jump)
{
	m_canJump = jump;
}

void CPlayer::canStandUp(bool stand)
{
	m_canStandUp = stand;
}

bool CPlayer::checkForHit(SVector3 line[],SVector3 *shotPos){

	SVector3 closestPoint;
	SVector3 eRadius;
	SVector3 pos = SVector3(xpos,ypos,zpos);
	SVector3 rv= radiusVector();

	//get the closest point on line
	closestPoint = closestPointOnLine(line[0],line[1],pos);

	//make a unit sphere
	SVector3 unitSphere = closestPoint - pos;

	unitSphere = normalize(unitSphere);

	//scale to eRadius in ellipsoid space
	eRadius.x = pos.x + (unitSphere.x * rv.x);
	eRadius.y = pos.y + (unitSphere.y * rv.y);
	eRadius.z = pos.z + (unitSphere.z * rv.z);

	double ellip = distance(eRadius,pos);
	double intersec = distance(closestPoint,pos);

	if(intersec < ellip) 
	{
		*shotPos = closestPoint;

		return true;
	}

	return false;

}

void CPlayer::updateViaNetwork(sendStruct player2){

	//update all the players info with the packet recieved

	//firstly check the players connected
	playerData.connected = player2.connected;

	//do time based movement and then set the positions
	timeBasedMovement();

	xpos = player2.pos.x;
	ypos = player2.pos.y;
	zpos = player2.pos.z;

	newx = xpos;
	newy = ypos;
	newz = zpos;
	
	yrot = player2.yrot;
	xrot = player2.xrot;

	SVector3 currentPos(xpos,ypos,zpos);

	//life data
	m_health = player2.health;

	if(player2.health > 0) m_alive = true;

	if(m_alive)
	{
		setState(player2.state);
	}

	//guns and shooting
	if(m_equippedGun != player2.gun)
	{
			if((player2.gun >= 0) && (player2.gun < 4))
			{
				m_equippedGun = player2.gun;
				myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
				pickUp();
			}
	}

	if((m_equippedGun >= 0) && (m_equippedGun < 4))
	{
		myGun[m_equippedGun]->m_currentMagazines = player2.magazines;
		myGun[m_equippedGun]->m_currentAmmo = player2.ammo;
	}

	SVector3 scale = SVector3(1,1,1);
	m_muzzlePos = rotateAboutY(character[m_activeChar].bulletStartPos,scale,currentPos,yrot);

	shotline[1] = SVector3(player2.lineAim.x,player2.lineAim.y,player2.lineAim.z);
	shotline[0] = SVector3(m_muzzlePos.x,m_muzzlePos.y,m_muzzlePos.z);

	for(int i = 0; i < player2.shotLock; i++)
	{
		if(myGun[m_equippedGun]->shoot(shotline,true))
		{
			shoot();
			character[m_activeChar].doMuzzleFlash();
			player2.shoot = false;
		}
	}

	if(player2.shotLock > 0) player2.shotLock = 0;

	//stats
	m_kills = player2.kills;
	m_deaths = player2.deaths;

}


void CPlayer::reduceHealth(int ammount){

	if(m_health > 75) CSound::getInstance()->play(10);
	else if(m_health >= 50) CSound::getInstance()->play(11);
	else if(m_health < 50) CSound::getInstance()->play(12);

	m_health -= ammount;
	if(m_health < 0) m_health = 0;
	
}

void CPlayer::increaseHealth(int ammount){

	//CSound::getInstance()->play(4);

	/*m_health += ammount;

	if(m_health > 100)
	{
		m_health = 100;
	}*/
	
}

void CPlayer::increaseArmour(int ammount){

	//CSound::getInstance()->play(4);

	m_armour += ammount;

	if(m_armour > 100)
	{
		m_armour = 100;
	}
	
}

void CPlayer::assignRespawnPos(SVector3 respawnAt){

	nextSpawnPoint = respawnAt;

}

bool CPlayer::moveLine(SVector3 &timeLine0,SVector3 &timeLine1,SVector3 &timeLine2){

	timeLine0.x = xpos;
	timeLine0.y = ypos;
	timeLine0.z = zpos;

	timeLine2.x = newx;
	timeLine2.y = newy;
	timeLine2.z = newz;

	SVector3 firstCheck = timeLine0 + (normalize(timeLine2 - timeLine0) * 40);
	timeLine1 = firstCheck;

	return true;
}

void CPlayer::forcePos(SVector3 newPos,bool x, bool y, bool z){

	if(x) newx = newPos.x;
	if(y) newy = newPos.y;
	if(z) newz = newPos.z;

}

SVector3 CPlayer::camPosVector(){

	SVector3 camVect = SVector3(camx,camy,camz);
	
	return camVect;
}

SVector3 CPlayer::camRotVector(){

	SVector3 camVect = SVector3(camrx,camry,0);
	
	return camVect;
}

SVector3 CPlayer::getReadyForCollision(){

	SVector3 velocityVector(0,0,0);

	velocityVector.x = newx - xpos;
	velocityVector.y = newy - ypos;
	velocityVector.z = newz - zpos;

	newx = xpos;
	newy = ypos;
	newz = zpos;

	return velocityVector;
}

void CPlayer::addGunAmmo(int gun, int ammount){

	if(!gotGun[gun]) 
	{
		gotGun[gun] = true;
		m_equippedGun = gun;
		myGun[m_equippedGun]->linkGunModel(character[m_activeChar]);
		pickUp();

		myGun[gun]->addAmmo(ammount);
		myGun[gun]->initialAmmo();
	}
	else
	{
		myGun[gun]->addAmmo(ammount);
	}

	
}

void CPlayer::drawGun(int gun){
	
	myGun[gun]->pickUp();
}

void CPlayer::checkCollision(SVector3 pos, SVector3 radiusVector)
{
	//ELLIPSE
	SVector3 collider1Pos, collider2Pos;
	
	collider1Pos = posVector(); //collider1Pos.y = 0;
	collider2Pos = pos; //collider2Pos.y = 0;

	float distanceBetween = distance(collider1Pos,collider2Pos);

	SVector3 normalVector = normalize(collider1Pos - collider2Pos);

	SVector3 collider1Ellipse,collider2Ellipse,ellipse1,ellipse2;

	ellipse1 = m_radiusVector; //ellipse1.y = 0;
	ellipse2 = radiusVector; //ellipse2.y = 0;

	collider1Ellipse.x = collider1Pos.x + (normalVector.x * ellipse1.x);
	collider1Ellipse.y = collider1Pos.y + (normalVector.y * ellipse1.y);
	collider1Ellipse.z = collider1Pos.z + (normalVector.z * ellipse1.z);

	collider2Ellipse.x = collider2Pos.x + (normalVector.x * ellipse2.x);
	collider2Ellipse.y = collider2Pos.y + (normalVector.y * ellipse2.y);
	collider2Ellipse.z = collider2Pos.z + (normalVector.z * ellipse2.z);

	float distance1 = distance(collider1Pos,collider1Ellipse);
	float distance2 = distance(collider2Pos,collider2Ellipse);

	float totalDistance = distance1 + distance2;

	if(distanceBetween < totalDistance)
	{
		SVector3 offset;

		float distanceOver = totalDistance - distanceBetween;
		
		offset = normalVector * distanceOver;

		offset.y = 0;

		moveVector(offset);

	}
}

void CPlayer::disconnect()
{
	playerData.connected = -1;
}

void CPlayer::stayConnected()
{
	playerData.connected = 1;
}

void CPlayer::setIdle()
{
	character[m_activeChar].SetTorsoAnimation("TORSO_STAND");
	state = -1;
	setState(LEGS_IDLE);
}

