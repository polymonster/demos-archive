#include "DataManagement.h"
#include <string>
#include <fstream>
#include "Definitions.h"
#include <iostream>

using namespace std;

         
int CData::FormatSaveFiles(const char slot[])
{
	char buf[1024];
	
	ifstream dataset;

	dataset.open(slot);

	dataset.getline(buf,1024,'\n');

	ofstream playerfile;

	playerfile.open(buf,ios::out);

	playerfile << "0\n0\n100\n100\n1\n12\n0";
	
	playerfile.close();

	dataset.getline(buf,1024,'\n');

	ofstream statsfile;

	statsfile.open(buf,ios::out);

	statsfile << "1" << "\n" << "0" << endl;
	statsfile << "1" << "\n" << "0" << endl;
	statsfile << "1" << "\n" << "0" << endl;
	statsfile << "1" << "\n" << "0" << endl;
	statsfile << "1" << "\n" << "0" << endl;
	
	statsfile.close();

	dataset.getline(buf,1024,'\n');

	ofstream inventoryfile;

	inventoryfile.open(buf,ios::out);

	for(int i=0; i < 20; i++)
	{
		inventoryfile << "0\n";
	}

	for(i=0; i < 4; i++)
	{
		inventoryfile << "0\n";
	}

	inventoryfile << "1\n";

	for(i=0; i < 4; i++)
	{
		inventoryfile << "0\n";
	}

	inventoryfile.close();

	return 0;

}


void CData::GetMiniStats(int slot,int *health, int *energy,int *days, int *hours, int* mins)
{
	char buf[1024];

	ifstream playerfile;
	
	if(slot == 0)
	{
		playerfile.open("Savefiles/Player0.dat");

	}
	else if(slot == 1)
	{
		playerfile.open("Savefiles/Player1.dat");

	}
	else if(slot == 2)
	{
		playerfile.open("Savefiles/Player2.dat");

	}
	else if(slot == 3)
	{
		playerfile.open("Savefiles/Player3.dat");

	}
	else if(slot == 4)
	{
		playerfile.open("Savefiles/Player4.dat");

	}

		playerfile.getline(buf,1024,'\n');

		playerfile.getline(buf,1024,'\n');

		playerfile.getline(buf,1024,'\n');

		*health = atoi(buf);

		playerfile.getline(buf,1024,'\n');

		*energy = atoi(buf);

		playerfile.getline(buf,1024,'\n');

		*days	= atoi(buf);

		playerfile.getline(buf,1024,'\n');

		*hours = atoi(buf);

		playerfile.getline(buf,1024,'\n');

		*mins = atoi(buf);

		playerfile.close();


}


