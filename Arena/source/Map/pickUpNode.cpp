#include "pickUpNode.h"
#include "timerSingleton.h"
#include "soundSingleton.h"

CPNode::CPNode(){

	m_pos = SVector3(150,80,1200);
	m_type = GUN_PLASMA;
	m_radius = 50;
	m_alive = true;
	m_timeLimit = 30;
	m_rotation = 0;

}

CPNode::CPNode(int type, SVector3 pos){

	m_pos = pos;
	m_type = type;
	m_radius = 50;
	m_alive = true;
	m_timeLimit = 30;
	m_rotation = 0;

}

CPNode::~CPNode()
{

}

const char* CPNode::name()
{
	switch(m_type)
	{
		case GUN_MACHINE:
		
			return "Machine Gun";
		
		break;

		case GUN_SHOT:

			return "Shotgun";

		break;

		case GUN_PLASMA:

			return "Plasma Gun";

		break;

		case GUN_LAZER:

			return "Lazer Gun";

		break;
	}

	return "";

}

void CPNode::draw(CPlayer &alj){

	int model = 0;
	glDisable(GL_LIGHTING);

	switch(m_type)
	{
	case HEALTH25:
	glColor3f(0.8,0.2,0);
	model = -1;
	break;
	case ARMOUR25:
	glColor3f(0.1,0.8,0);
	model = -1;
	break;
	case HEALTH50:
	glColor3f(0.8,0.2,0);
	model = -1;
	break;
	case ARMOUR50:
	glColor3f(0.1,0.8,0);
	model = -1;
	break;
	case HEALTH100:
	glColor3f(0.8,0.2,0);
	model = -1;
	break;
	case ARMOUR100:
	glColor3f(0.1,0.8,0);
	model = -1;
	break;
	case GUN_MACHINE:
	glColor4f(1,1,1,1);
	model = 0;
	break;
	case GUN_SHOT:
	glColor4f(1,1,1,1);
	model = 1;
	break;
	case GUN_PLASMA:
	glColor4f(1,1,1,1);
	model = 2;
	break;
	case GUN_LAZER:
	glColor4f(1,1,1,1);
	model = 3;
	break;
	}

	//draw
	if(m_alive)
	{
		glPushMatrix();
		glTranslated(m_pos.x,m_pos.y,m_pos.z);
		glRotated(m_rotation,0,1,0);
		if(model == -1) glutSolidCube(10);
		else alj.drawGun(model);
		glPopMatrix();
		if(CTimer::getInstance()->getFps() != 0) m_rotation += 300 / CTimer::getInstance()->getFps();
	}

	glEnable(GL_LIGHTING);

}

void CPNode::check(CPlayer &alj){

	m_timer += CTimer::getInstance()->animBaseSpeed();
	if(m_timer >= m_timeLimit) m_alive = true;

	//check for pickup
	if((distance(alj.posVector(),m_pos) < m_radius) && (m_alive))
	{
		switch(m_type)
		{
		case HEALTH25:
		alj.increaseHealth(25);
		break;
		case HEALTH50:
		alj.increaseHealth(50);
		break;
		case HEALTH100:
		alj.increaseHealth(100);
		break;
		case ARMOUR25:
		alj.increaseArmour(25);
		break;
		case ARMOUR50:
		alj.increaseArmour(50);
		break;
		case ARMOUR100:
		alj.increaseArmour(100);
		break;
		case GUN_MACHINE:
		alj.addGunAmmo(0,10);
		break;
		case GUN_PLASMA:
		alj.addGunAmmo(2,10);
		break;
		case GUN_SHOT:
		alj.addGunAmmo(1,10);
		break;
		case GUN_LAZER:
		alj.addGunAmmo(3,10);
		break;
		}

		m_alive = false;
		m_timer = 0;

		char message[128];

		strcpy(message,"You picked up some ");
		strcat(message,name());
		strcat(message," ammo");

		alj.addMessage(message);

		CSound::getInstance()->play(1);
	}
}

void CPNode::render(CWeapons *weapon[])
{
	int t = 0;

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	switch(m_type)
	{
		case GUN_MACHINE:
		t = 0;
		break;
		case GUN_PLASMA:
		t = 2;
		break;
		case GUN_SHOT:
		t = 1;
		break;
		case GUN_LAZER:
		t = 3;
		break;
	}

	if(m_alive)
	{
		glPushMatrix();
		glTranslated(m_pos.x,m_pos.y,m_pos.z);
		glRotated(m_rotation,0,1,0);
		weapon[t]->pickUp();
		glPopMatrix();

		m_rotation += CTimer::getInstance()->animBaseSpeed();
	}
}

void CPNode::setType(int type){

	m_type = type;

}

void CPNode::saveData(ofstream &file){

	file << "pickups " << m_type << " " << m_pos.x << " " << m_pos.y << " "
	<< m_pos.z << "\n";

}

void CPNode::highlight(){

	glDisable(GL_LIGHTING);
	glColor4f(0.8,0,0.1,0.5);
	
	glPushMatrix();
	glTranslated(m_pos.x,m_pos.y,m_pos.z);
	glutSolidCube(70);
	glPopMatrix();
	glDisable(GL_LIGHTING);
}

void CPNode::setPos(SVector3 pos){

	m_pos = pos;

}