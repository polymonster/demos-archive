#include "main.h"

// TIMERS -------------------------------------------------------------------------------------
volatile int update_frame = 0;
volatile int persecondflag = 0;

int sec;
int minutes;
int hours;
int days;

int itimer = 0;

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
int gameLoop(BITMAP *cursor, const char datafile[])
{
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
		allegro_message("NO BG FILE");
	}

	int backgroundwidth, backgroundheight;
	bk->getDimension(&backgroundwidth, &backgroundheight);

		CBackground* sd = new CBackground();
		if (sd->initialise("data/shade.bmp",NULL,1024,768,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
		return -1;
		allegro_message("NO BG FILE");
	}

	
		CBackground* water = new CBackground();
		if (water->initialise("data/Background.bmp","Background.pgm",background_windowwidth,background_windowheight,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_width,visiblescreen_height)==0) {
		return -1;
		allegro_message("NO BG FILE");
	}

	//ITEM SPRITES------------------------------------------------------------------------------------------------------

	CMiniGames* mgames = new CMiniGames(); 

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
			allegro_message("NO ITEMS FILE");
			return 1;
		}
	}


	CSprite** fire = new CSprite*[numberofFire];
	for (i=0;i<numberofFire;i++) 
	{
		fire[i] = new CSprite();
		if (!fire[i]->initialise("data/fire sprite data.dat")) 
		{
			allegro_message("NO FIRE FILE");
			return 1;
		}
	}

	item[0]->spawn(2,99999,400,200,0,0);

	CSprite** spear = new CSprite*[1];

	spear[0] = new CSprite();
	if (!spear[0]->initialise("data/spear sprite data.dat")) 
	{
		allegro_message("NO SPEAR FILE");
		return 1;
	}

	CSprite** gazell = new CSprite*[numberofgazell];
	for (i=0;i<numberofgazell;i++) 
	{
		gazell[i] = new CSprite();
		if (!gazell[i]->initialise("data/gazell sprite data.dat")) 
		{
			allegro_message("NO GAZELLE FILE");
			return 1;
		}

		gazell[i]->setGazellHealth(15);
		gazell[i]->setLootObjects(MEAT,BONE,SINEW,0,0,0);
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
	bool mouseovertree = false;
	bool showtreeloot = false;

	//get inside load from dat file
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

		tree[i]->setLootObjects(1,1,1,1,0,0);
	}

	//TRAPS

	int numberofsaplings = 3;
	bool mouseoversapling = false;
	int activesapling;
	bool lootTrap = false;

	CSprite** sapling = new CSprite*[numberofsaplings];
	for (i=0;i<numberofsaplings;i++) 
	{
		sapling[i] = new CSprite();
		if (!sapling[i]->initialise("data/sapling sprite data.dat")) 
		{
			return 1;
		}

		sapling[i]->setLootObjects(0,0,0,0,0,0);
	}

	sapling[0]->spawn(0,999999,700,50,0,0);
	sapling[1]->spawn(0,999999,700,300,0,0);
	sapling[2]->spawn(0,999999,900,225,0,0);

	int numberofrocks = 3;

	CSprite** rocknode = new CSprite*[numberofrocks];
	for (i=0;i<numberofrocks;i++) 
	{
		rocknode[i] = new CSprite();
		if (!rocknode[i]->initialise("data/rock sprite data.dat")) 
		{
			return 1;
		}

		rocknode[i]->setLootObjects(FLINT,0,0,0,0,0);
	}

	rocknode[0]->spawn(0,9999999,1330,660,0,0);
	rocknode[1]->spawn(0,9999999,1550,780,0,0);
	rocknode[2]->spawn(0,9999999,1400,1123,0,0);


	CSprite* baseCamp = new CSprite();
	if (!baseCamp->initialise("data/camp sprite data.dat")) {
		return 1;
	}
	
	// ================================================================

	CPlayerSprite* playerSprite = new CPlayerSprite();
	if (!playerSprite->initialise("data/Player Data.dat")) {
		return 1;
	}
	
	//PLAYER VARS--------------------------------------------------------------------------------------
	int xpos;
	int ypos;

	double health ;
	double energy ;

	int cookeditem = 0;

	int nrgDrain = 0;
	int fireRate = 0;

	int equippedItem = 0;
	bool building = false;
	int buildtimer = 400;
	bool buildcomplete = false;

	//WEAPONS AND WEP STATS-----------------------------------------------------------------------------
	int Spears = 0;		
	int wieldWeapon = 0;

	int equippeditem = 0;

	int mtimer = 0;

	bool mouseoninterface = false;

	bool inbase = false;
	
	BITMAP * guibg = NULL;
	BITMAP * bag1 = NULL;
	BITMAP * skill1 = NULL;
	BITMAP * topgui = NULL;
	BITMAP * cookingbg = NULL;
	BITMAP * lootCursor =NULL;
	BITMAP * attackCursor = NULL;
	BITMAP * waterElement = NULL;
	BITMAP * fireElement = NULL;
	BITMAP * ghostcamp = NULL;

	cookingbg = load_bitmap("data/CookingBarBG.bmp",0);
	guibg =load_bitmap("data/guibg.bmp",0);
	bag1 = load_bitmap("data/BackpackButton1.bmp",0);
	skill1 = load_bitmap("data/SkillButton1.bmp",0);
	topgui = load_bitmap("data/TopBarGui.bmp",0);	
	lootCursor = load_bitmap("data/cursorLoot.bmp",0);
	attackCursor = load_bitmap("data/cursorAttack.bmp",0);
	fireElement = load_bitmap("data/FireWarn.bmp",0);
	waterElement = load_bitmap("data/WaterDroplet.bmp",0);
	ghostcamp	=	load_bitmap("data/CampTrans.bmp",0);

	bool elementActive[5];

	bool inventoryopen = false;
	bool mouseOverWater = false;
	bool inWaterMenu = false;
	bool statsopen = false;	
	bool setattackmouse = true;
	bool setgenericmouse = true;
	bool setlootmouse = true;
	bool lootingrangeset = false;
	bool mouseOverRocks = false;
	bool lootRocks = false;

	int activerocks;

	CInventory* inventory = new CInventory();
	CStats* playerStats = new CStats();
	
	inventory->Initialise();

	inventory->Load(datafile);

	inventory->Sort();
	
	playerStats->Load(datafile);

	playerStats->initialise();

	int skillx = 0;
	int inventx = 0;

	// MAKE COLOURS-----------------------------------------------------------------------------------------------------------------

	text_mode(-1);
	int colour_white = makecol(225,222,194);
	int colour_grey = makecol(128,128,128);
	int colour_yellow = makecol(225,222,0);
	int colour_green = makecol(0,190,0);
	int colour_dkred = makecol(190,0,0);
	int colour_blue = makecol(0,0,190);

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
	char buf[1024];
	char file[1024];

	ifstream maindata;
	
	maindata.open(datafile);

	maindata.getline(file,1024, '\n');
	
	ifstream playerdata;

	playerdata.open(file);

	playerdata.getline(buf,1024, '\n');

	xpos = atoi(buf);
	
	playerdata.getline(buf,1024, '\n');

	ypos = atoi(buf);

	playerdata.getline(buf,1024,'\n');

	health = atoi(buf);

	playerdata.getline(buf,1024, '\n');

	energy = atoi(buf);

	playerdata.getline(buf,1024, '\n');

	days = atoi(buf);

	playerdata.getline(buf,1024, '\n');

	hours = atoi(buf);

	playerdata.getline(buf,1024, '\n');

	minutes = atoi(buf);

	playerdata.close();
	maindata.close();

	playerSprite->setpos(xpos,ypos);

	
	int mouseWorldx, mouseWorldy;

	int fireSkillLevel;

	int huntingSkillLevel;

	int trappingSkillLevel;

	int cookingSkillLevel;

	bool inMenu = false;

	bool save = false;

	bool sleep = false;

	bool startbuilding = false;

	//GAME LOOP----------------------------------------------------------------------------------------------------------------------
	bool keep_playing = true;

	while (keep_playing) 
	{
		
		if (update_frame > 0) 
		{

				time_t seconds;
				time(&seconds);

				srand((unsigned int)seconds);	
				
			for(i=0;i<5;i++)
			{
				elementActive[i] = false;
			}


			//DRAW SPRITES/WORLD/POSITIONING-----------------------------------------------------------------------------------------
		
			clear_bitmap(buffer);	
		
			playerSprite->getpos(&xpos,&ypos);

			bk->drawBackground(buffer,xpos,ypos,false);
		
			set_clip(buffer,visiblescreen_topleft_x,visiblescreen_topleft_y,visiblescreen_bottomright_x,visiblescreen_bottomright_y);

			for(i=0;i<numberofFire;i++)
			{
				if(fire[i]->isAlive())
				{
					fire[i]->drawSprite(buffer,bk);
					fire[i]->incrementAnimationframe();
					if(fire[i]->getSpriteState() == 1)
					{
					 elementActive[1] = true;
					}
				}
							
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

				if((mouse_x > sapsrcx - 20) && (mouse_x < sapsrcx + 20) && (mouse_y > sapsrcy - 20)  && (mouse_y < sapsrcy + 20) && (!inMenu) && (!inventoryopen))
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

			for(i = 0;i <numberofrocks;i++)
			{
				if(rocknode[i]->isAlive())
					rocknode[i]->drawSprite(buffer,bk);
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

				gazell[i]->detectPlayer(1,xpos,ypos,0);
				gazell[i]->runTimerCount();

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
					ghealth = ghealth - 5;            //- damage of weap, + any poison 
					gazell[i]->setGazellHealth(ghealth);
					gazell[i]->runAway();
					playerStats->HuntingSkill(300);
					
				}

				if(gazell[i]->checkCollisionwithBackground(water, gx, gy))
				{
					gazell[i]->switchstates();
				}

				if((mouse_x > gsx - 20) && (mouse_x < gsx + 20) && (mouse_y > gsy - 20)  && (mouse_y < gsy + 20) && (!inMenu) && (!inventoryopen))
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
			{
				spear[0]->drawSprite(buffer,bk);
				spear[0]->updatepos();
			}

			if(baseCamp->isAlive()) 
			{
				int bx, by, sbx, sby;
				baseCamp->drawSprite(buffer,bk);
				baseCamp->getpos(&bx,&by);
				int state = baseCamp->getSpriteState();
				bk->getScreencoordinate(bx,by,&sbx,&sby);


				int x2 = bx + 100;
				int y2 = by + 4;

				if((!buildcomplete) && (state == 1))
				{
					buildtimer--;
				}

				if(buildtimer == 200)
				{
					baseCamp->incrementAnimationframe();	
				}
				else if((buildtimer == 0) && (!buildcomplete))
				{
					buildcomplete = true;
					buildtimer = 400;
					baseCamp->incrementAnimationframe();
				}

			}

			if(inbase)
			{
					inventory->storeInventory(buffer,true,&inbase,&save,&sleep,&building);
			}

			if(building)
			{
				baseCamp->setSpriteState(1);
				building = false;
			}


			//PLAYER CONTROL---------------------------------------------------------------------------------------------------------

			int newxpos, newypos;
			
			if (key[KEY_D]) {							

				newxpos = xpos + 3;								
				if (newxpos>backgroundwidth)					
					newxpos = backgroundwidth;					
				newypos = ypos;								
																 
				playerSprite->setSpriteState(WALK_EAST);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_A]) {

				newxpos = xpos - 3;
				if (newxpos<0)
					newxpos = 0;
				newypos = ypos;

				playerSprite->setSpriteState(WALK_WEST);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_W]) {
				newxpos = xpos;
				newypos = ypos - 3;
				if (newypos < 0)
					newypos = 0;

				playerSprite->setSpriteState(WALK_NORTH);
				playerSprite->incrementAnimationframe();

			} else if (key[KEY_S]) {
				newxpos = xpos;
				newypos = ypos + 3;
				if (newypos > backgroundheight)
					newypos = backgroundheight;

				playerSprite->setSpriteState(WALK_SOUTH);
				playerSprite->incrementAnimationframe();


			} else {				

				newxpos = xpos;
				newypos = ypos;

			}


			if(key[KEY_ALT])
			{		
					
					if(setattackmouse)
					{
					set_mouse_sprite(attackCursor);
					setattackmouse = false;
					setgenericmouse = true;
					}

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

			if(startbuilding)
			{
				masked_stretch_blit(ghostcamp,buffer,0,0,98,57,mouse_x - 40,mouse_y - 20,98,57);

				if(mouse_b & 1)
				{
					baseCamp->spawn(0,99999,mouseWorldx,mouseWorldy,0,0);
					buildcomplete = false;
					startbuilding = false;
					mtimer = 10;
				}
			}
			
			
			// COLLISIONS-----------------------------------------------------------------------------
			
			if (playerSprite->checkCollisionwithBackground(bk,newxpos,newypos)) 
			{
				newxpos = xpos;
				newypos = ypos;
			}

			if (playerSprite->checkCollisionwithBackground(water,newxpos,newypos)) 
			{
				newxpos = xpos;
				newypos = ypos;
			}

			mouseOverRocks = false;

			for(i = 0; i<numberofrocks;i++)
			{
				int rx, ry;

				rocknode[i]->getpos(&rx,&ry);
				
				if((newxpos >= rx - 60) && (newxpos <= rx + 60) && (newypos <= ry + 50) && (newypos >= ry - 50))
				{
					newxpos = xpos;
					newypos = ypos;
				}


				if (playerSprite->checkCollisionwithSprite(rocknode[i],mouseWorldx,mouseWorldy)) 
				{
					mouseOverRocks = true;
					activerocks = i;
				}
			}

			mouseOverWater = false;
			
			if (playerSprite->checkCollisionwithBackground(water,mouseWorldx,mouseWorldy)) 
			{
				mouseOverWater = true;
			}

			if(baseCamp->isAlive()) //&& (baseCamp->getSpriteState() == 1))
			{
				int bx, by;

				baseCamp->getpos(&bx,&by);

				if((newxpos >= bx - 40) && (newxpos <= bx + 40) && (newypos <= by + 28) && (newypos >= by + 15))
				{
					newxpos = xpos;
					newypos = ypos;
				}

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

			set_clip(buffer,0,0,0,0);

			
			playerSprite->setParameterValue(0, health);
			playerSprite->setParameterValue(1, energy);

	
			// DRAWING---------------------------------------------------------------------------------
			// Set the drawing area back to normal again

			stretch_blit(guibg,buffer,0,0,1024,158,0, 610, 1024, 158);

			stretch_blit(topgui,buffer,0,0,1024,29,0, 0, 1024, 29);
	
			playerSprite->setpos(newxpos,newypos);

			playerSprite->drawAvatar(buffer);

			stretch_blit(bag1,buffer,inventx,0,26,36,397,726, 26, 36);

			stretch_blit(skill1,buffer,skillx,0,33,34,441,726, 33, 34);

			bk->drawMiniMap(buffer,852,636,152,114);

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
				}
			}
			//INVENTORYBUTTON_____________________________________________________________________________
			if((mouse_x > 395) && (mouse_x < 430) && (mouse_y > 725) && (mouse_y < 760))
			{

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
				inventory->InventModal(&inventoryopen,buffer,&startbuilding);
				inventx = 26;

			}
			else
			{
				inventory->Sort();
				inventx = 0;
			}

			if(statsopen)
			{
				playerStats->DrawStats(buffer);
				skillx = 33;

			}
			else
			{
				skillx = 0;
			}

			if(((mouse_x > 616) || (mouse_y < 310)))
			{

				if((mouse_b & 1) && (itimer == 0))
				{
					statsopen = false;
				}
			}


			//FIRE AND FIRE MINI GAME CALL

			bool lightFire = inventory->lightFire();

			fireSkillLevel = playerStats->getFireSkill();
			trappingSkillLevel = playerStats->getTrappingSkill();
			huntingSkillLevel = playerStats->getHuntingSkill();
			cookingSkillLevel = playerStats->getCookingSkill();

			if(lightFire)
			{
					fire[fireno]->spawn(0,999999,xpos+10,ypos-10,0,0);
					inventory->setfirebool();
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
							int exp;
							int lifespan;

							firelit = mgames->fireminigame(fireSkillLevel,&exp,&lifespan);
							fire[i]->setLifeSpan(lifespan);

							if(lifespan > 0)
								fire[i]->setSpriteState(1);
								
							playerStats->FireSkill(exp);

						}
						else if(((mouse_b & 1) && (state == 1)) && ((equippedItem == MEAT) || (equippedItem == RABBIT_MEAT) || (equippedItem == FISH_ONE)))
						{
	
							fire[i]->setSpriteState(2);
							inventory->itemUsed();
							fire[i]->startCooking(100);
							cooking = true;
							mtimer = 5;

							if(equippedItem == MEAT)
							{
								cookeditem = COOKED_MEAT;
							}
							else if(equippedItem == RABBIT_MEAT)
							{
								cookeditem = COOKED_RABBIT_MEAT;
							}
							else if(equippedItem == FISH_ONE)
							{
								cookeditem = COOKED_FISH_ONE;
							}

						}
						else if((mouse_b & 1) && (state == 2) & (mtimer == 0))
						{
							cooking = false;
							fire[i]->setSpriteState(1);
							
							if((fire[i]->cooktime() <= 40) && (fire[i]->cooktime()> 15))
							{
								inventory->AddItem(cookeditem);
								playerStats->CookingSkill(cookeditem * 10);
							}

							cookeditem = 0;

						}
						else if((mouse_b & 1) && (state == 1) && (equippedItem == RABBIT_HIDE))
						{
							inventory->itemUsed();
							inventory->AddItem(CURED_RABBIT_HIDE);
							mtimer = 5;

						}
					}
					
					if(state == 2)
					{
						fire[i]->cookingFire(buffer,cookingbg,fsrcx,fsrcy,cooking);
					}

			}


			//WEAPON AND ITEM ACTIONS--------------------------------------------------------------------------------------------------------------------
			wieldWeapon = inventory->returnEquippedWeapon();
			
			// WEAPONS
	
			if(fireRate > 0)
				fireRate--;

			//ITEMS

			equippedItem = inventory->returnEquippedItem();

			//====================================================================================================================================

						//WITH SPRITE DISPLAYING "LOOT"--------------------------------------------------------------
			
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
				
					int treesrcx, treesrcy;
			
					bk->getScreencoordinate(treex,treey,&treesrcx,&treesrcy);

					if((mouse_x < treesrcx + 100) && (mouse_x > treesrcx - 100) && (mouse_y < treesrcy + 100) && (mouse_y > treesrcy - 100) && (!inventoryopen))
					{		
							mouseovertree = true;
							activetree = i;
							

					}
			}
			}

			int loot1,loot2,loot3,loot4,loot5,loot6;

			if(showtreeloot)
			{
				
				tree[activetree]->getLootObjects(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				inventory->LootNode(loot1,loot2,loot3,loot4,loot5,loot6,buffer,lootboxx, lootboxy);
				inventory->updateLoot(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				tree[activetree]->setLootObjects(loot1,loot2,loot3,loot4,loot5,loot6);
				showtreeloot = inventory->returnLootingBool();
			}
			
			if(lootgazell)
			{
				gazell[activegazell]->getLootObjects(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				inventory->LootNode(loot1,loot2,loot3,loot4,loot5,loot6,buffer,lootboxx, lootboxy);
				inventory->updateLoot(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				gazell[activegazell]->setLootObjects(loot1,loot2,loot3,loot4,loot5,loot6);
				lootgazell = inventory->returnLootingBool();
			}

			if(lootRocks)
			{
				rocknode[activerocks]->getLootObjects(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				inventory->LootNode(loot1,loot2,loot3,loot4,loot5,loot6,buffer,lootboxx, lootboxy);
				inventory->updateLoot(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				rocknode[activerocks]->setLootObjects(loot1,loot2,loot3,loot4,loot5,loot6);
				lootRocks = inventory->returnLootingBool();
			}

			if(lootTrap)
			{
				sapling[activesapling]->getLootObjects(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				inventory->LootNode(loot1,loot2,loot3,loot4,loot5,loot6,buffer,lootboxx, lootboxy);
				inventory->updateLoot(&loot1,&loot2,&loot3,&loot4,&loot5,&loot6);
				sapling[activesapling]->setLootObjects(loot1,loot2,loot3,loot4,loot5,loot6);
				lootTrap = inventory->returnLootingBool();
			}

			if(inWaterMenu)
			{
				int option;

				inventory->WaterMenu(buffer,lootboxx,lootboxy,&option);
				inWaterMenu = inventory->returnWaterMenu();

				if(option == 1)
				{	
					option = 0;
					int exp, yourcatch;
					mgames->fishingminigame(1,&exp,&yourcatch);
					playerStats->FishingSkill(exp);

					for(int i =0;i<yourcatch;i++)
					{
						inventory->AddItem(FISH_ONE);
					}


				}
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


			lootingrangeset = false;

			//MOUSE OVER

			if((!lootgazell) && (!lootTrap) && (!showtreeloot) && (!inWaterMenu) && (!inbase))
			{
				inMenu = false;
			}


			if((!inventoryopen) && (!statsopen) && (!inMenu))
			{
			
			if((mouseovergazell) && (!mouseovertree) && (!mouseoversapling))
			{
				gazell[activegazell]->drawGazellInfo(buffer);

				if(playerSprite->isInRange(sx,sy))
				{
				lootingrangeset = true;

				if((gazell[activegazell]->getSpriteState() == 4) && (mouse_b & 1) && (mtimer == 0))
				{
					lootgazell = true;
					inMenu = true;
					lootboxx = mouse_x;
					lootboxy = mouse_y;
					mtimer = 5;
				}
				}

			}

			if((mouseovertree) && (!mouseoversapling))
			{
				tree[activetree]->drawStaticInfo(buffer, 0);

				if(playerSprite->isInRange(sx,sy))
				{
					lootingrangeset = true;

				if((mouse_b & 1) &&(mtimer == 0))
				{
					showtreeloot = true;
					inMenu = true;
					lootboxx = mouse_x;
					lootboxy = mouse_y;
					mtimer = 5;
				}
				}

			}

			if(mouseoversapling)
			{
				sapling[activesapling]->drawTrapInfo(buffer,0);
				int state = sapling[activesapling]->getSpriteState();

				if(playerSprite->isInRange(sx,sy))
				{
					//set_mouse_sprite(lootCursor);
					lootingrangeset = true;

					if((mouse_b & 1) && (mtimer == 0) && (state == 0))
					{
						sapling[activesapling]->SetTrap(hours,minutes,trappingSkillLevel);
						sapling[activesapling]->setSpriteState(1);
						mtimer = 5;

					}
					else if((mouse_b & 1) && (mtimer == 0) && (state > 1))
					{
						lootTrap = true;
						inMenu = true;
						lootboxx = mouse_x;
						lootboxy = mouse_y;
						mtimer = 5;

					}
					else if((mouse_b & 2) && (mtimer == 0) && (state > 1))
					{
						sapling[activesapling]->setSpriteState(1);
						sapling[activesapling]->SetTrap(hours,minutes,trappingSkillLevel);
					}
				}
				

			}

			if((mouseOverWater) && (playerSprite->isInRange(sx,sy)))
			{
					lootingrangeset = true;

					if((mouse_b & 1) && (mtimer == 0))
					{
							inWaterMenu = true;
							inMenu = true;
							lootboxx = mouse_x;
							lootboxy = mouse_y;
							mtimer = 5;
						
					}

			}

			
			if((mouseOverRocks) && (playerSprite->isInRange(sx,sy)))
			{
					lootingrangeset = true;

					if((mouse_b & 1) && (mtimer == 0))
					{
							lootRocks = true;
							inMenu = true;
							lootboxx = mouse_x;
							lootboxy = mouse_y;
							mtimer = 5;
						
					}

			}
			
			if(mouseOverRocks)
			{
				rocknode[activerocks]->drawStaticInfo(buffer,1);
			}
			

				if((playerSprite->checkCollisionwithSprite(baseCamp,mouseWorldx,mouseWorldy)) && (mouse_b & 1) && (mtimer == 0))
				{
					inbase = true;
					inMenu = true;
					mtimer = 10;
				}
			}
			

			
			if((lootingrangeset) && (setlootmouse))
			{
				set_mouse_sprite(lootCursor);
				setlootmouse = false;
				setgenericmouse = true;
				setattackmouse = true;
			}
			else if((setgenericmouse) && (!lootingrangeset) && (!key[KEY_ALT]))
			{
				set_mouse_sprite(cursor);
				setgenericmouse = false;
				setlootmouse = true;
				setattackmouse = true;
			}

			
			if(elementActive[0])
			{
				masked_stretch_blit(waterElement,buffer,0,0,45,50,970,35,45,50);
			}

			if(elementActive[1])
			{
				masked_stretch_blit(fireElement,buffer,0,0,45,50,910,35,45,50);
			}

			//FOREGROUND DRAWING ONTOP OF EVERYTHING
			inventory->equippedItem(buffer);

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
				textprintf(buffer, font, debugx, 90, colour_white, "BT: %i", buildtimer);

	
			}

			//TIMERS + BLITTING TO SCREEN-----------------------------------------------------------------

			if(save)
			{
				inventory->Save(datafile);

				playerStats->Save(datafile);
				
				ofstream saveplyr;
				
				saveplyr.open(file, ios::out);
				
				saveplyr << xpos << "\n";
				saveplyr << ypos << "\n";
				saveplyr << health<< "\n";
				saveplyr << energy << "\n";
				saveplyr << days << "\n";
				saveplyr << hours << "\n";
				saveplyr << minutes << "\n";
				
				saveplyr.close();

				save = false;
				
			}

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
					if(hours == 24)
					{
						days++;
						hours = 0;
					}
				}
			}

			if(nrgDrain == 2)
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

			textprintf(buffer, font, 880, 10, colour_white, "Day %i", days);
			textprintf(buffer, font, 957, 10, colour_white, "%02i", hours);
			textprintf(buffer, font, 971, 10, colour_white, ":%02i", minutes);
	
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

			if(health < 100)
			{
				health = health + inventory->healthIncrease();
				//health = health + cookingSkillLevel;
			}
			else if(health > 100)
			{
				health = 100;
			}

			inventory->healthIncreased();

			if((inventory->Drinking()) && (energy < 100))
				energy++;


	if (key[KEY_ESC]) 
		keep_playing=false;
		}

	} while (keep_playing);
	//USER HAS QUIT GAME---------------------------------------------------------------------------------------------


	// END PHASE------------------------------------------------------------------------------------------------------
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

	destroy_bitmap(buffer);
	destroy_bitmap(guibg);
	destroy_bitmap(bag1);
	destroy_bitmap(skill1);
	destroy_bitmap(topgui);
	destroy_bitmap(cookingbg);
	destroy_bitmap(lootCursor);
	destroy_bitmap(attackCursor);
	destroy_bitmap(waterElement);
	destroy_bitmap(fireElement);
	destroy_bitmap(ghostcamp);

	delete playerStats;

	for(i = 0; i < numberofrocks; i++)
	delete rocknode[i];
	delete rocknode;
	delete baseCamp;

	delete mgames;

	//delete inventory;

	return 1;

}

int main(void) 
{

	if (!initialise_allegro()) 
	{
		allegro_message("Initialisation Failure\n % s\n", allegro_error );
		return 1;
	}

	set_window_title("Bush Craft");
	set_color_depth(32);
	int window_width = 1024;
	int window_height = 768;

	if ( set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, window_width, window_height , 0 , 0 ) < 0 )	
	{
		allegro_message("Unable initialize graphics module\n % s\n", allegro_error );
		return  1;
	}

	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	BITMAP *cursor = NULL;
	cursor = load_bitmap("data/cursor.bmp",0);
	BITMAP *config = NULL;
	BITMAP *minigames = NULL;
	BITMAP *start = NULL;
	BITMAP *noOption = NULL;
	BITMAP *quit = NULL;

	config = load_bitmap("Main Menu/Options.bmp",0);
	minigames = load_bitmap("Main Menu/Minigames.bmp",0);
	start = load_bitmap("Main Menu/Start.bmp",0);
	noOption = load_bitmap("Main Menu/None.bmp",0);
	quit = load_bitmap("Main Menu/Quit.bmp",0);


	int colour_white = makecol(225,222,194);

	int selectedslot;
	int texty = 30;
	int statrtrn1, statrtrn2;
	int menuChoice = 0;

	bool inmain = true;
	bool createnewgame = true;

	char* slotname[5];

	slotname[0] = "Savefiles/slot0.dat";
	slotname[1] = "Savefiles/slot1.dat";
	slotname[2] = "Savefiles/slot2.dat";
	slotname[3] = "Savefiles/slot3.dat";
	slotname[4] = "Savefiles/slot4.dat";

	CData* data = new CData();
	CMiniGames* mgames = new CMiniGames(); 
	
	CBackground* mbg = new CBackground();
	if(mbg->initialise("Main Menu/MenuBG.bmp",NULL,window_width,window_height,0,0,window_width,window_height)==0)
	{
		return 1;
	}

	
	show_mouse(screen);

	set_mouse_sprite(cursor);

	while(inmain)
	{
		
		mbg->drawBackground(buffer,512,384,false);
		stretch_blit(noOption,buffer,0,0,337,264,329,440,337,264);
		textprintf(buffer, font, 0, 0, colour_white, "mouse x%i", mouse_x);
		textprintf(buffer, font, 0, 10, colour_white, "mouse y%i", mouse_y);
		//textprintf(buffer, font, 0, 20, colour_white, "Key [2] - To Load Saved Game");
		//textprintf(buffer, font, 0, 30, colour_white, "Key [3] - Fire Mini Game");
		//textprintf(buffer, font, 0, 40, colour_white, "Key [4] - Fishing Mini Game");
		//textprintf(buffer, font, 0, 100, colour_white, "Key [0] - To Quit Game");

		int ymin = 450;
		int ymax = 505;

		menuChoice = 0;

		for(int i = 0; i < 4; i++)
		{
		
		if((mouse_x > 330) && (mouse_x < 667) && (mouse_y > ymin) && (mouse_y < ymax))
		{
			menuChoice = i + 1;
		}

		ymin = ymax + 8;
		ymax = ymax + 67;


		}

		if(menuChoice == 1)
		{
			stretch_blit(start,buffer,0,0,337,264,329,440,337,264);

			if(mouse_b & 1)
			{
				data->LoadGame(buffer,cursor,0,&selectedslot,mbg);
				if(selectedslot > -1)
				{
					gameLoop(cursor,slotname[selectedslot]);
				}
			}
			
		}
		else if(menuChoice == 2)
		{
			stretch_blit(minigames,buffer,0,0,337,264,329,440,337,264);
		}
		else if(menuChoice == 3)
		{
			stretch_blit(config,buffer,0,0,337,264,329,440,337,264);
		}
		else if(menuChoice == 4)
		{
			stretch_blit(quit,buffer,0,0,337,264,329,440,337,264);

			if(mouse_b & 1)
			{
				inmain = false;
			}
		}



	/*	if(key[KEY_1])
		{
			data->LoadGame(buffer,cursor,0,&selectedslot);
			
			if(selectedslot > -1)
			{
				gameLoop(cursor,slotname[selectedslot]);
			}
		}
		else if(key[KEY_2]) // LOAD
		{
			data->LoadGame(buffer,cursor,1,&selectedslot);
			
			if(selectedslot > -1)
			{
				gameLoop(cursor,slotname[selectedslot]);
			}	
		}
		else*/ if(key[KEY_3])
		{
			mgames->fireminigame(5,&statrtrn1,&statrtrn2);
		}
		else if(key[KEY_4])
		{
			mgames->fishingminigame(5,&statrtrn1,&statrtrn2);
		}
		else if(key[KEY_0])
		{
			inmain = false;
		}

		scare_mouse();	
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
		unscare_mouse();

		clear_bitmap(buffer);

	}

	delete mgames;
	delete data;
	delete mbg;
	

	destroy_bitmap(cursor);
	destroy_bitmap(config);
	destroy_bitmap(minigames);
	destroy_bitmap(noOption);
	destroy_bitmap(start);
	destroy_bitmap(quit);

	return 0;





} 

END_OF_MAIN();
