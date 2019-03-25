#include <string>
#include <fstream>
#include "Sprite.h"
#include "Definitions.h"

using namespace std;



CSprite::CSprite() {

	m_spriteImage = NULL;
	m_totalAnimationframes = NULL;
	m_spriteWidths = NULL;
	m_spriteHeights = NULL;
	m_numberofState = 0;
	m_binitialised = false;
	m_bdisplaytext = false;
	m_alive = false;
	m_runtimer = 0;

	colour_white = makecol(225,222,194);
	colour_grey = makecol(128,128,128);
	colour_yellow = makecol(225,222,0);
	colour_green = makecol(0,190,0);
	colour_dkred = makecol(190,0,0);
	colour_blue = makecol(0,0,190);
	

}

CSprite::~CSprite() {

	if (m_binitialised) {
		if ((m_spriteImage!=NULL)&&(m_totalAnimationframes!=NULL)) {
			for (int i=0;i<m_numberofState;i++) {
				if (m_spriteImage[i]!=NULL) {
					for (int j=0;j<m_totalAnimationframes[i];j++) {
						if (m_spriteImage[i][j]!=NULL) {
							destroy_bitmap(m_spriteImage[i][j]);
							m_spriteImage[i][j] = NULL;
						}
					}

					delete m_spriteImage[i];
					m_spriteImage[i] = NULL;

				}
			}

			delete m_spriteImage;
			m_spriteImage = NULL;

			delete m_totalAnimationframes;

		}

		if (m_spriteWidths != NULL) {
			delete m_spriteWidths;
			m_spriteWidths = NULL;
		}

		if (m_spriteHeights != NULL) {
			delete m_spriteHeights;
			m_spriteHeights = NULL;
		}
	}

	m_totalAnimationframes = 0;

	m_binitialised = false;


}

void CSprite::drawSprite(BITMAP* buffer, CBackground *bkground) {

	if (!m_binitialised) {

		return;

	}

	int topborder, leftborder, bottomborder, rightborder;

	bkground->getVisibleborder(&topborder, &leftborder, &bottomborder, &rightborder);

	if ((m_xpos+m_spriteWidths[m_currentState]/2<leftborder)||(m_xpos-m_spriteWidths[m_currentState]/2>rightborder)||(m_ypos+m_spriteHeights[m_currentState]/2<topborder)||(m_ypos-m_spriteHeights[m_currentState]/2>bottomborder))
		return;

	int xscreenpos, yscreenpos;
	double dx, dy;

	bkground->getScreencoordinate(m_xpos, m_ypos, &xscreenpos, &yscreenpos);
	bkground->getScreentoWindowRatio(&dx, &dy);

	masked_stretch_blit(m_spriteImage[m_currentState][m_currentAnimationframe], buffer, 0, 0, m_spriteWidths[m_currentState], m_spriteHeights[m_currentState], xscreenpos-m_spriteWidths[m_currentState]/2*dx, yscreenpos-m_spriteHeights[m_currentState]/2*dy, m_spriteWidths[m_currentState]*dx, m_spriteHeights[m_currentState]*dy);

	if (m_bdisplaytext)
		textout_centre(buffer,font,m_text,xscreenpos, yscreenpos-m_spriteHeights[m_currentState]/2-10, makecol(255,255,255));

}


int CSprite::setSpriteState(int state) {

	if (!m_binitialised) {

		return 0;

	}

	if ((state >= m_numberofState)||(state < 0)) {

		allegro_message("The supplied state is out of range");

		return 0;

	}

	if (state!=m_currentState) {
		m_currentState = state;
		m_currentAnimationframe = 0;
	}

	return 1;

}


void CSprite::incrementAnimationframe() {

	if (!m_binitialised) {

		return;

	}

	m_currentAnimationframe++;

	if (m_currentAnimationframe>=m_totalAnimationframes[m_currentState])
		m_currentAnimationframe = 0;

}

void CSprite::setpos(int xpos, int ypos) {

	m_xpos = xpos;
	m_ypos = ypos;

}

void CSprite::getpos(int* xpos, int* ypos){

	*xpos = m_xpos;
	*ypos = m_ypos;

}

