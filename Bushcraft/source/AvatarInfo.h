#ifndef INC_AVATARINFO_H
#define INC_AVATARINFO_H

#include "Allegro.h"

class CAvatarInfo {

protected:
	BITMAP* m_avatarImage;
	bool m_binitialised;
	int m_topleftx, m_toplefty;
	int m_barWidth, m_height, m_avatarWidth;
	int m_totalnumberofParameter;
	char** m_parameterText;
	double* m_parameterValue;
	int* m_parameterColour;

public:
	CAvatarInfo();
	~CAvatarInfo();
	bool isInitialised() { return m_binitialised; };
	bool initialise(const char filename[], int topleftx, int toplefty, int avatarWidth, int avatarHeight, int barWidth, int parameternumber, char** parameterText, double* parameterValue, int* parameterColour); 
	bool initialise(const char filename[], int topleftx, int toplefty, int avatarWidth, int avatarHeight, int barWidth); 
	bool setSize(int topleftx, int toplefty, int width, int height, int avatarWidth);
	bool setParameterText(int parameterindex, const char text[]);
	bool setParameterValue(int parameterindex, double value);
	bool getParameterValue(int parameterindex, double* value);
	bool setParameterColor(int parameterindex, int color);
	bool drawAvatarinfo(BITMAP* buffer);
	int getTotalNumberofParameter() { return m_totalnumberofParameter; };
};

#endif /* INC_AVATARINFO_H */
