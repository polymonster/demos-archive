#ifndef INC_PLAYERSPRITE_H
#define INC_PLAYERSPRITE_H

#include "Sprite.h"
#include "AvatarInfo.h"

class CPlayerSprite : public CSprite 
{
protected:
	int m_parameternumber;
	double* m_parameter;
	CAvatarInfo* m_avatarInfo;
public:
	CPlayerSprite();
	~CPlayerSprite();
	bool initialise(const char filename[]);
	void drawSprite(BITMAP* buffer,CBackground *bkground);
	void drawAvatar(BITMAP *buffer);
	bool setParameterValue(int paramindex, double value);
	bool getParameterValue(int paramindex, double *value);
	bool isInRange(int xpos,int ypos);

};

#endif /* INC_PLAYERSPRITE_H */