bool CSprite::checkCollisionwithBackground(CBackground *bkground, int xpos, int ypos) {

	if (!m_binitialised)
		return true;

	int xcoord[4];
	int ycoord[4];

	xcoord[0] = xpos-m_spriteWidths[m_currentState]/2;
	xcoord[1] = xpos+m_spriteWidths[m_currentState]/2;
	xcoord[2] = xcoord[1];
	xcoord[3] = xcoord[0];

	ycoord[0] = ypos-m_spriteWidths[m_currentState]/2;
	ycoord[1] = ycoord[0];
	ycoord[2] = ypos+m_spriteWidths[m_currentState]/2;
	ycoord[3] = ycoord[2];

	return bkground->checkCollision(xcoord, ycoord, 4);

	/*

	(0) ----- (1)
	 |         |
	 |         |
	 |         |
	 |         |
	(3) ----- (2)

	*/

}

bool CSprite::getVertices(int* xvert, int *yvert) {

	if (!m_binitialised)
		return false;

	if (m_bdisplaytext)
		return false;

	*(xvert+0) = m_xpos-m_spriteWidths[m_currentState]/2;
	*(xvert+1) = m_xpos+m_spriteWidths[m_currentState]/2;

	*(yvert+0) = m_ypos-m_spriteWidths[m_currentState]/2;
	*(yvert+1) = m_ypos+m_spriteWidths[m_currentState]/2;

	return true;

}


bool CSprite::checkCollisionwithSprite(CSprite* sprite, int xpos, int ypos) {

	if (!m_binitialised)
		return false;

	if (m_bdisplaytext)
		return false;

	int xcoord[2];
	int ycoord[2];

	xcoord[0] = xpos-m_spriteWidths[m_currentState]/2;
	xcoord[1] = xpos+m_spriteWidths[m_currentState]/2;

	ycoord[0] = ypos-m_spriteWidths[m_currentState]/2;
	ycoord[1] = ypos+m_spriteWidths[m_currentState]/2;

	int xvert[2];
	int yvert[2];

	sprite->getVertices(xvert,yvert);

	for (int i=0;i<2;i++) {
		for (int j=0;j<2;j++) {
			if ((xvert[i]>=xcoord[0])&&(xvert[i]<=xcoord[1])&&(yvert[j]>=ycoord[0])&&(yvert[j]<=ycoord[1]))
				return true;
		}
	}

	for (i=0;i<2;i++) {
		for (int j=0;j<2;j++) {
			if ((xcoord[i]>=xvert[0])&&(xcoord[i]<=xvert[1])&&(ycoord[j]>=yvert[0])&&(ycoord[j]<=yvert[1]))
				return true;
		}
	}


	return false;



	/*

	(0) ----- (1)
	 |         |
	 |         |
	 |         |
	 |         |
	(3) ----- (2)

	*/

}

bool CSprite::hasCollidedwithSprite(CSprite* sprite) {

	if (!m_binitialised)
		return false;

	if (m_bdisplaytext)
		return false;

	int xcoord[2];
	int ycoord[2];

	xcoord[0] = m_xpos-m_spriteWidths[m_currentState]/2;
	xcoord[1] = m_xpos+m_spriteWidths[m_currentState]/2;

	ycoord[0] = m_ypos-m_spriteWidths[m_currentState]/2;
	ycoord[1] = m_ypos+m_spriteWidths[m_currentState]/2;

	int xvert[2];
	int yvert[2];

	sprite->getVertices(xvert,yvert);

	for (int i=0;i<2;i++) {
		for (int j=0;j<2;j++) {
			if ((xvert[i]>=xcoord[0])&&(xvert[i]<=xcoord[1])&&(yvert[j]>=ycoord[0])&&(yvert[j]<=ycoord[1]))
				return true;
		}
	}

	return false;



	/*

	(0) ----- (1)
	 |         |
	 |         |
	 |         |
	 |         |
	(3) ----- (2)

	*/

}


void CSprite::setPopupText(const char* text) {

	if (!m_binitialised) {

		return;

	}

	strncpy(m_text,text,256);


}



