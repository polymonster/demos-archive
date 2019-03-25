#include <allegro.h>
#include <time.h>
#include "Background.h"
#include "PlayerSprite.h"
#include "Sprite.h"
#include "Inventory.h"
#include "Stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//PLAYER STATES

#define WALK_EAST		0
#define WALK_NORTH		1
#define WALK_WEST		2
#define WALK_SOUTH		3

//INVENTORY ITEMS

#define EMPTY				0
#define STICK				1
#define STONE				2
#define SHARP_STICK			3
#define KINDLING			4
#define HAND_DRILL			5
#define MEAT				6
#define BONE				7
#define SINEW				8
#define COOKED_MEAT			9
#define SEWING_KIT			10
#define HARDENED_SPEAR		11
#define RABBIT				12
#define RABBIT_MEAT			13
#define RABBIT_HIDE			14
#define CURED_RABBIT_HIDE	15
#define LEATHER_FLASK		16

// ENEMY STATES

// TIMERS -------------------------------------------------------------------------------------
volatile int update_frame = 0;
volatile int persecondflag = 0;

int sec = 0;
int minutes = 0;
int hours = 12;
int days = 1;
int slower = 0;

int itimer = 0;
int sparktimer = 0;

void timer_function() {
	update_frame=1;
} END_OF_FUNCTION(timer_function);

void fps_counter_function() {
	persecondflag=1;
} END_OF_FUNCTION(fps_counter_function);

//INITIALSE ALLEGRO-----------------------------------------------------------------------------
bool initialise_allegro() {

	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();
	
	if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)) 
	{
		allegro_message("Sound Error: %s", allegro_error);
		return false;
	} 
	else
		return true;
}



//-----------------------------------------------------------------------------------------------
int fireminigame(void)
{
	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	BITMAP *unlitfire = NULL;
	BITMAP *litfire1 = NULL;
	BITMAP *litfire2 = NULL;
	BITMAP *litfire3 = NULL;

	unlitfire =load_bitmap("data/fireanimguiUNLIT.bmp",0);
	litfire1 =load_bitmap("data/fireanimguiLITSMALL.bmp",0);
	litfire2 =load_bitmap("data/fireanimguiLITLARGE.bmp",0);
	litfire3 =load_bitmap("data/fireanimguiLITGIANT.bmp",0);

	int colour_white = makecol(255,255,255);

	int firequality = 0;

	int window_width = 1024;
	int window_height = 768;

	int frameControl = 0;


	CBackground* fmg = new CBackground();
	//if (fmg->initialise("FireMinigame.bmp",NULL,background_windowwidth,background_windowheight,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
	if(fmg->initialise("data/FireMinigame.bmp",NULL,window_width,window_height,0,0,window_width,window_height)==0){
		return -1;
	}
	
	CSprite** fireMan = new CSprite*[1];

	fireMan[0] = new CSprite();
	if (!fireMan[0]->initialise("data/fire man sprite data.dat")) {
		return 1;
	}

	fireMan[0]->spawn(0,999999,230,310,0,0);

	CSprite** droppingitem1 = new CSprite*[12];
	for (int i=0;i<12;i++) 
	{
		droppingitem1[i] = new CSprite();
		if (!droppingitem1[i]->initialise("data/dropping item sprite data.dat")) 
		{
			return 1;
		}

	}


	int areax = 605;
	
	CSprite** area = new CSprite*[4];
	for (i=0;i<4;i++) 
	{
		area[i] = new CSprite();
		if (!area[i]->initialise("data/area item sprite data.dat")) 
		{
			return 1;
		}

		area[i]->spawn(0,999999,areax,310,0,0);
		areax = areax + 92;
	}


	int spawntimer = 0;
	int column = 0;
	int activenode = 0;
	int speed = 2;
	int round = 0;
	int nodes = 1;
	int spawnrate = 130;
	int acc = 0;
	unsigned int score = 0;
	int miss = 0;
	int addpoints = 0;

	do
	{

		
		if(frameControl == 0)
		{
		fireMan[0]->incrementAnimationframe();
		frameControl = 3;
		}
		
		
		if(spawntimer == 0)
		{
			int itemx, itemy;

			spawntimer = spawnrate;

			itemy = 0;


			time_t seconds;
			time(&seconds);

			srand((unsigned int)seconds);

			if(round < 1)
			{
			column = rand()%2;
			}
			else
			{
			column = rand()%4;
			}
			
			int number;

			if((round == 3) && (activenode < 9))
			{
				number = 2;
			}
			else
			{
				number = 1;
			}

			int state;

			if(column == 0)
			{
				state = 0;

				itemx = 605;
				
				for(i=0;i< number;i++)
				{
					droppingitem1[activenode]->spawn(state,999999,itemx,itemy,0,speed);
					activenode++;
					itemy = itemy - 75;
				}
			}
			else if (column == 1)
			{
				state = 1;

				itemx = 881;

				for(i=0;i< number;i++)
				{
					droppingitem1[activenode]->spawn(state,999999,itemx,itemy,0,speed);
					activenode++;
					itemy = itemy - 75;
				}
			}
			
			
			if (column == 2)
			{
				state = 2;

				itemx = 697;

				for(i=0;i< number;i++)
				{
					droppingitem1[activenode]->spawn(state,999999,itemx,itemy,0,speed);
					activenode++;
					itemy = itemy - 75;
				}
			}
			else if (column == 3)
			{
				state = 3;

				itemx = 789;

				for(i=0;i< number;i++)
				{
					droppingitem1[activenode]->spawn(state,999999,itemx,itemy,0,speed);
					activenode++;
					itemy = itemy - 75;
				}
			}




		}

		if(activenode == 12)
		{
			activenode = 0;
		}

		for(i=0;i<activenode+1;i++)
		{
			int xpos, ypos;
			int state;
			
			droppingitem1[i]->getpos(&xpos,&ypos);
			state = droppingitem1[i]->getSpriteState();


			if((xpos == 605) && (ypos < 340) && (ypos > 285))  //COLUMN L
			{
				if(((key[KEY_A])||(key[KEY_LEFT])) && (state < 4))
				{
					addpoints = ypos - 310;
					if(addpoints < 0)
					{
						addpoints = addpoints * -1;
						
					}
					
					
					if(addpoints < 2)
					{
						addpoints = 5000;
						droppingitem1[i]->setSpriteState(4);
					}
					else
					{
						addpoints = 1000/addpoints;
						droppingitem1[i]->setSpriteState(5);
					}

					score = score + addpoints;
					
					
					acc++;

					
				}

			}
			else if((xpos == 881) & (ypos < 340) && (ypos > 285)) //COLUMN R
			{
				if(((key[KEY_D])||(key[KEY_RIGHT])) && (state < 4))
				{
					addpoints = ypos - 310;
					if(addpoints < 0)
					{
						addpoints = addpoints * -1;
						
					}
					
					
					if(addpoints < 2)
					{
						addpoints = 5000;
						droppingitem1[i]->setSpriteState(4);
					}
					else
					{
						addpoints = 1000/addpoints;
						droppingitem1[i]->setSpriteState(5);
					}

					score = score + addpoints;
					
					
					acc++;
					
				
				}

			}
			else if((xpos == 697) & (ypos < 340) && (ypos > 285)) //COLUMN R
			{
				if(((key[KEY_W])||(key[KEY_UP])) && (state < 4))
				{
					addpoints = ypos - 310;
					if(addpoints < 0)
					{
						addpoints = addpoints * -1;
						
					}
					
					
					if(addpoints < 2)
					{
						addpoints = 5000;
						droppingitem1[i]->setSpriteState(4);
					}
					else
					{
						addpoints = 1000/addpoints;
						droppingitem1[i]->setSpriteState(5);
					}

					score = score + addpoints;
					
					
					acc++;
					

				
				}

			}
			else if((xpos == 789) & (ypos < 340) && (ypos > 285)) //COLUMN R
			{
				if(((key[KEY_S])||(key[KEY_DOWN])) && (state < 4))
				{
					addpoints = ypos - 310;
					if(addpoints < 0)
					{
						addpoints = addpoints * -1;
						
					}
					
					
					if(addpoints < 2)
					{
						addpoints = 5000;
						droppingitem1[i]->setSpriteState(4);
					}
					else
					{
						addpoints = 1000/addpoints;
						droppingitem1[i]->setSpriteState(5);
					}

					score = score + addpoints;
					
					
					acc++;
					
				
				}

			}
			


		}

		
		for(i=0;i<12;i++)
		{
			int xpos, ypos;
			int state;
			
			droppingitem1[i]->getpos(&xpos,&ypos);
			state = droppingitem1[i]->getSpriteState();
			
			if((ypos > 370) && (state < 4) && (droppingitem1[i]->isAlive()))
			{
				droppingitem1[i]->setSpriteState(6);
				miss++;
			}
		}

		if(acc == round * 5)
		{
			round++;

			if(round == 2)
			{
				speed++;
				spawnrate = spawnrate - 75;
			}

			acc = 0;
			
		}


		

		
		set_clip(buffer,0,0,0,0);
		fmg->drawBackground(buffer,window_width/2,window_height/2,NULL);
		fireMan[0]->drawSprite(buffer,fmg);

		if(round == 1)
		{
			stretch_blit(unlitfire,buffer,0,0,113,143,388,630, 113, 120);
		}
		else if (round == 2)
		{
			stretch_blit(litfire1,buffer,0,0,113,143,388,630, 113, 120);
		}
		else if ((round == 3) || (round == 4))
		{
			stretch_blit(litfire2,buffer,0,0,113,143,388,630, 113, 120);
		}
		else if (round == 5)
		{
			stretch_blit(litfire3,buffer,0,0,113,143,388,630, 113, 120);
		}


		textprintf(buffer, font, 0,20, colour_white, "Mouse x: %i", mouse_x);
		textprintf(buffer, font, 0, 30, colour_white, "Mouse y: %i", mouse_y);
		textprintf(buffer, font, 0, 40, colour_white, "spawner: %i", spawntimer);
		textprintf(buffer, font, 0, 50, colour_white, "Node: %i", activenode);
		textprintf(buffer, font, 0, 60, colour_white, "column: %i", column);
		textprintf(buffer, font, 0, 70, colour_white, "ACCUM: %i", acc);
		textprintf(buffer, font, 0, 80, colour_white, "Round: %i", round);
		textprintf(buffer, font, 512, 300, colour_white, "%i", score);

		set_clip(buffer,543,55,945,580);

		for(i=0;i<4;i++) 
		{
			area[i]->drawSprite(buffer,fmg);
		}
		
		
		for(i = 0; i < 12; i++)
		{
			if(droppingitem1[i]->isAlive())
			{
			droppingitem1[i]->drawSprite(buffer,fmg);
			droppingitem1[i]->updatepos();
			}
		}

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

		
		if(frameControl > 0)
			frameControl--;

		
		if(spawntimer > 0)
		{
			spawntimer--;
		}


		

	}while((miss < 100) && (round < 6) && (!key[KEY_B]));

	delete fmg;

	delete fireMan[0];
	delete fireMan;

	for (i=0;i<12;i++) 
		delete droppingitem1[i];
	delete droppingitem1;

	for (i=0;i<4;i++) 
		delete area[i];
	delete area;

	destroy_bitmap(buffer);
	destroy_bitmap(unlitfire);
	destroy_bitmap(litfire1);
	destroy_bitmap(litfire2);
	destroy_bitmap(litfire3);

	if(round = 4)
	{
		firequality = 1;
	}
	else
	{
		firequality = 0;
	}

	return firequality;

}


