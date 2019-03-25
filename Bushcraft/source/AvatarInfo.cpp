#include <string>
#include <fstream>
#include "AvatarInfo.h"

using namespace std;

CAvatarInfo::CAvatarInfo() {

	m_avatarImage = NULL;
	m_binitialised = false;
	m_topleftx = 0;
	m_toplefty = 0;
	m_barWidth =0; 
	m_height = 0;
	m_avatarWidth = 0;
	m_totalnumberofParameter = 0;
	m_parameterText = NULL;
	m_parameterValue = NULL;
	m_parameterColour = NULL;

}

CAvatarInfo::~CAvatarInfo() {

	if (m_avatarImage != NULL) {
		destroy_bitmap(m_avatarImage);
		m_avatarImage = NULL;
	}

	m_binitialised = false;
	m_topleftx = 0;
	m_toplefty = 0;
	m_barWidth =0; 
	m_height = 0;
	m_avatarWidth = 0;

	if (m_parameterText != NULL) {
		for (int i=0;i<m_totalnumberofParameter;i++) {
			if (m_parameterText[i]!=NULL)
				delete m_parameterText[i];
			m_parameterText[i]=NULL;
		}
		delete m_parameterText;
		m_parameterText=NULL;
	}

	if (m_parameterValue != NULL) {
		delete m_parameterValue;
		m_parameterValue = NULL;
	}

	if (m_parameterColour != NULL) {
		delete m_parameterColour;
		m_parameterColour = NULL;
	}

	m_totalnumberofParameter = 0;

}

bool CAvatarInfo::initialise(const char filename[], int topleftx, int toplefty, int avatarWidth, int avatarHeight, int barWidth, int parameternumber, char** parameterText, double* parameterValue, int* parameterColour) {

	if (m_binitialised) {

		allegro_message("The Avatar Info has already been initialised");
		return false;

	}

	m_avatarImage = load_bitmap(filename,0);

	if (m_avatarImage==NULL) {

		allegro_message(filename);
		return false;

	}

	m_topleftx = topleftx;
	m_toplefty = toplefty;
	m_barWidth = barWidth; 
	m_height = avatarHeight;
	m_avatarWidth = avatarWidth;

	m_totalnumberofParameter = parameternumber;

	if (m_totalnumberofParameter>0) {

		m_parameterText = new char*[m_totalnumberofParameter];
		m_parameterValue = new double[m_totalnumberofParameter];
		m_parameterColour = new int[m_totalnumberofParameter];

		for (int i=0;i<m_totalnumberofParameter;i++) {
			m_parameterText[i] = new char[256];
			strcpy(m_parameterText[i],parameterText[i]);
			m_parameterValue[i] = parameterValue[i];
			m_parameterColour[i] = parameterColour[i];
		}
	}

	m_binitialised = true;

	return true;

}


bool CAvatarInfo::setSize(int topleftx, int toplefty, int barwidth, int height, int avatarWidth) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	m_topleftx = topleftx;
	m_toplefty = toplefty;
	m_barWidth = barwidth; 
	m_height = height;
	m_avatarWidth = avatarWidth;

	return true;

}


bool CAvatarInfo::setParameterText(int parameterindex, const char text[]) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_totalnumberofParameter)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	strcpy(m_parameterText[parameterindex], text);

	return true;
}

bool CAvatarInfo::setParameterValue(int parameterindex, double value) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_totalnumberofParameter)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	m_parameterValue[parameterindex] = value;

	return true;
}


bool CAvatarInfo::setParameterColor(int parameterindex, int color) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_totalnumberofParameter)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	m_parameterColour[parameterindex] = color;

	return true;

}