void CSprite::togglePopupText() {

	if (!m_binitialised) {

		return;

	}

	m_bdisplaytext = !m_bdisplaytext;

}
void CSprite::displayTextPopup(BITMAP* buffer, CBackground *bkground) {

	if (!m_binitialised) {

		return;

	}

	int xscreenpos, yscreenpos;

	bkground->getScreencoordinate(m_xpos, m_ypos, &xscreenpos, &yscreenpos);

	textout_centre(buffer,font,m_text,xscreenpos, yscreenpos, 0);

}

bool CSprite::initialise(const char filename[]) {

	if (m_binitialised) {
		allegro_message("Sprite object can not be initialised twice");
		return false;
	}


	ifstream infile;

	infile.open(filename);
	if (!infile.is_open()) {
		allegro_message("Unable to open initialisation file");
		return false;
	}

	char buf[1024];

	infile.getline(buf,1024,'\n');
	if (strcmp(buf,"Sprite Data File")) {
		allegro_message("The supplied file is not a sprite data file");
		infile.close();
		return false;
	}

	bool keep_reading = true;
	while (keep_reading) {
		infile.getline(buf,1024,'\n');
		if ((buf[0]!='#')&&(buf[0]!=NULL))
			keep_reading = false;
	}

	if (infile.eof()) {
		allegro_message("The file contains no number of states");
		infile.close();
		return false;
	}

	m_numberofState = atoi(buf);
	if (m_numberofState<=0) {
		allegro_message("The file contains negative number of states");
		infile.close();
		return false;
	}

	m_spriteImage = new BITMAP**[m_numberofState];
	if (m_spriteImage==NULL) {

		allegro_message("The program failed to allocate memory to hold sprite images (level 1)");
		infile.close();
		return false;

	}

	m_totalAnimationframes = new int[m_numberofState];
	if (m_totalAnimationframes==NULL) {
		allegro_message("The program failed to allocate memory to hold total animation frames");
		delete m_spriteImage;
		m_spriteImage = NULL;
		infile.close();
		return false;
	}

	m_spriteWidths = new int[m_numberofState];
	if (m_spriteWidths==NULL) {
		allegro_message("The program failed to allocate memory to hold total animation frames");

		delete m_spriteImage;
		m_spriteImage = NULL;

		delete m_totalAnimationframes;
		m_totalAnimationframes = NULL;

		infile.close();
		return false;
	}

	m_spriteHeights = new int[m_numberofState];
	if (m_spriteHeights==NULL) {
		allegro_message("The program failed to allocate memory to hold total animation frames");

		delete m_spriteImage;
		m_spriteImage = NULL;
		
		delete m_totalAnimationframes;
		m_totalAnimationframes = NULL;

		delete m_spriteWidths;
		m_spriteWidths = NULL;

		infile.close();
		return false;
	}

	for (int i=0;i<m_numberofState;i++) {
		keep_reading = true;
		while (keep_reading) {
			infile.getline(buf,1024,'\n');
			if ((buf[0]!='#')&&(buf[0]!=NULL))
				keep_reading = false;
		}

		if (infile.eof()) {
			allegro_message("Unexpected end of files encountered");
				
			delete m_totalAnimationframes;
			m_totalAnimationframes = NULL;

			delete m_spriteWidths;
			m_spriteWidths = NULL;

			int imax=i;

			for (int j=0;j<imax;j++) {
				delete m_spriteImage[j];
				m_spriteImage[j] = NULL;
			}
			
			delete m_spriteImage;
			m_spriteImage = NULL;


			infile.close();
			return false;
		}

		BITMAP *temp = load_bitmap(buf,0);
		if (temp==NULL) {
			allegro_message("Unable to load a sprite image");
				
			delete m_totalAnimationframes;
			m_totalAnimationframes = NULL;

			delete m_spriteWidths;
			m_spriteWidths = NULL;

			int imax=i;

			for (int j=0;j<imax;j++) {
				delete m_spriteImage[j];
				m_spriteImage[j] = NULL;
			}

			delete m_spriteImage;
			m_spriteImage = NULL;

			infile.close();
			return false;
		}

		infile.getline(buf,1024,' ');
		m_spriteWidths[i] = atoi(buf);

		infile.getline(buf,1024,' ');
		m_spriteHeights[i] = atoi(buf);

		infile.getline(buf,1024,'\n');
		m_totalAnimationframes[i] = atoi(buf);

		m_spriteImage[i] = new BITMAP*[m_totalAnimationframes[i]];

		if (m_spriteImage[i] == NULL) {
			allegro_message("The program failed to allocate memory to hold total animation frames");
		
			delete m_totalAnimationframes;
			m_totalAnimationframes = NULL;

			delete m_spriteWidths;
			m_spriteWidths = NULL;

			int imax=i+1;

			for (int j=0;j<imax;j++) {
				delete m_spriteImage[j];
				m_spriteImage[j] = NULL;
			}

			delete m_spriteImage;
			m_spriteImage = NULL;

			m_meat = true;
			m_bone = true;
			m_sinew = true;

			m_treeloot1 = true;
			m_treeloot2 = true;
			m_treeloot3 = true;

			destroy_bitmap(temp);

			infile.close();
			return false;
		}

		for (int j=0; j<m_totalAnimationframes[i]; j++) {

			m_spriteImage[i][j] = create_bitmap(m_spriteWidths[i], m_spriteHeights[i]);
			blit(temp, m_spriteImage[i][j], j*m_spriteWidths[i], 0, 0, 0, m_spriteWidths[i], m_spriteHeights[i]);

		}

		destroy_bitmap(temp);
	}

	infile.close();

	m_currentAnimationframe = 0;

	m_currentState = 0;

	m_xpos = 0;
	m_ypos = 0;

	m_binitialised = true;
	infile.close();
	
	return true;

}




