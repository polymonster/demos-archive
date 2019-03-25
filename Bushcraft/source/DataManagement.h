#include <allegro.h>
#include "Background.h"
#ifndef INC_DATA_H
#define INC_DATA_H

class CData
{


protected:
	



public:

	//CData();
	//~CData();

	int LoadGame(BITMAP *buffer, BITMAP *cursor, int gamemode, int *slot, CBackground *mainbg);
	void GetMiniStats(int slot,int *health, int *energy,int *days, int *hours, int* mins);
	int FormatSaveFiles(const char slot[]);





};

#endif