int CData::LoadGame(BITMAP *buffer, BITMAP *cursor, int gamemode, int *slot, CBackground *mainbg)
{
	char buf[1024];
	
	int slotused[5];

	int textx;

	int selectedslot;

	char* slotname[5];

	slotname[0] = "Savefiles/slot0.dat";
	slotname[1] = "Savefiles/slot1.dat";
	slotname[2] = "Savefiles/slot2.dat";
	slotname[3] = "Savefiles/slot3.dat";
	slotname[4] = "Savefiles/slot4.dat";

	bool loadgame;
	bool clicked = false;
	
	int colour_white = makecol(225,222,194);
	int colour_dkred = makecol(128,0,0);
	int colour_dkgreen = makecol(0,128,0);
	int colour_blue = makecol(50,25,128);

	BITMAP *noOption = NULL;
	BITMAP *newGame = NULL;
	BITMAP *loadGame = NULL;
	BITMAP *backMain = NULL;
	BITMAP *eraseGame = NULL;

	noOption = load_bitmap("Main Menu/LoadMenu/LoadMenu-None.bmp", 0);
	newGame = load_bitmap("Main Menu/LoadMenu/LoadMenu-New.bmp", 0);
	loadGame = load_bitmap("Main Menu/LoadMenu/LoadMenu-Load.bmp", 0);
	backMain = load_bitmap("Main Menu/LoadMenu/LoadMenu-Back.bmp", 0);
	eraseGame = load_bitmap("Main Menu/LoadMenu/LoadMenu-Erase.bmp", 0);
	
	ifstream mainfile;

	mainfile.open("Savefiles/maindatafile.dat");

	for(int i = 0; i < 5;i++)
	{
		mainfile.getline(buf,1024,'\n');
		slotused[i] = atoi(buf);

		if(slotused[i] == 1);
		{
			gamemode = 1;
		}
	}

	mainfile.close();


	while(loadgame)
	{
		clear_bitmap(buffer);

		mainbg->drawBackground(buffer,512,334,false);

		masked_stretch_blit(noOption,buffer,0,0,781,264,126,443,781,264);

		//textprintf(buffer, font, 0, 0, colour_white, "mouse x%i", mouse_x);
		//textprintf(buffer, font, 0, 10, colour_white, "mouse y%i", mouse_y);

		if((mouse_x > 260) && (mouse_x < 390) && (mouse_y > 625) && (mouse_y < 670))
		{
			masked_stretch_blit(newGame,buffer,0,0,781,264,126,443,781,264);
			
			if(mouse_b & 1)
			{
				gamemode = 0;
			}
		}
		else if((mouse_x > 420) && (mouse_x < 570) && (mouse_y > 625) && (mouse_y < 670))
		{
			masked_stretch_blit(loadGame,buffer,0,0,781,264,126,443,781,264);
			
			if(mouse_b & 1)
			{
				gamemode = 1;
			}
		}
		else if((mouse_x > 600) && (mouse_x < 770) && (mouse_y > 625) && (mouse_y < 670))
		{
			masked_stretch_blit(eraseGame,buffer,0,0,781,264,126,443,781,264);
			
			if(mouse_b & 1)
			{
				gamemode = 2;
			}
		}
		else if((mouse_x > 820) && (mouse_x < 905) && (mouse_y > 679) && (mouse_y < 702))
		{
			masked_stretch_blit(backMain,buffer,0,0,781,264,126,443,781,264);
			
			if(mouse_b & 1)
			{
				*slot = -1;
				return 0;
			}
		}

		/*if(key[KEY_2])
		{
			gamemode = 1;
		}

		if(key[KEY_3])
		{
			gamemode = 2;
		}*/

				
		//textprintf_centre_ex(buffer, font,512, 10, colour_white,-1, "Choose a game slot");

		textx = 215;

		int xmax = textx + 65;

		for(int i = 0; i < 5; i ++)
		{
			

			if((mouse_x > textx - 55) && (mouse_x < xmax) && (mouse_y > 475) && (mouse_y < 575))
			{
				
				rectfill(buffer,textx-61,475,textx + 59,576,colour_blue);

				if(mouse_b & 1)
				{	
					clicked = true;
					selectedslot = i;
				}

			}


			textprintf_centre_ex(buffer, font,textx,480, colour_white,-1, "Slot %i", i+1);
			//textprintf_ex(buffer, font,0,100, colour_white,-1, "x%i", mouse_x);
			//textprintf_ex(buffer, font,0,110, colour_white,-1, "y%i", mouse_y);

				if(slotused[i] == 1)
				{
					int health,energy,days,hours,mins;
					GetMiniStats(i,&health,&energy,&days,&hours,&mins);

					textprintf_centre_ex(buffer, font,textx,500, colour_white,-1, "Day % i", days);
					textprintf_centre_ex(buffer, font,textx,510, colour_white,-1, "Time %i:%02i", hours, mins);

					int alignbarsx = textx - 45;
						
					rectfill(buffer,alignbarsx,535,alignbarsx + (health/1.3),540,colour_dkred);
					rectfill(buffer,alignbarsx,545,alignbarsx + (energy/1.3),550,colour_dkgreen);

					textprintf_centre_ex(buffer, font,textx,560, colour_white,-1, "Slot In Use");
				}
				else
				{
					textprintf_centre_ex(buffer, font,textx,560, colour_white,-1, "Empty Slot");
				}

					textx = textx + 150;
					xmax = textx + 65;
				}
					
					
				if(clicked)
				{
					if((gamemode == 0) && (slotused[selectedslot] == 1))
					{
						textprintf_centre_ex(buffer, font,512,200, colour_white,-1, "This slot is in use! overwrite a new file?");

						if(key[KEY_Y])
						{
							loadgame = false;
							clicked = false;
						}
						else if(key[KEY_N])
						{
							selectedslot = -1;
							clicked = false;
						}
					}
					else if((gamemode == 1) && (slotused[selectedslot] == 0))
					{
						textprintf_centre_ex(buffer, font,512,200, colour_white,-1, "This file is empty!");

						if(key[KEY_ENTER])
						{
							selectedslot = -1;
							clicked = false;
						}
					}
					else if((gamemode == 2) && (slotused[selectedslot] == 1))
					{
						textprintf_centre_ex(buffer, font,512,200, colour_white,-1, "Really Erase");

						if(key[KEY_Y])
						{
							FormatSaveFiles(slotname[selectedslot]);
							slotused[selectedslot] = 0;
							selectedslot = -1;
							clicked = false;
						}
						else if(key[KEY_N])
						{
							selectedslot = -1;
							clicked = false;
						}
					}
					else if((gamemode == 2) && (slotused[selectedslot] == 0))
					{
						textprintf_centre_ex(buffer, font,512,200, colour_white,-1, "already empty");
							selectedslot = -1;
						
						if(key[KEY_ENTER])
						{
							selectedslot = -1;
							clicked = false;
						}

					}
					else
					{
						loadgame = false;
					}

				}

					scare_mouse();
					blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
					unscare_mouse();

				if(key[KEY_ESC])
				{
					*slot = -1;
					return 0;
				}

			}

	
			clear_bitmap(buffer);


			
			scare_mouse();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
			unscare_mouse();

			loadgame = true;
			
			if((gamemode == 0) || (slotused[selectedslot] == 0)) //MAKE NEW
			{
				FormatSaveFiles(slotname[selectedslot]);
			}
		
			ofstream savetomain;

			slotused[selectedslot] = 1;

			savetomain.open("Savefiles/maindatafile.dat", ios::out);
				
				for(i = 0; i < 5; i ++)
				{
					savetomain << slotused[i] << "\n";
				}
			
			*slot = selectedslot;
				
			mainfile.close();

			destroy_bitmap(noOption);
			destroy_bitmap(newGame);
			destroy_bitmap(loadGame);
			destroy_bitmap(backMain);
			destroy_bitmap(eraseGame);

			return 0;

}