void CSprite::spawn(int state, int lifespan, int xpos, int ypos, int speedx, int speedy) {

	setSpriteState(state);
	m_lifespan = lifespan;
	m_speedx = speedx;
	m_speedy = speedy;

	m_xpos = xpos;
	m_ypos = ypos;

	m_alive = true;

}


void CSprite::updatepos() {

	m_xpos += m_speedx;
	m_ypos += m_speedy;

	if (m_lifespan!=999999)
		m_lifespan--;

	if (m_lifespan < 0)
		m_alive = false;


}


void CSprite::despawn() {

	m_lifespan = 0;
	m_alive = false;


}

void CSprite::getspeed(int* xspeed, int* yspeed) {

	*xspeed = m_speedx;
	*yspeed = m_speedy;

}


void CSprite::setSpeed(int xspeed, int yspeed) {

	m_speedx = xspeed;
	m_speedy = yspeed;

}

void CSprite::GazellRoam() 
{
	m_roamtimer--;
	
	if(m_roamtimer <= 0)
	{
	
	time_t seconds;
	time(&seconds);
	
	srand((unsigned int)seconds);
	m_newstate = rand()%5;

	if(m_newstate == 0)
	{
		m_speedx = 1;
		m_speedy = 0;
		setSpriteState(m_newstate);
	}
	else if(m_newstate == 1)
	{
		m_speedx = 0;
		m_speedy = -1;
		setSpriteState(m_newstate);
	}
	else if(m_newstate == 2)
	{
		m_speedx = -1;
		m_speedy = 0;
		setSpriteState(m_newstate);
	}
	else if(m_newstate == 3)
	{
		m_speedx = 0;
		m_speedy = 1;
		setSpriteState(m_newstate);
	}
	else if(m_newstate == 4)
	{
		m_speedx = 0;
		m_speedy = 0;
	}
		
		timefluctuator = rand()%3;
		
		setRoam(200 * timefluctuator);
	}



}

void CSprite::setRoam(int roamtimer) {

	m_roamtimer = roamtimer;

}

void CSprite::GazellOutsideBound(int backgroundwidth, int backgroundheight) {
		
		int gx,gy;
	
		getpos(&gx, &gy);

		if(gx <= 0)
		{
			setSpriteState(0);
			setSpeed(1,0);

		}
		else if(gy <= 0)
		{
			setSpriteState(3);
			setSpeed(0,1);
		}
		else if(gx >= backgroundwidth)
		{
			setSpriteState(2);
			setSpeed(-1,0);
		}
		else if(gy >= backgroundheight)
		{
			setSpriteState(1);
			setSpeed(0,-1);
		}

}

void CSprite::setGazellHealth(int health)
{
	m_health = health;
}

