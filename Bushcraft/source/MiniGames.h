#include "allegro.h"
#ifndef INC_MINIGAMES_H
#define INC_MINIGAMES_H


class CMiniGames
{

protected:

public:

	int fireminigame(int fireSkillLevel, int *exp, int *lifespan);
	int fishingminigame(int fishingSkillLevel, int *exp, int *yourcatch);

};

#endif