#include <fstream>
#include "PlayerSprite.h"

using namespace std;

CPlayerSprite::CPlayerSprite() {

	CSprite::CSprite();

	m_parameternumber = 0;
	m_parameter = NULL;
	m_avatarInfo = NULL;

}

CPlayerSprite::~CPlayerSprite() {

	CSprite::~CSprite();

	m_parameternumber = 0;

	if (m_parameter!=NULL)
		delete m_parameter;
	m_parameter = NULL;

	if (m_avatarInfo!=NULL)
		delete m_avatarInfo;
	m_avatarInfo = NULL;

}

bool CPlayerSprite::initialise(const char filename[]) {

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

	keep_reading = true;
	while (keep_reading) {
		infile.getline(buf,1024,'\n');
		if ((buf[0]!='#')&&(buf[0]!=NULL))
			keep_reading = false;
	}

	m_currentAnimationframe = 0;

	m_currentState = 0;

	m_xpos = 0;
	m_ypos = 0;

	m_binitialised = true;

	if (infile.eof()) {
		infile.close();
		return true;
	}

	m_parameternumber = atoi(buf);

	if (m_parameternumber<0)
		m_parameternumber = 0;

	keep_reading = true;
	while (keep_reading) {
		infile.getline(buf,1024,'\n');
		if ((buf[0]!='#')&&(buf[0]!=NULL))
			keep_reading = false;
	}

	if (infile.eof()) {
		infile.close();
		return true;
	}

	char avatarfile[256];
	strcpy(avatarfile, buf);
 
	infile.getline(buf,1024,' ');
	int playerAvatar_x = atoi(buf);

	infile.getline(buf,1024,' ');
	int playerAvatar_y = atoi(buf);

	infile.getline(buf,1024,' ');
	int playerAvatar_w = atoi(buf);

	infile.getline(buf,1024,' ');
	int playerAvatar_h = atoi(buf);

	infile.getline(buf,1024,'\n');
	int bar_w = atoi(buf);

	m_avatarInfo = new CAvatarInfo();
	if (!m_avatarInfo->initialise(avatarfile, playerAvatar_x, playerAvatar_y, playerAvatar_w, playerAvatar_h, bar_w)) {
		delete m_avatarInfo;
		m_avatarInfo = NULL;
		infile.close();
	}

	int pnum = m_avatarInfo->getTotalNumberofParameter();

	if (pnum>m_parameternumber)
		m_parameternumber = pnum;

	if (m_parameternumber > 0) {
		m_parameter = new double[m_parameternumber];
		for (int i=0;i<pnum;i++) {
			double td;
			m_avatarInfo->getParameterValue(i,&td);
			m_parameter[i]=td;
		}

		for (i=pnum;i<m_parameternumber;i++)
			m_parameter[i]=0;
	}


	infile.close();
	return true;

}

void CPlayerSprite::drawSprite(BITMAP* buffer,CBackground *bkground) {

	CSprite::drawSprite(buffer, bkground);

}


void CPlayerSprite::drawAvatar(BITMAP *buffer) {

	if (!m_binitialised)
		return;
	

	if (m_avatarInfo!=NULL) {
		m_avatarInfo->drawAvatarinfo(buffer);
	}

}

bool CPlayerSprite::setParameterValue(int parameterindex, double value) {

	if (!m_binitialised) {

		allegro_message("The Player Object has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_parameternumber)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	m_parameter[parameterindex] = value;

	if (m_avatarInfo!=NULL) {
		int pnum =	m_avatarInfo->getTotalNumberofParameter();
		if (parameterindex<pnum)
			m_avatarInfo->setParameterValue(parameterindex,value);
	}

	return true;

}

bool CPlayerSprite::getParameterValue(int parameterindex, double* value) {

	if (!m_binitialised) {

		allegro_message("The Player Object has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_parameternumber)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	*value = m_parameter[parameterindex];

	return true;

}

bool CPlayerSprite::isInRange(int xpos,int ypos)
{
	if((xpos > mouse_x - 150) && (ypos > mouse_y - 150) && (xpos < mouse_x + 150) && (ypos < mouse_y + 150))
	{
		return true;
	}
	else
	{
		return false;
	}
}