void CSprite::drawStaticInfo(BITMAP *buffer, int mode)
{

	int halfwidth = m_spriteWidths[m_currentState]/6;
	int imagex = 598 - halfwidth;
		
	if(mode == 0)
	{
		textprintf_centre_ex(buffer, font, 598,655, colour_white,-1, "Tree");
	}
	else if (mode == 1)
	{
		textprintf_centre_ex(buffer, font, 598,655, colour_white,-1, "Rocks");
	}
	masked_stretch_blit(m_spriteImage[m_currentState][m_currentAnimationframe], buffer, 0, 0, m_spriteWidths[m_currentState], m_spriteHeights[m_currentState],imagex,670,m_spriteWidths[m_currentState] / 3, m_spriteHeights[m_currentState] / 3);
	
}

void CSprite::drawGazellInfo(BITMAP *buffer)
{

	int halfwidth = m_spriteWidths[m_currentState]/2;
	int imagex = 598 - halfwidth - 10;
		
		
	textprintf_centre_ex(buffer, font, 598,655, colour_white,-1, "Gazelle");
	masked_stretch_blit(m_spriteImage[m_currentState][m_currentAnimationframe], buffer, 0, 0, m_spriteWidths[m_currentState], m_spriteHeights[m_currentState],imagex,670, m_spriteWidths[m_currentState] + 20, m_spriteHeights[m_currentState] + 20);
	rectfill(buffer, 520, 730,520+m_health * 10,740, colour_dkred);
}

void CSprite::cookingFire(BITMAP *buffer, BITMAP *cookingbg, int fx, int fy, bool cooking)
{

	int barcol;
	int barx1, barx2, bary1, bary2;

	barx1 = fx - 50;
	bary1 = fy - 40;
	bary2 = fy - 30;
	barx2 = barx1 + m_cooktimer;

	masked_stretch_blit(cookingbg,buffer,0,0,106,16,barx1 - 3, bary1 - 3, 106, 16);
	
	if(cooking)
	{
		if(m_cooktimer > 0)
		m_cooktimer--;
	

		if(m_cooktimer > 40)
		{
			barcol = colour_blue;
		}
		else if((m_cooktimer <= 40) && (m_cooktimer > 15))
		{
			barcol = colour_yellow;
		}
		else
		{
			barcol = colour_dkred;
		}

		rectfill(buffer,barx1, bary1, barx2, bary2, barcol);
	}



}

void CSprite::startCooking(int timer)
{
	m_cooktimer = timer;
}

void CSprite::timerCount()
{
	if(m_returnTimer > 0)
		m_returnTimer --;
}

void CSprite::SetTrap(int hours,int minutes,int traplvl)
{
	time_t seconds;
	time(&seconds);
	
	srand((unsigned int)seconds);

	int formula = (10 * (rand()%7 +1)) / traplvl;
	
	m_deactiveM = minutes + formula;
	m_deactiveH = hours;

	if(m_deactiveM > 60)
	{
		m_deactiveH++;
		m_deactiveM = m_deactiveM - 60;
	}

	if(m_deactiveH > 23)
	{
		m_deactiveH = 0;
	}


}

void CSprite::CheckTrapDeactive(int hours, int minutes, BITMAP *buffer, int *state)
{
	int colour_white = makecol(255,255,255);
	int m_state;

	if((m_deactiveM == minutes) && (m_deactiveH == hours))
	{
			time_t seconds;
			time(&seconds);

			m_deactiveM = 0;
			m_deactiveH = 0;

			srand((unsigned int)seconds);
			m_trapHasCaught = rand()% 2;

			if(m_trapHasCaught)
			{
				m_state = 3;
				setLootObjects(RABBIT,0,0,0,0,0);
			}
			else
			{
				m_state = 2;
			}

			*state = m_state;
		
	}



}