// M A I N -------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------

int main(void) 
{

	// Call the Allegro initialisation function
	if (!initialise_allegro()) {
		allegro_message("Initialisation Failure\n % s\n", allegro_error );
		return 1;
	}

	set_window_title("Bush Craft");
	set_color_depth(32);
	int window_width = 1024;
	int window_height = 768;

	if ( set_gfx_mode( GFX_AUTODETECT_WINDOWED, window_width, window_height , 0 , 0 ) < 0 )	{
		allegro_message("Unable initialize graphics module\n % s\n", allegro_error );
		return  1;
	}

	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	if (buffer == NULL) {
		allegro_message("Unable to initialise the back buffer");
		return -1;
	}

	int visiblescreen_width = 1024;
	int visiblescreen_height = 600;
	int visiblescreen_topleft_x = (SCREEN_W-visiblescreen_width)/2;
	int visiblescreen_topleft_y = 20;
	int visiblescreen_bottomright_x = visiblescreen_topleft_x + visiblescreen_width;
	int visiblescreen_bottomright_y = visiblescreen_topleft_y + visiblescreen_height;
	int background_windowwidth = 768;
	int background_windowheight = 450;


	CBackground* bk = new CBackground();
	if (bk->initialise("data/Background.bmp",NULL,background_windowwidth,background_windowheight,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
		return -1;
	}

	// Find out what is the size in pixels the background is
	int backgroundwidth, backgroundheight;
	bk->getDimension(&backgroundwidth, &backgroundheight);

		CBackground* sd = new CBackground();
		if (sd->initialise("data/shade.bmp",NULL,1024,768,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
		return -1;
	}

		CBackground* water = new CBackground();
		if (water->initialise("data/Background.bmp","Background.pgm",background_windowwidth,background_windowheight,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
		return -1;
	}


	
	//ITEM SPRITES------------------------------------------------------------------------------------------------------
	
	//ALTER TO 1 INVENTORY SPRITE---------------------------------------------------------------------------------------

	int numberofitems = 2;
	int numberofFire = 10;
	int fireno = 0;
	int numberofgazell = 4;
	
	CSprite** item = new CSprite*[numberofitems];
	for (int i=0;i<numberofitems;i++) 
	{
		item[i] = new CSprite();
		if (!item[i]->initialise("data/items sprite data.dat")) 
		{
			return 1;
		}
	}


	CSprite** fire = new CSprite*[numberofFire];
	for (i=0;i<numberofFire;i++) 
	{
		fire[i] = new CSprite();
		if (!fire[i]->initialise("data/fire sprite data.dat")) 
		{
			return 1;
		}
	}

	item[0]->spawn(2,99999,400,200,0,0);

	CSprite** spear = new CSprite*[1];

	spear[0] = new CSprite();
	if (!spear[0]->initialise("data/spear sprite data.dat")) 
	{
		return 1;
	}

	CSprite** gazell = new CSprite*[numberofgazell];
	for (i=0;i<numberofgazell;i++) 
	{
		gazell[i] = new CSprite();
		if (!gazell[i]->initialise("data/gazell sprite data.dat")) 
		{
			return 1;
		}

		gazell[i]->setGazellHealth(15);
	}

	bool mouseovergazell;
	int activegazell;
	bool lootgazell = false;

	gazell[0]->spawn(2,99999,800,500,0,0);
	gazell[0]->setRoam(300);

	gazell[1]->spawn(1,99999,100,300,0,0);
	gazell[1]->setRoam(1);

	gazell[2]->spawn(0,99999,250,420,0,0);
	gazell[2]->setRoam(110);

	gazell[3]->spawn(4,99999,500,100,0,0);
	gazell[3]->setRoam(222);
	
	
	//TREES--------------------------------------------------------------------------------------------------------------
	
	int numberoftrees = 10;
	int activetree;
	bool treelooted = false;
	int clicktimer = 0;
	int numberofsaplings = 3;

	bool mouseovertree = false;
	bool mouseoversapling = false;
	int activesapling;
	bool lootTrap = false;

	int treelocationx[10];
	int treelocationy[10];

	treelocationx[0] = 100;
	treelocationy[0] = 125;

	treelocationx[1] = 300;
	treelocationy[1] = 240;

	treelocationx[2] = 30;
	treelocationy[2] = 320;

	treelocationx[3] = 260;
	treelocationy[3] = 420;

	treelocationx[4] = 580;
	treelocationy[4] = 95;

	treelocationx[5] = 835;
	treelocationy[5] = 620;

	treelocationx[6] = 690;
	treelocationy[6] = 665;

	treelocationx[7] = 620;
	treelocationy[7] = 325;

	treelocationx[8] = 640;
	treelocationy[8] = 545;

	treelocationx[9] = 70;
	treelocationy[9] = 630;



	CSprite** tree = new CSprite*[numberoftrees];
	for (i=0;i<numberoftrees;i++) 
	{
		tree[i] = new CSprite();
		if (!tree[i]->initialise("data/tree sprite data.dat")) 
		{
			return 1;
		}
	}

	for (i=0;i<numberoftrees;i++)
	{
		tree[i]->spawn(0,999999,treelocationx[i],treelocationy[i],0,0);
		time_t seconds;
		time(&seconds);

		srand((unsigned int)seconds);
		int rndloot = rand()%3 + 1;

		tree[i]->setLoot(3, false);
	}

	CSprite** sapling = new CSprite*[numberofsaplings];
	for (i=0;i<numberofsaplings;i++) 
	{
		sapling[i] = new CSprite();
		if (!sapling[i]->initialise("data/sapling sprite data.dat")) 
		{
			return 1;
		}
	}

	sapling[0]->spawn(0,999999,700,50,0,0);
	sapling[1]->spawn(0,999999,700,300,0,0);
	sapling[2]->spawn(0,999999,900,225,0,0);
	
	// ================================================================

	CPlayerSprite* playerSprite = new CPlayerSprite();
	if (!playerSprite->initialise("data/Player Data.dat")) {
		return 1;
	}

	int xpos = 0;
	int ypos = 0;
	playerSprite->setpos(xpos,ypos);

	text_mode(-1);

	double health = 100;
	double energy = 100;
	int level = 1;
	int speed = 1;
	int strength = 1;
	
	double levelExp = 0;
	int fireMaker = 1;
	double fireExp = 0;

	int hunting = 1;
	double huntingExp = 0;

	int cooking = 1;
	double cookingExp = 0;

	int nrgDrain = 0;
	
	
	int fireRate = 0;


	//PLAYERS INVENTORY ITEMS---------------------------------------------------------------------------

			int nextEmpty;
			int returnoption = 0;
			int maxinvent;
			bool collideWithTree = false;
			bool showtreeloot = false;
			bool lootspawned = false;
			int lootItem[10];
			int selectedLoot = 0;
			bool manipulate = false;
			bool click = false;
			int wieldWeapon = 0;
			int woodcount = 0;
			bool statsopen = false;
			bool mouseoninterface = false;
			int Spears = 0;
			int equippedItem = 0;
			


			maxinvent = 20;

			int inventoryitem[20];
			int selecteditem = 0;
			int equippeditem = 0;

			int mtimer = 0; //mouse timer for inventory clicks

			for(i=0; i < maxinvent; i++)
			{
				inventoryitem[i] = 0;
			}

			for(i=0; i < 10; i++)
			{
				lootItem[i] = 0;
			}


	// DECLARE A VAST AMOUNT OF BITMAPS FOR THE INVENTORY "TO SCREEN" IMAGES
	
	// Bools for inventory options
			
	bool inventoryopen = false;
	bool hasStone = false;
	bool hasStick = false;
	bool hasSharpStick = false;
	bool hasKindling = false;
	bool hasFire = false;
	bool mouseOverWater = false;
	bool inWaterMenu = false;

	int maxloot = 10;	
	
	BITMAP * emptyinventory = NULL;
	BITMAP * inventoryStick = NULL;
	BITMAP * inventoryStone = NULL;
	BITMAP * guibg = NULL;
	BITMAP * treeLabel = NULL;
	BITMAP * lootTable = NULL;
	BITMAP * manipulateTable = NULL;
	BITMAP * inventorySharpStick = NULL;
	BITMAP * inventoryKindling = NULL;
	BITMAP * inventoryHandDrill = NULL;
	BITMAP * inventoryBg = NULL;
	BITMAP * bag1 = NULL;
	BITMAP * bag2 = NULL;
	BITMAP * skill1 = NULL;
	BITMAP * skill2 = NULL;
	BITMAP * statsWindow = NULL;
	BITMAP * popup = NULL;
	BITMAP * topgui = NULL;
	BITMAP * meat=NULL;
	BITMAP * bone=NULL;
	BITMAP * sinew=NULL;
	BITMAP * spark=NULL;
	BITMAP * cookedmeat=NULL;
	BITMAP *cookingbg;
	BITMAP *rabbit = NULL;

	cookingbg = NULL;
	cookingbg = load_bitmap("data/CookingBarBG.bmp",0);

	emptyinventory =load_bitmap("data/emptyinvent.bmp",0);
	inventoryStick =load_bitmap("data/WoodBig.bmp",0);
	inventoryStone =load_bitmap("data/SharpStoneBig.bmp",0);
	guibg =load_bitmap("data/guibg.bmp",0);
	treeLabel =load_bitmap("data/TreeTag.bmp",0);
	lootTable =load_bitmap("data/LootWindow.bmp",0);
	manipulateTable = load_bitmap("data/Manipulate.bmp",0);
	inventorySharpStick = load_bitmap("data/SharpStick.bmp",0);
	inventoryKindling = load_bitmap("data/Kindling.bmp",0);
	inventoryHandDrill = load_bitmap("data/HandDrill.bmp",0);
	inventoryBg = load_bitmap("data/Bag.bmp",0);
	bag1 = load_bitmap("data/BackpackButton1.bmp",0);
	bag2 = load_bitmap("data/BackpackButton2.bmp",0);
	skill1 = load_bitmap("data/SkillButton1.bmp",0);
	skill2 = load_bitmap("data/SkillButton2.bmp",0);
	statsWindow = load_bitmap("data/StatsWindow.bmp",0);
	popup = load_bitmap("data/popup.bmp",0);
	topgui = load_bitmap("data/TopBarGui.bmp",0);	
	meat = load_bitmap("data/Meat.bmp",0);
	bone = load_bitmap("data/Bone.bmp",0);
	sinew = load_bitmap("data/Sinew.bmp",0);
	spark = load_bitmap("data/Spark.bmp",0);
	cookedmeat = load_bitmap("data/MeatCooked.bmp",0);
	rabbit = load_bitmap("data/Rabbit.bmp",0);

	CInventory* inventory = new CInventory();

	CStats* playerStats = new CStats();

	playerStats->initialise();

	inventory->Initialise();
	inventory->Load();

	// -----------------------------------------------------------------------------------------------------------------------------
	// MAKE COLOURS-----------------------------------------------------------------------------------------------------------------
	int colour_white = makecol(255,255,255);
	int colour_yellow = makecol(255,255,0);
	int colour_dkred = makecol(128,0,0);
	int colour_dkgreen = makecol(0,128,0);

	
	//------------------------------------------------------------------------------------------------------------------------------
	// TIMERS & FRAME RATE----------------------------------------------------------------------------------------------------------
	
	int frames_per_second = 0;
	int framecounter = 0;

	LOCK_VARIABLE(update_frame);
	LOCK_FUNCTION(timer_function);
	install_int_ex(timer_function, MSEC_TO_TIMER(75));

	LOCK_VARIABLE(persecondflag);
	LOCK_FUNCTION(fps_counter_function);
	install_int_ex(fps_counter_function, MSEC_TO_TIMER(1000));
	
	
	//LOAD DATD FROM DAT FILE-------------------------------------------------------------------------------------------------------
	//move to its own class
	ifstream plyrFile;
	
	char buf[1024];

	plyrFile.open("data/dat.dat");

	if (!plyrFile.is_open()) {
		allegro_message("Unable to open Player file");
		return false;
	}

	plyrFile.getline(buf,1024,'\n');

	health = atoi(buf);

	plyrFile.getline(buf,1024,'\n');

	energy = atoi(buf);

	plyrFile.close();

	show_mouse(screen);

	int mouseWorldx, mouseWorldy;


	//GAME LOOP----------------------------------------------------------------------------------------------------------------------
	bool keep_playing = true;

	while (keep_playing) 
	{
		if (update_frame > 0) 
		{

				time_t seconds;
				time(&seconds);

				srand((unsigned int)seconds);

				

			//DRAW SPRITES/WORLD/POSITIONING-----------------------------------------------------------------------------------------
		
			clear_bitmap(buffer);	
			
			
			playerSprite->getpos(&xpos,&ypos);

			bk->drawBackground(buffer,xpos,ypos,false);
		
			set_clip(buffer,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_bottomright_x,visiblescreen_bottomright_y);

			for(i=0;i<numberofFire;i++)
			{
				if(fire[i]->isAlive())
					fire[i]->drawSprite(buffer,bk);
					fire[i]->incrementAnimationframe();
							
			}

			mouseovergazell = false;
			mouseovertree = false;
			mouseoversapling = false;

			for(i=0;i<numberofsaplings;i++)
			{
				int sapx, sapy;
				int sapsrcx, sapsrcy;

				sapling[i]->getpos(&sapx, &sapy);

				bk->getScreencoordinate(sapx,sapy,&sapsrcx,&sapsrcy);

				int state = sapling[i]->getSpriteState();
				
				if(sapling[i]->isAlive())
					sapling[i]->drawSprite(buffer,bk);

				if((mouse_x > sapsrcx - 20) && (mouse_x < sapsrcx + 20) && (mouse_y > sapsrcy - 20)  && (mouse_y < sapsrcy + 20) && (!lootgazell) && (!mouseoninterface) && (!inventoryopen))
				{
					mouseoversapling = true;
					activesapling = i;
				}

				if(state == 1)
				{
					sapling[i]->CheckTrapDeactive(hours, minutes, buffer, &state);
				}

				sapling[i]->setSpriteState(state);



			}


			for(i=0;i<numberofgazell;i++)
			{
				int gx, gy;
				int gsx, gsy;
				int gs;
				
				int ghealth = gazell[i]->getGazellHealth();

				gs = gazell[i]->getSpriteState();
				
				if(gazell[i]->isAlive())
					gazell[i]->drawSprite(buffer,bk);

				gazell[i]->updatepos();

				gazell[i]->getpos(&gx,&gy);

				bk->getScreencoordinate(gx,gy,&gsx,&gsy);

				if(ghealth == 0)
				{
					gazell[i]->setSpriteState(4);
					gazell[i]->setSpeed(0,0);
					
				}
				
				if(gs != 4)
				{
					gazell[i]->GazellRoam();
				}		
					
				gazell[i]->GazellOutsideBound(backgroundwidth, backgroundheight);


				for(int j = 0; j < numberofgazell; j++)
				{
					if(i != j)
					{
						if(gazell[i]->checkCollisionwithSprite(gazell[j], gx,gy))
						{
							gazell[i]->switchstates();
						}
					}
				}

				
				
				if(gazell[i]->checkCollisionwithSprite(spear[0], gx,gy) && (spear[0]->isAlive()))
				{
					spear[0]->despawn();
					huntingExp = huntingExp + 33;
					ghealth = ghealth - 5;
					gazell[i]->setGazellHealth(ghealth);
					
	
				}

				if(gazell[i]->checkCollisionwithBackground(water, gx, gy))
				{
					gazell[i]->switchstates();
				}

				if((mouse_x > gsx - 20) && (mouse_x < gsx + 20) && (mouse_y > gsy - 20)  && (mouse_y < gsy + 20) && (!lootgazell) && (!mouseoninterface) && (!inventoryopen))
				{
					mouseovergazell = true;
					activegazell = i;
				}

			}

			

			playerSprite->drawSprite(buffer,bk);

			
			for(i=0;i<numberoftrees;i++)
			{
				if(tree[i]->isAlive())
				tree[i]->drawSprite(buffer,bk);
			}

			for(i=0;i<numberofitems;i++)
			{
				if(item[i]->isAlive())
					item[i]->drawSprite(buffer,bk);
			}


			if(spear[0]->isAlive())
				spear[0]->drawSprite(buffer,bk);
			spear[0]->updatepos();
			//-----------------------------------------------------------------------------------------------------------------------

			//PLAYER CONTROL---------------------------------------------------------------------------------------------------------
			int newxpos, newypos;
			
			if (key[KEY_D]) {							

				newxpos = xpos + 5;								
				if (newxpos>backgroundwidth)					
					newxpos = backgroundwidth;					
				newypos = ypos;								
																 
				playerSprite->setSpriteState(WALK_EAST);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_A]) {

				newxpos = xpos - 5;
				if (newxpos<0)
					newxpos = 0;
				newypos = ypos;

				playerSprite->setSpriteState(WALK_WEST);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_W]) {
				newxpos = xpos;
				newypos = ypos - 5;
				if (newypos < 0)
					newypos = 0;

				playerSprite->setSpriteState(WALK_NORTH);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_S]) {
				newxpos = xpos;
				newypos = ypos + 5;
				if (newypos > backgroundheight)
					newypos = backgroundheight;

				playerSprite->setSpriteState(WALK_SOUTH);
				playerSprite->incrementAnimationframe();


			} else {				

				newxpos = xpos;
				newypos = ypos;

			}

			if(key[KEY_C])
			{
			fireminigame();
			}

			
			if(key[KEY_ALT])
			{

					int px, py;
					int psrcx, psrcy;

					playerSprite->getpos(&px,&py);
					bk->getScreencoordinate(px,py,&psrcx,&psrcy);
					if(mouse_x > psrcx + 30)
					{
						playerSprite->setSpriteState(WALK_EAST);
					}
					else if(mouse_x < psrcx - 30)
					{
						playerSprite->setSpriteState(WALK_WEST);
					}

					if(mouse_y > psrcy + 60)
					{
						playerSprite->setSpriteState(WALK_SOUTH);
					}
					else if(mouse_y < psrcy - 60)
					{
						playerSprite->setSpriteState(WALK_NORTH);
					}
			}
					
			
			if(wieldWeapon > 0)
			{

				inventory->getAmmo(wieldWeapon, &Spears);

				if((key[KEY_ALT]) && (mouse_b & 1) && (fireRate == 0))
				{


					int state;
					int sx, sy;
					fireRate = 5;

					state = playerSprite->getSpriteState();
					
					if(state == 0)
					{
						sx = 10;
						sy = 0;
					}
					else if(state == 1)
					{
						sx = 0;
						sy = -10;
					}
					else if(state == 2)
					{
						sx = -10;
						sy = 0;
					}
					else if(state == 3)
					{
						sx = 0;
						sy = 10;
					}

					spear[0]->spawn(state,10,xpos,ypos,sx,sy);

					inventory->useAmmo(wieldWeapon);
					

				}

			}
			
			
			// COLLISIONS-----------------------------------------------------------------------------
			
			
			// Checking collision with background
			if (playerSprite->checkCollisionwithBackground(bk,newxpos,newypos)) 
			{
				// There is collision at the proposed new location
				newxpos = xpos;
				newypos = ypos;
			}

			if (playerSprite->checkCollisionwithBackground(water,newxpos,newypos)) 
			{
				// There is collision at the proposed new location
				newxpos = xpos;
				newypos = ypos;
			}

				mouseOverWater = false;
			
			if (playerSprite->checkCollisionwithBackground(water,mouseWorldx,mouseWorldy)) 
			{
				mouseOverWater = true;
			}

			
			for(i =0; i < numberofFire; i++)
			{
				int fix, fiy;
				int state;

				fire[i]->getpos(&fix,&fiy);
				
				state = fire[i]->getSpriteState();
				
				if(state == 1)
					fire[i]->updatepos();
				
				
				if((fire[i]->checkCollisionwithSprite(playerSprite,fix,fiy)) && (state == 1) && (fire[i]->isAlive()))
				{
					if(health > 0)
					health--;
					
				}
			}
				
			playerSprite->setParameterValue(0, health);
			playerSprite->setParameterValue(1, energy);

			//WITH SPRITE DISPLAYING "LOOT"--------------------------------------------------------------

			//TREE --------------------------------------------------------------------------------------		
			collideWithTree = false;
			
			int lootboxx, lootboxy;

			if((!showtreeloot) && (!mouseoninterface))
			{
			for(i = 0;i<numberoftrees;i++)
			{
				
				int currentx, currenty;
				int treex, treey;
				int noLootTemp;
				bool lootedTemp;
				
				playerSprite->getpos(&currentx,&currenty);
				
				tree[i]->getpos(&treex, &treey);
				
				tree[i]->getLoot(&noLootTemp, &lootedTemp);


					int treesrcx, treesrcy;
			
					bk->getScreencoordinate(treex,treey,&treesrcx,&treesrcy);

					if((mouse_x < treesrcx + 100) && (mouse_x > treesrcx - 100) && (mouse_y < treesrcy + 100) && (mouse_y > treesrcy - 100) && (!inventoryopen))
					{		
							mouseovertree = true;
							activetree = i;
							

					}
			}
			}

			
			if(showtreeloot)
			{
				inventory->spaceAvail();
				tree[activetree]->lootTree(buffer,lootTable,inventoryStick,lootboxx, lootboxy,inventory->returnWood(),inventory->spaceAvail());
				inventory->AddItem(tree[activetree]->getTreeLoot());
				showtreeloot = tree[activetree]->treelooted();
			}
			
			if(lootgazell)
			{
				gazell[activegazell]->lootGazell(buffer,lootTable,meat,bone,sinew, lootboxx, lootboxy,inventory->spaceAvail());
				inventory->AddItem(gazell[activegazell]->getGazellLoot());
				lootgazell = gazell[activegazell]->gazlooted();
			}

			if(lootTrap)
			{
				sapling[activesapling]->LootTrap(buffer,lootTable,rabbit,lootboxx, lootboxy,inventory->spaceAvail(),0);
				inventory->AddItem(sapling[activesapling]->trapLootReturn());
				lootTrap = sapling[activesapling]->treelooted();
			}

			if(inWaterMenu)
			{
				inventory->WaterMenu(buffer,lootboxx,lootboxy);
				inWaterMenu = inventory->returnWaterMenu();
			}


	
			// DRAWING---------------------------------------------------------------------------------
			// Set the drawing area back to normal again
			set_clip(buffer,0,0,0,0);

			stretch_blit(guibg,buffer,0,0,1024,158,0, 610, 1024, 158);

			stretch_blit(topgui,buffer,0,0,1024,29,0, 0, 1024, 29);
	
			playerSprite->setpos(newxpos,newypos);

			playerSprite->drawAvatar(buffer);

			

			stretch_blit(bag1,buffer,0,0,26,36,397,726, 26, 36);

			bk->drawMiniMap(buffer,855,638,145,110);


			
			// INVENTORY ------------------------------------------------------------------------------

			// COLLECTING ITEMS FROM THE WORLD----------------------------------------------------------

			//COLLECT ITEM FROM AN ITEM SPRITE ---------------------------------------------------------

			for(i=0; i<numberofitems; i++)
			{
				int ix,iy;
				int state;

				item[i]->getpos(&ix,&iy);

				state = item[i]->getSpriteState();

				if((item[i]->checkCollisionwithSprite(playerSprite, ix, iy)) && (item[i]->isAlive()))
				{
					item[i]->despawn();
					inventory->AddItem(state);
					nextEmpty++;
				}
			}
			//------------------------------------------------------------------------------------------

					
			int numberofrows=4;
			int inventoryitemxpos[20];
			int inventoryitemypos[4];

			
			inventoryitemxpos[0] = 0;
			inventoryitemypos[0] = 560;

			
			for(i = 1; i < 5; i++)
			{
				int j = i-1;
				inventoryitemxpos[i] = inventoryitemxpos[j] + 82;
			}

			for(i = 5; i < 20; i++)
			{
				int j = i-5;
				inventoryitemxpos[i] = inventoryitemxpos[j];
			}

			for(i = 1; i < 4; i++)
			{
				int j = i-1;	
				inventoryitemypos[i] = inventoryitemypos[j] - 50;
			}
			

			int current = 0; 


			//INVENTORYBUTTON_____________________________________________________________________________
			if((mouse_x > 395) && (mouse_x < 430) && (mouse_y > 725) && (mouse_y < 760))
			{
				stretch_blit(bag2,buffer,0,0,26,36,397,726, 26, 36);
				

				if((mouse_b & 1) && (!inventoryopen) && (itimer == 0))
				{
					inventoryopen = true;
					statsopen = false;
					itimer = 5;
				}
				else if((mouse_b & 1) && (inventoryopen) && (itimer == 0))
				{
					inventoryopen = false;
					itimer = 5;

				}

				
			}


			//STATS BUTTON______________________________________________________________________________
			if((mouse_x > 442) && (mouse_x < 475) && (mouse_y > 725) && (mouse_y < 760))
			{

				if((mouse_b & 1) && (!statsopen) && (itimer == 0))
				{
					statsopen = true;
					inventoryopen = false;
					itimer = 5;
		
				}
				else if((mouse_b & 1) && (statsopen) && (itimer == 0))
				{
					statsopen = false;
					itimer = 5;
		
				}
			}

				if(itimer > 0)
				itimer--;

			if(inventoryopen)
			{
				inventory->InventModal(inventoryopen, buffer);
			}
			else
			{
				inventory->Sort();
			}


		
			if(((mouse_x > 616) || (mouse_y < 310)))
			{
				selecteditem = -1;
				
				if((mouse_b & 1) && (!manipulate) && (itimer == 0))
				{
					inventoryopen = false;
					statsopen = false;
					selecteditem = -1;
					inventory->closeMenus();
				}
			}


			//FIRE AND FIRE MINI GAME CALL

			bool lightFire = inventory->lightFire();

			if(lightFire)
			{
					fire[fireno]->spawn(0,999999,xpos+10,ypos-10,0,0);
					inventory->setfirebool();
					fireExp = fireExp + 6;
					fireno++;

			}

			for(i=0; i < numberofFire; i++)
			{
					int fx, fy;
					int fsrcx, fsrcy;
					int state = fire[i]->getSpriteState();
					int firelit =0;
					bool cooking;


					fire[i]->getpos(& fx, &fy);
					
					bk->getScreencoordinate(fx,fy,&fsrcx,&fsrcy);

					if((mouse_x >= fsrcx - 10) && (mouse_x <= fsrcx + 10) && (mouse_y >= fsrcy - 10) && (mouse_y <= fsrcy + 10))
					{
						if((mouse_b & 1) && (state == 0))
						{
							firelit = fireminigame();
						}
						else if((mouse_b & 1) && (state == 1) && (equippedItem == MEAT))
						{
							fire[i]->setSpriteState(2);
							inventory->itemUsed();
							fire[i]->startCooking(100);
							cooking = true;
							mtimer = 5;

						}
						else if((mouse_b & 1) && (state == 2) & (mtimer == 0))
						{
							cooking = false;
							fire[i]->setSpriteState(1);
							if((fire[i]->cooktime() <= 40) && (fire[i]->cooktime()> 15))
							{
								inventory->AddItem(COOKED_MEAT);
							}

						}
						else if((mouse_b & 1) && (state == 1) && (equippedItem == RABBIT_HIDE))
						{
							inventory->itemUsed();
							inventory->AddItem(CURED_RABBIT_HIDE);
							mtimer = 5;

						}
					}

					if(firelit == 1)
					{
						fire[i]->setSpriteState(1);
					}
					
					if(state == 2)
					{
						fire[i]->cookingFire(buffer,cookingbg,fsrcx,fsrcy,cooking);
					}

			}


			if(health < 100)
			{
			health = health + inventory->healthIncrease();
			}

			inventory->healthIncreased();

			if((inventory->Drinking()) && (energy < 100))
				energy++;



			

			//WEAPON AND ITEM ACTIONS--------------------------------------------------------------------------------------------------------------------
			wieldWeapon = inventory->returnEquippedWeapon();
			
			// WEAPONS
	
			if(fireRate > 0)
				fireRate--;

			//ITEMS

			equippedItem = inventory->returnEquippedItem();

					
			//SET STATS--------------------------------------------------------------------------------------
			
			if(fireExp >= fireMaker * 1000)
			{
				fireExp = 0;
				fireMaker++;
				levelExp = levelExp + 1000;
			}

			if(levelExp >= level * 10,000)
			{
				level = 0;
				level++;
			}

			if(cookingExp >= cooking * 1000)
			{
				cookingExp = 0;
				cooking++;
				levelExp = levelExp + 1000;
			}

			if(huntingExp >= hunting * 1000)
			{
				huntingExp = 0;
				hunting++;
				levelExp = levelExp + 1000;
			}


			// STATS WINDOW -----------------------------------------------------------------------------------


			
			if(statsopen)
			{
				playerStats->DrawStats(buffer);

				/*int statx = 25;
				int healthinteger;
				int energyinteger; 
				
				healthinteger = (int) health;
				energyinteger = (int) energy;

				stretch_blit(skill2,buffer,0,0,33,34,441,726, 33, 34);
				masked_stretch_blit(statsWindow,buffer,0,0,184,241,0, 380, 184, 241);
				textprintf(buffer, font, statx, 430, colour_dkred, "Health: %i", healthinteger);
				textprintf(buffer, font, statx, 440, colour_dkgreen, "Energy: %i", energyinteger);

				textprintf(buffer, font, statx, 460, colour_yellow, "Level: %i", level);
				textprintf(buffer, font, statx, 470, colour_yellow, "Speed: %i", speed);
				textprintf(buffer, font, statx, 480, colour_yellow, "Strength: %i", strength);

				textprintf(buffer, font, statx, 500, colour_white, "Fire Lighting: %i", fireMaker);
				textprintf(buffer, font, statx, 510, colour_white, "Hunting: %i", hunting);
				textprintf(buffer, font, statx, 520, colour_white, "Cooking: %i", cooking);
				
				if((mouse_x < 180) && (mouse_x > 20) && (mouse_y > 460) && (mouse_y < 470))
				{
					stretch_blit(popup,buffer,0,0,76,80,mouse_x,mouse_y, 76, 80);
					textprintf(buffer, font, mouse_x + 10, mouse_y +10, colour_yellow, "EXP");
					textprintf(buffer, font, mouse_x + 10, mouse_y +20, colour_yellow, "To Next");

					long toNext;

					toNext = level * 10000 - levelExp;
					
					textprintf(buffer, font, mouse_x + 10, mouse_y +35, colour_yellow, "%li", toNext);
				}

				
				//get this inside a for--
				int numberofskills = 3;
				int skill[3];
				int skillexp[3];

				skill[0] = fireMaker;
				skillexp[0] = fireExp;

				skill[1] = hunting;
				skillexp[1] = huntingExp;

				skill[2] = cooking;
				skillexp[2] = cookingExp;

				int miny, maxy;

				miny = 500;
				maxy = 510;
				
				for(i = 0;i<numberofskills;i++)
				{
					if((mouse_x < 180) && (mouse_x > 20) && (mouse_y > miny) && (mouse_y < maxy))
					{
					stretch_blit(popup,buffer,0,0,76,80,mouse_x,mouse_y, 76, 80);
					textprintf(buffer, font, mouse_x + 10, mouse_y +10, colour_yellow, "EXP");
					textprintf(buffer, font, mouse_x + 10, mouse_y +20, colour_yellow, "To Next");

					int toNext;

					toNext = skill[i] * 1000 - skillexp[i];
					
					textprintf(buffer, font, mouse_x + 10, mouse_y +35, colour_yellow, "%i", toNext);
					}
					
					miny = maxy + 1;
					maxy = maxy + 10;
					
				}*/


			}
			else
			{
				stretch_blit(skill1,buffer,0,0,33,34,441,726, 33, 34);
			}

			
			for(i = 0; i < numberoftrees; i++)
			{
				if(tree[i]->timerReturn() > 0)
				mtimer = tree[i]->timerReturn();
				tree[i]->timerCount();
			}

			for(i = 0; i < numberofgazell; i++)
			{
				if(gazell[i]->timerReturn() > 0)
				mtimer = gazell[i]->timerReturn();
				gazell[i]->timerCount();
			}

			for(i = 0; i < numberofsaplings; i++)
			{
				if(sapling[i]->timerReturn() > 0)
				mtimer = sapling[i]->timerReturn();
				sapling[i]->timerCount();
			}

			int sx,sy;

			if(inventory->returnTimer() > 0)
			{
				mtimer = inventory->returnTimer();
				inventory->timerCount();
			}
			
			bk->getScreencoordinate(xpos,ypos, &sx, &sy);

			bk->getMousecoordinate(&mouseWorldx, &mouseWorldy);


			
			// MOUSE OVER DISPLAYS
			
			if((mouseovergazell) && (!mouseovertree) && (!mouseoversapling))
			{
				gazell[activegazell]->drawGazellInfo(buffer);

				if((gazell[activegazell]->getSpriteState() == 4) && (mouse_b & 1) &&(!lootgazell) && (mtimer == 0)  && (!lootTrap)  &&(!showtreeloot) && (playerSprite->isInRange(sx,sy)))
				{
					lootgazell = true;
					lootboxx = mouse_x;
					lootboxy = mouse_y;
					mtimer = 5;
				}

			}

			if((mouseovertree) && (!mouseoversapling))
			{
				tree[activetree]->drawTreeInfo(buffer);

				if((mouse_b & 1) &&(!showtreeloot) && (mtimer == 0)  && (!lootTrap) &&(!lootgazell) && (playerSprite->isInRange(sx,sy)))
				{
					showtreeloot = true;
					lootboxx = mouse_x;
					lootboxy = mouse_y;
					mtimer = 5;
				}

			}

			if(mouseoversapling)
			{
				sapling[activesapling]->drawTrapInfo(buffer,0);
				int state = sapling[activesapling]->getSpriteState();

				if(playerSprite->isInRange(sx,sy))
				{
				
					if((mouse_b & 1) && (mtimer == 0) && (state == 0))
					{
						sapling[activesapling]->SetTrap(hours,minutes,1);
						sapling[activesapling]->setSpriteState(1);
						mtimer = 5;

					}
					else if((mouse_b & 1) && (mtimer == 0) && (state > 1) && (!lootTrap)  && (!showtreeloot) && (!lootgazell))
					{
						lootTrap = true;
						lootboxx = mouse_x;
						lootboxy = mouse_y;
						mtimer = 5;

					}
					else if((mouse_b & 2) && (mtimer == 0) && (state > 1) && (!lootTrap)  && (!showtreeloot) && (!lootgazell))
					{
						sapling[activesapling]->setSpriteState(1);
						sapling[activesapling]->SetTrap(hours,minutes,1);
					}
				}
				

			}

			if(mouseOverWater)
			{

					if((mouse_b & 1) && (mtimer == 0) && (!inWaterMenu))
					{
							inWaterMenu = true;
							lootboxx = mouse_x;
							lootboxy = mouse_y;
							mtimer = 5;
						
					}

			}


			//FOREGROUND DRAWING ONTOP OF EVERYTHING
			inventory->equippedItem(buffer);
			//sd->drawBackground(buffer,512, 384,true); //SHADED LAYER

			//DEBUGGING---------------------------------------------------------------------------------

			int debugx = 10;

			if(key[KEY_X])
			{
			textprintf(buffer, font, debugx, 10, colour_white, "Frame per second: %i", frames_per_second);
			textprintf(buffer, font, debugx,20, colour_white, "Mouse x: %i", mouse_x);
			textprintf(buffer, font, debugx, 30, colour_white, "Mouse y: %i", mouse_y);
			textprintf(buffer, font, debugx, 40, colour_white, "Player x: %i", xpos);
			textprintf(buffer, font, debugx, 50, colour_white, "Player y: %i", ypos);
			textprintf(buffer, font, debugx,60, colour_white, "Mouse World x: %i", mouseWorldx);
			textprintf(buffer, font, debugx, 70, colour_white, "Mouse World y: %i", mouseWorldy);
			textprintf(buffer, font, debugx, 80, colour_white, "Over Water: %i", mouseOverWater);

			//textprintf(buffer, font, debugx, 60, colour_white, "inventopen: %i", inventoryopen);
			//textprintf(buffer, font, debugx, 70, colour_white, "Item: %i", selecteditem + 1);
			//textprintf(buffer, font, debugx, 80, colour_white, "Collide: %i", collideWithTree);
			//textprintf(buffer, font, debugx, 90, colour_white, "Empty: %i", nextEmpty);
			//textprintf(buffer, font, debugx, 100, colour_white, "Option: %i", returnoption);
			//textprintf(buffer, font, debugx, 110, colour_white, "Loot Box: %i", showtreeloot);
			//textprintf(buffer, font, debugx, 120, colour_white, "Loot Box X: %i", lootboxx);
			//textprintf(buffer, font, debugx, 130, colour_white, "Loot Box Y: %i", lootboxy);
			//textprintf(buffer, font, debugx, 140, colour_white, "Mouseoverloot: %i", selectedLoot);
			//textprintf(buffer, font, debugx, 150, colour_white, "Manipulate Y/N: %i", manipulate);
			//textprintf(buffer, font, debugx, 180, colour_white, "Wood: %i", woodcount);
			//textprintf(buffer, font, debugx, 220, colour_white, "shwLootd: %i", showtreeloot);
			//textprintf(buffer, font, debugx, 230, colour_white, "activetree: %i", activetree);

			}

			//TIMERS + BLITTING TO SCREEN-----------------------------------------------------------------

			/*if(slower < 3)
			{
				slower++;
			}
			else
			{
				slower = 0;
			}
			
			if(slower ==)*/

			sec++;
			
			if(sec == 59)
			{
				minutes ++;
				sec = 0;
				nrgDrain ++;
				if(minutes == 59)
				{
					hours++;
					minutes = 0;
					if(hours ==23)
					{
						days++;
						hours = 0;
					}
				}
			}

			if(nrgDrain == 2 + strength)
			{
				nrgDrain = 0;

				if(energy >0)
				{
					energy = energy --;;
				}
				else
				{
					health = health --;
				}

			}

			textprintf(buffer, font, 850, 10, colour_white, "Day %i", days);
			textprintf(buffer, font, 943, 10, colour_white, "%02i", hours);
			textprintf(buffer, font, 957, 10, colour_white, ":%02i", minutes);
			//textprintf(buffer, font, 980, 10, colour_white, ":%02i", sec);
			
			scare_mouse();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
			unscare_mouse();

			framecounter++;


			update_frame=0;

			if (persecondflag > 0) {
				persecondflag = 0;
				frames_per_second = framecounter;
				framecounter = 0;
			}

			if(mouse_y > 616)
			{
				mouseoninterface = true;
			}
			else
			{
				mouseoninterface = false;
			}

			if(mtimer > 0)
				mtimer --;


	if (key[KEY_ESC]) 
		keep_playing=false;
		}

	} while (keep_playing);
	//USER HAS QUIT GAME---------------------------------------------------------------------------------------------


	// END PHASE------------------------------------------------------------------------------------------------------

	// Remove the timer interrupts
	remove_int(timer_function);
	remove_int(fps_counter_function);



	delete bk;
	delete sd;

	delete water;

	delete playerSprite;

	for(i = 0; i < numberoftrees; i++)
		delete tree[i];
	delete tree;

	for(i = 0; i < numberofFire; i++)
		delete fire[i];
	delete fire;

		for(i = 0; i < numberofitems; i++)
		delete item[i];
	delete item;

			for(i = 0; i < numberofgazell; i++)
		delete gazell[i];
	delete gazell;

	delete spear[0];
	delete spear;

				for(i = 0; i < numberofsaplings; i++)
		delete sapling[i];
	delete sapling;

	//delete inventory;

	destroy_bitmap(buffer);
	destroy_bitmap(emptyinventory);
	destroy_bitmap(inventoryStick);
	destroy_bitmap(inventoryStone);
	destroy_bitmap(guibg);
	destroy_bitmap(lootTable);
	destroy_bitmap(treeLabel);
	destroy_bitmap(manipulateTable);
	destroy_bitmap(inventorySharpStick);
	destroy_bitmap(inventoryKindling);
	destroy_bitmap(inventoryHandDrill);
	destroy_bitmap(inventoryBg);
	destroy_bitmap(bag1);
	destroy_bitmap(bag2);
	destroy_bitmap(skill1);
	destroy_bitmap(skill2);
	destroy_bitmap(statsWindow);
	destroy_bitmap(popup);
	destroy_bitmap(topgui);
	destroy_bitmap(meat);
	destroy_bitmap(bone);
	destroy_bitmap(sinew);
	destroy_bitmap(spark);
	destroy_bitmap(cookedmeat);
	destroy_bitmap(cookingbg);
	destroy_bitmap(rabbit);

	delete playerStats;


	return 0;

} END_OF_MAIN();