bool CAvatarInfo::drawAvatarinfo(BITMAP *buffer) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	stretch_blit(m_avatarImage, buffer, 0,0, m_avatarImage->w, m_avatarImage->h, m_topleftx, m_toplefty, m_avatarWidth, m_height);

	if (m_totalnumberofParameter==0)
		return false;

	int text_x = m_topleftx + m_avatarWidth + 3;
	int text_y = m_toplefty;
	int bar_x1 = 129;
	int bar_y1 = 673;
	int bar_x2 = bar_x1 + m_parameterValue[0]/71*m_barWidth;
	int bar_y2 = bar_y1 + 11;

	int bar_colour;

	if (m_parameterValue[0]>50)
		bar_colour = makecol(128,0,0);
	else if (m_parameterValue[0]<50)
		bar_colour = makecol(255,0,0);
	else
		bar_colour = makecol(255,0,0);

	rectfill(buffer, bar_x1, bar_y1, bar_x2, bar_y2, bar_colour);

	for (int i=1;i<m_totalnumberofParameter;i++) {
		text_y = bar_y2 + 3;
		bar_y1 = 724;
		bar_x2 = bar_x1 + m_parameterValue[1]/71*m_barWidth;
		bar_y2 = bar_y1 + 11;

		if (m_parameterValue[1]>50)
			bar_colour = makecol(0,128,0);
		else if (m_parameterValue[1]<50)
			bar_colour = makecol(0,255,0);

	rectfill(buffer, bar_x1, bar_y1, bar_x2, bar_y2, bar_colour);

	}

	return true;
}

bool CAvatarInfo::getParameterValue(int parameterindex, double* value) {

	if (!m_binitialised) {

		allegro_message("The Avatar Info has not already been initialised");
		return false;

	}

	if ((parameterindex<0)||(parameterindex>=m_totalnumberofParameter)) {

		allegro_message("The supplied parameter number exceeds range");
		return false;

	}

	*value = m_parameterValue[parameterindex];

	return true;


}

bool CAvatarInfo::initialise(const char filename[], int topleftx, int toplefty, int avatarWidth, int avatarHeight, int barWidth) {

	if (m_binitialised) {
		allegro_message("Avatar Info object can not be initialised twice");
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
	if (strcmp(buf,"Avatar Data File")) {
		allegro_message("The supplied file is not an avatar info data file");
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
		allegro_message("The file contains no image filename");
		infile.close();
		return false;
	}

	m_avatarImage = load_bitmap(buf,0);

	if (m_avatarImage==NULL) {

		allegro_message(filename);
		infile.close();
		return false;

	}

	keep_reading = true;
	while (keep_reading) {
		infile.getline(buf,1024,'\n');
		if ((buf[0]!='#')&&(buf[0]!=NULL))
			keep_reading = false;
	}

	if (infile.eof()) {
		allegro_message("The file contains no total number of parameters");
		destroy_bitmap(m_avatarImage);
		m_avatarImage = NULL;
		infile.close();
		return false;
	}

	m_totalnumberofParameter = atoi(buf);

	if (m_totalnumberofParameter<=0) {
		allegro_message("The file contains invalid number of total number of parameters");
		destroy_bitmap(m_avatarImage);
		m_avatarImage = NULL;
		infile.close();
		return false;
	}

	m_parameterText = new char*[m_totalnumberofParameter];
	m_parameterValue = new double[m_totalnumberofParameter];
	m_parameterColour = new int[m_totalnumberofParameter];

	for (int i=0;i<m_totalnumberofParameter;i++) {
		keep_reading = true;
		while (keep_reading) {
			infile.getline(buf,1024,'\n');
			if ((buf[0]!='#')&&(buf[0]!=NULL))
				keep_reading = false;
		}

		if (infile.eof()) {
			allegro_message("Unexpected end of files encountered");
			delete m_parameterValue;
			delete m_parameterColour;

			for (int j=0;j<i;j++) {
				delete m_parameterText[i];
				m_parameterText[i]=NULL;
			}

			delete m_parameterText;
			m_parameterText = NULL;

			infile.close();
			return false;
		}

		m_parameterText[i] = new char[256];
		strcpy(m_parameterText[i],buf);

		infile.getline(buf,1024,' ');
		m_parameterValue[i] = atoi(buf);

		infile.getline(buf,1024,' ');
		int red = atoi(buf);

		infile.getline(buf,1024,' ');
		int green = atoi(buf);

		infile.getline(buf,1024,'\n');
		int blue = atoi(buf);

		m_parameterColour[i] = makecol(red,green,blue);

	}

	infile.close();

	m_binitialised = true;

	m_topleftx = topleftx;
	m_toplefty = toplefty;
	m_barWidth = barWidth; 
	m_height = avatarHeight;
	m_avatarWidth = avatarWidth;

	return true;

}