void CSprite::drawTrapInfo(BITMAP *buffer,int trapType)
{

	int halfwidth = m_spriteWidths[m_currentState]/2;
	int imagex = 598 - halfwidth;
		
	if(trapType == 0)
	{
	textprintf_centre_ex(buffer, font, 598,655, colour_white,-1, "Sapling");
	}
	
	masked_stretch_blit(m_spriteImage[m_currentState][m_currentAnimationframe], buffer, 0, 0, m_spriteWidths[m_currentState], m_spriteHeights[m_currentState],imagex,670, m_spriteWidths[m_currentState], m_spriteHeights[m_currentState]);
	
	if(m_currentState == UNSET)
	{
	textprintf_centre_ex(buffer, font, 598,730, colour_white,-1, "Click To Set Trap");
	}
	else if(m_currentState == SET)
	{
	textprintf_centre_ex(buffer, font, 598,730, colour_white,-1, "Trap Set And Waiting");
	}
	else if((m_currentState == CAUGHT) ||(m_currentState == FAIL))
	{
	textprintf_centre_ex(buffer, font, 598,728, colour_white,-1, "Click To Inspect");
	textprintf_centre_ex(buffer, font, 598,738, colour_white,-1, "Right Click To Reset Trap");
	}


}

void CSprite::setLifeSpan(int lifespan)
{
	m_lifespan = lifespan;
}

void CSprite::detectPlayer(int animaltype, int playerx, int playery, int playerStealthMode)
{
	if(m_runtimer == 0)
	{	
	if(animaltype == 1) // gazelle
	{
		if(playerStealthMode == 0) // standing
		{
			if((m_currentState == 0) && (playerx < m_xpos + 100) && (playerx > m_xpos) && (playery > m_ypos - 50) && (playery < m_ypos + 50))
			{
				m_runtimer = 50;
				switchstates();
			}
			else if((m_currentState == 2) && (playerx > m_xpos - 100) && (playerx < m_xpos)  && (playery > m_ypos - 50) && (playery < m_ypos + 50))
			{
				m_runtimer = 50;
				switchstates();
			}
			else if((m_currentState == 1) && (playery > m_ypos - 100) && (playery < m_ypos)  && (playerx > m_xpos - 50) && (playerx < m_xpos + 50))
			{
				m_runtimer = 50;
				switchstates();
			}
			else if((m_currentState == 3) && (playery < m_ypos + 100) && (playery > m_ypos)  && (playerx > m_xpos - 50) && (playerx < m_xpos + 50))
			{
				m_runtimer = 50;
				switchstates();
			}


		}

	}
	}

}

void CSprite::runTimerCount()
{
	if(m_runtimer > 0)
		m_runtimer--;
}

void CSprite::runAway()
{
	int multiplier;
	
	if(m_runtimer > 0)
	{
		multiplier = m_health / 3;
	}
	else
	{
		multiplier = 1;
	}

		switch (m_currentState) 
		{
			case 0:
				setSpeed(1 * multiplier,0);
			break;
			case 1:
				setSpeed(0,-1 * multiplier);
			break;
			case 2:
				setSpeed(-1 * multiplier,0);
			break;
			case 3:
				setSpeed(0,1 * multiplier);
			break;
			default:
			break;
		}



}

void CSprite::switchstates()
{
	int multiplier;
	
	if(m_runtimer > 0)
	{
		multiplier = m_health / 3;
	}
	else
	{
		multiplier = 1;
	}
	
	
	switch (m_currentState) 
		{
			case 0:
				m_currentState = 2;
				setSpeed(-1 * multiplier,0);
			break;
			case 1:
				m_currentState = 3;
				setSpeed(0,1 * multiplier);
			break;
			case 2:
				m_currentState = 0;
				setSpeed(1 * multiplier,0);
			break;
			case 3:
				m_currentState = 1;
				setSpeed(0,-1 * multiplier);
			break;
			default:
			break;
		}
	
}

void CSprite::setLootObjects(int loot1, int loot2, int loot3, int loot4, int loot5, int loot6)
{
	m_lootitem[0] = loot1;
	m_lootitem[1] = loot2;
	m_lootitem[2] = loot3;
	m_lootitem[3] = loot4;
	m_lootitem[4] = loot5;
	m_lootitem[5] = loot6;
}

void CSprite::getLootObjects(int *loot1, int *loot2, int *loot3, int *loot4, int *loot5, int *loot6)
{
	*loot1 = m_lootitem[0];
	*loot2 = m_lootitem[1];
	*loot3 = m_lootitem[2];
	*loot4 = m_lootitem[3];
	*loot5 = m_lootitem[4];
	*loot6 = m_lootitem[5];
}


