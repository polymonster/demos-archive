#include <allegro.h>
#include <time.h>
#include "Background.h"
#include "DataManagement.h"
#include "PlayerSprite.h"
#include "Sprite.h"
#include "Inventory.h"
#include "Stats.h"
#include "Definitions.h"
#include "MiniGames.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "string.h"

using namespace std;


int CMiniGames::fireminigame(int fireSkillLevel, int *exp, int *lifespan)
{
	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	BITMAP *unlitfire = NULL;
	BITMAP *litfire1 = NULL;
	BITMAP *litfire2 = NULL;
	BITMAP *litfire3 = NULL;
	BITMAP *result0 = NULL;
	BITMAP *result1 = NULL;
	BITMAP *result2 = NULL;
	BITMAP *result3 = NULL;
	BITMAP *numbers = NULL;

	unlitfire =load_bitmap("data/fireanimguiUNLIT.bmp",0);
	litfire1 =load_bitmap("data/fireanimguiLITSMALL.bmp",0);
	litfire2 =load_bitmap("data/fireanimguiLITLARGE.bmp",0);
	litfire3 =load_bitmap("data/fireanimguiLITGIANT.bmp",0);
	result0 = load_bitmap("data/ResultFail.bmp",0);
	result1 = load_bitmap("data/ResultSmall.bmp",0);
	result2 = load_bitmap("data/ResultLarge.bmp",0);
	result3 = load_bitmap("data/ResultHuge.bmp",0);
	numbers = load_bitmap("data/09.bmp",0);

	int colour_white = makecol(225,222,194);
	int colour_grey = makecol(128,128,128);
	int colour_yellow = makecol(225,222,0);
	int colour_green = makecol(0,190,0);
	int colour_dkred = makecol(190,0,0);
	int colour_blue = makecol(0,0,190);

	int firequality = 0;

	int window_width = 1024;
	int window_height = 768;

	int frameControl = 0;

	string scorearray;


	CBackground* fmg = new CBackground();
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
	int speed = 1;
	int round = 0;
	int nodes = 1;
	int spawnrate = 130;
	int acc = 0;
	unsigned int score = 0;
	int miss = 0;
	int addpoints = 0;

	bool heldkey = false;

	do
	{

		
		if(frameControl == 0)
		{
		fireMan[0]->incrementAnimationframe();
		frameControl = 5;
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



		for(i=0;i<12;i++)
		{
			int xpos, ypos;
			int state;
			
			droppingitem1[i]->getpos(&xpos,&ypos);
			state = droppingitem1[i]->getSpriteState();


			if((xpos == 605) && (ypos < 340) && (ypos > 285)  && (!heldkey))  //COLUMN L
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
			
			if((xpos == 881) & (ypos < 340) && (ypos > 285)  && (!heldkey)) //COLUMN R
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
			
			if((xpos == 697) & (ypos < 340) && (ypos > 285)  && (!heldkey)) //COLUMN R
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
			
			if((xpos == 789) & (ypos < 340) && (ypos > 285) && (!heldkey)) //COLUMN R
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
				speed;
				spawnrate = spawnrate - 75;
			}

			acc = 0;
			
		}

		if((key[KEY_W]) || (key[KEY_A])  || (key[KEY_S]) ||(key[KEY_D]) ||(key[KEY_UP])  || (key[KEY_DOWN])  || (key[KEY_LEFT]) || (key[KEY_RIGHT]))
		{
			heldkey = true;
		}
		else
		{
			heldkey = false;
		}
	

				
		set_clip(buffer,0,0,0,0);
		
		fmg->drawBackground(buffer,window_width/2,window_height/2,NULL);
		fireMan[0]->drawSprite(buffer,fmg);
		int scorearray[4];

		for(i = 0; i<5;i++)
		{
			scorearray[i] = 0;
		}

		for(i=0; i<score;i++)
		{
			scorearray[4]++;
			
			if(scorearray[4] == 10)
			{
				scorearray[3]++;
				scorearray[4] = 0;
			}
			
			if(scorearray[3] == 10)
			{
				scorearray[2]++;
				scorearray[3] = 0;
			}
			
			if(scorearray[2] == 10)
			{
				scorearray[1]++;
				scorearray[2] = 0;
			}

			if(scorearray[1] == 10)
			{
				scorearray[0]++;
				scorearray[1] = 0;
			}

		}

		int xx = 0;

		for(i = 0; i < 5; i++)
		{
			masked_stretch_blit(numbers,buffer,(scorearray[i] * 25),0,25,25,xx,0,25,25);
			xx =xx + 27;
		}

		//textprintf(buffer,font,0,0,colour_white,"%i",heldkey);

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


		//textprintf(buffer, font, 0,20, colour_white, "Mouse x: %i", mouse_x);
		//textprintf(buffer, font, 0, 30, colour_white, "Mouse y: %i", mouse_y);
		//textprintf(buffer, font, 0, 40, colour_white, "spawner: %i", spawntimer);
		//textprintf(buffer, font, 0, 50, colour_white, "Node: %i", activenode);
		//textprintf(buffer, font, 0, 60, colour_white, "column: %i", column);
		//textprintf(buffer, font, 0, 70, colour_white, "ACCUM: %i", acc);
		//textprintf(buffer, font, 0, 80, colour_white, "Round: %i", round);
		//textprintf(buffer, font, 512, 300, colour_white, "%i", score);

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


		

	}while((miss < fireSkillLevel) && (round < 6) && (!mouse_b & 1));
	
	bool endmsg = true;
	int msgtimer = 1000;
	
	do
	{
		
		int windowx = window_width/2 - 76;
		int windowy = window_height/2 - 105;
		
		if(round == 5)
		{
			masked_stretch_blit(result3,buffer,0,0,152,209,windowx,windowy, 152, 209);
			*lifespan = 3000;
		}
		else if (round == 4)
		{
			masked_stretch_blit(result2,buffer,0,0,152,209,windowx,windowy, 152, 209);
			*lifespan = 1500;
		}
		else if (round == 3)
		{
			masked_stretch_blit(result2,buffer,0,0,152,209,windowx,windowy, 152, 209);
			*lifespan = 900;
		}
		else if (round == 2)
		{
			masked_stretch_blit(result1,buffer,0,0,152,209,windowx,windowy, 152, 209);
			*lifespan = 300;
		}
		else if (round == 1)
		{
			masked_stretch_blit(result0,buffer,0,0,152,209,windowx,windowy, 152, 209);
			*lifespan = 0;
		}

		//textprintf_centre_ex(buffer, font,windowx+76,windowy+55, colour_white,-1, "-=Score=-");

		//textprintf_centre_ex(buffer, font,windowx+76,windowy+65, colour_white,-1, "%05i",score);

		*exp = score / 10;

		msgtimer--;

		set_clip(buffer,0,0,0,0);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
	
	}while(!mouse_b & 1);



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
	destroy_bitmap(result0);
	destroy_bitmap(result1);
	destroy_bitmap(result2);
	destroy_bitmap(result3);
	destroy_bitmap(numbers);


	return firequality;

}

int CMiniGames::fishingminigame(int fishingSkillLevel, int *exp, int *yourcatch)
{
	time_t seconds;
	time(&seconds);

	srand((unsigned int)seconds);
	
	BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
	BITMAP *slider = NULL;
	BITMAP *slider2 = NULL;
	BITMAP *aimbar = NULL;
	BITMAP *hook = NULL;
	BITMAP *numbers = NULL;

	slider = load_bitmap("data/Slider.bmp",0);
	slider2 = load_bitmap("data/SliderSilver.bmp",0);
	aimbar = load_bitmap("data/AimBar.bmp",0);
	hook = load_bitmap("data/hook.bmp",0);
	numbers = load_bitmap("data/09.bmp",0);

	int window_width = 1024;
	int window_height = 768;
	int colour_white = makecol(225,222,194);
	int colour_green = makecol(0,128,0);
	int colour_blue = makecol(0,0,128);
	int colour_red = makecol(128,0,0);
	int colour_a = makecol(128,0,128);

	int score = 0;


	CBackground* fishmg = new CBackground();
	if(fishmg->initialise("data/FishingBackground.bmp","FishingBackgroundCollision.pgm",window_width,window_height,0,0,window_width,window_height)==0)
	{
		return -1;
	}

	CSprite* fish = new CSprite;
	fish->initialise("data/fish sprite data.dat");

	//SCHOOLS OF FISH---------------------------------

	CSprite* school = new CSprite;

	school->initialise("data/fishschool sprite data.dat");
	
	int schoolroam = rand()%300 + 100;
	int schoolrate = 0;
	int schoolycount = 300;

	int sx = rand()%800;
	int sy = rand()%200 + 220;

	school->spawn(0,99999,sx,sy,-1,0);
	

	bool ingame = true;
	bool caughtfish = false;
	bool cast = false;
	bool drawncast = false;
	bool xbarincrease = true;

	int endoflinex = 50;
	int endofliney = 400;
	int linelength;

	linelength = 950 - endoflinex;

	int point1x = 950 - (linelength /2);
	int point1y = 200;
	int hookx;
	int hooky;
	int rate = 0;
	int barx1 = 616;
	int bary1 = 721;
	int barx2 = 617;
	int bary2 = 749;
	int barspeed = 1;
	int fishrange;
	int fishdepth;


	int mtimer = 0;

	point1y = 150;
	int cyclecounter = 0;

	int slowrate = 0;

	int fishcount = 0;

	bool setrange = true;
	bool mousepress = false;
	bool dopowerbar = false;
	bool fishreeledin = false;


	int catchtimer = 0;
	int catchrand  = 0;

	int curve = 0;

	int upordown;
	upordown = rand()%100;

	int newendoflinex;
	int newendofliney;

	

	do
	{
		int line[8];

		line[0] = 1024;	// xpos1
		line[1] = 150;	//ypos1
		line[2] = point1x; //1st Horizontal
		line[3] = point1y; //1st vertical
		line[4] = point1x;; //2nd/NULL
		line[5] = point1y;; //2nd/NULL
		line[6] = endoflinex; //endx
		line[7] = endofliney; //endy

		clear_bitmap(buffer);

		set_clip(buffer,0,0,window_width,window_height);

		fishmg->drawBackground(buffer,window_width/2,window_height/2,false);

		const int points[] = {line[4],line[0],line[5],line[1],line[6],line[2],line[7],line[3]};


		if((mouse_b&1) && (!dopowerbar))
		{
			dopowerbar = true;
			mtimer = 15;
		}

		if(mtimer > 0)
		{
			mtimer--;
		}

		if((!cast) && (dopowerbar))
		{
			int schx,schy;
			int powerbar;
			
			school->getpos(&schx,&schy);
	
			fishrange = barx1 + ((950 - schx) / 4);
			fishdepth = schy + barx1 - 200;
		

			if(setrange)
			{
				powerbar = barx2;
				rectfill(buffer,barx1, bary1, barx2, bary2, colour_green);
				masked_stretch_blit(slider,buffer,0,0,7,35,fishrange,bary1,7,35);
			}
			else
			{
				rectfill(buffer,barx1, bary1, powerbar, bary2, colour_green);
				
				masked_stretch_blit(aimbar,buffer,0,0,63,30,barx2,bary1,30,30);

				masked_stretch_blit(slider,buffer,0,0,7,35,fishrange,bary1,7,35);

				if(fishdepth < 916)
				{
					masked_stretch_blit(slider2,buffer,0,0,7,35,fishdepth,bary1,7,35);
				}
				else
				{
					masked_stretch_blit(slider2,buffer,0,0,7,35,916,bary1,7,35);
				}
			}
				

			if(((mouse_b & 1) && (setrange) && (mtimer == 0)) || (cyclecounter == 2))
			{
				setrange = false;
				hookx = 950 - ((barx2 - barx1) * 4);
				mousepress = true;
				barx2 = 617;
				barspeed = 2;
				cyclecounter = 0;
				mtimer = 5;
			}
			else if((mouse_b & 1) && (!setrange) && (!mousepress))
			{
				cast = true;
				setrange = true;
				dopowerbar = false;
				barspeed = 1;
				hooky = barx2 - barx1 + 200;
				point1y = 150;
				barx2 = 617;
				catchrand = rand()%1000;
				catchtimer = (1000/fishingSkillLevel) + catchrand;
			}

			if(!(mouse_b & 1))
			{ 
				mousepress = false;
			}
		

			endoflinex = 1000;
			endofliney = 100;

			position_mouse_z(hookx);
			caughtfish = false;

			if(!xbarincrease)
			{
				if(barx2 > barx1)
				{
				barx2 = barx2 - barspeed;
				}
				else
				{
				xbarincrease = true;
				cyclecounter++;
				}
			}
			else
			{
				if(barx2 < barx1 + 300)
				{
					barx2 = barx2 + barspeed;
				}	
				else
				{
					xbarincrease = false;
					cyclecounter++;
				}
			}

			
		}

		//DrawCast----------------------------------------------------------------------------------------------------------------------------	


		if(cast)
		{
			dopowerbar = false;
			int midx = (950 - hookx) / 2 + hookx;

			if(!drawncast)
			{
				
				if(endoflinex > midx) //drawing it out
				{
					endoflinex =  endoflinex - 10;
					endofliney = endofliney --;
					point1y--;


				}
				else if((endofliney < hooky) || (endoflinex > hookx)) //moving the line down
				{
					
					
					endoflinex =  endoflinex - 3;
					if(endofliney < hooky)
					{
					endofliney =  endofliney + 3;
					}

					linelength = 950 - endoflinex;
					point1x = 950 - (linelength /2);
					

				}
				else
				{
					drawncast = true;
					score = score + 100;
				}

				newendoflinex = endoflinex;
				newendofliney = endofliney;
				masked_stretch_blit(hook,buffer,0,0,10,13,endoflinex - 6,endofliney,10,13);

			}

		//------------------------------------------------------------------------------------------------------------------------------
				

		if(drawncast)	//waiting for bite
		{
			int schoolx, schooly;

			school->getpos(&schoolx,&schooly);

	
			if((mouse_b & 2) && (endofliney > 210))
			{
				newendofliney = endofliney - 2;
			}

			if(endoflinex < 900)
			{
				newendoflinex = mouse_z * -1;


				if(newendoflinex < endoflinex)
				{
					newendoflinex = endoflinex;
				}
				else if (newendoflinex > endoflinex)
				{
					newendoflinex = newendoflinex + 2;
				}

				position_mouse_z(newendoflinex);
				
			}

			if(fish->checkCollisionwithSprite(school,endoflinex,endofliney) && (!caughtfish))
			{
				catchtimer--;

				if(catchtimer > 0)
				{
						arc(buffer,schoolx,schooly,0,itofix(catchtimer),20,colour_red);
						arc(buffer,schoolx,schooly,0,itofix(catchtimer),40,colour_red);

						int radius = 20;

						for(int i =0;i <20;i++)
						{
							arc(buffer,schoolx,schooly,0,itofix(catchtimer),radius,colour_red);
							radius++;
						}


						fastline(buffer,schoolx+20,schooly,schoolx+40,schooly,colour_red);

						floodfill(buffer,schoolx+24,schooly-3,colour_red);

				}
			}
			else
			{
				catchtimer = (1000/fishingSkillLevel) + catchrand;
			}

			masked_stretch_blit(hook,buffer,0,0,10,13,endoflinex - 5,endofliney,10,13);

			if(catchtimer == 0)
			{
				caughtfish = true;
				score = score + 573;
				fish->spawn(0,9999999,endoflinex,endofliney,0,0);
				catchtimer = 1000/fishingSkillLevel + catchrand;
			}

 
			if(caughtfish)
			{

				if(slowrate == 0) // BASE THIS ON FISH STRENGTH
				{
					newendoflinex--;			
					newendofliney++;
					slowrate = 5;
				}

				if(slowrate > 0)
					slowrate--;

				fish->setSpeed(0,0);

				fish->setpos(newendoflinex + 20,newendofliney);

				if((endofliney < 250) && (endoflinex > 850))
				{
					fishreeledin = true;
					caughtfish = false;
					curve = 0;
				}
				
				if(fish->checkCollisionwithBackground(fishmg,endoflinex - 20,endofliney - 20))
				{
					newendoflinex = endoflinex;
					newendofliney = endofliney;
					caughtfish = false;
					fish->setSpeed(-2,0);
					catchtimer = 1000/fishingSkillLevel + catchrand;
				}
			}

			if(fishreeledin)
			{					
				fish->setSpeed(2,-1);
				score = score + 57;

			}

			fish->updatepos();

			int fx,fy;

			fish->getpos(&fx,&fy);
			
			if((fishreeledin) && (fx > 1024))
			{
					caughtfish = false;
					fishreeledin = false;
					cast = false;
					drawncast = false;
					fishcount++;
					fish->despawn();
			}
			else if(fx < 0)
			{
				fish->despawn();
			}

			if(fish->isAlive())
				fish->drawSprite(buffer,fishmg);

			
			if((endofliney < 330) && (endoflinex > 680) && (mouse_b & 1))
			{
				hookx = 950 - ((barx2 - 700) * 5);
				cast = false;
				drawncast = false;
				mtimer = 10;
			}

				
			}


			endoflinex = newendoflinex;
			endofliney = newendofliney;
			
			position_mouse_z(endoflinex * - 1);
			linelength = 950 - endoflinex;
			point1x = 950 - (linelength /4);
			spline(buffer,&points[8],colour_white);


		}

		

		
		school->drawSprite(buffer,fishmg);

		int schoolx, schooly;

		school->getpos(&schoolx,&schooly);

		int xspeed,yspeed;

		school->getspeed(&xspeed,&yspeed);

		if(schoolrate == 0)
		{
			school->updatepos();
			school->incrementAnimationframe();
			if(!fishreeledin)
			{
			fish->incrementAnimationframe();
			}
			schoolrate = 10;
		}

		if((schoolroam == 0) && (schoolycount > 0))
		{	
			if((upordown > 50) && (schooly < 540))
			{
			 school->setSpeed(xspeed,-1);
			}
			else if((upordown < 50) && (schooly > 240))
			{
			 school->setSpeed(xspeed,1);
			}

			schoolycount--;
		}
		
		
		if(schoolycount == 0)
		{
			schoolycount = 500;
			schoolroam = (rand()%300) + 300;
			upordown = rand()%100;
			school->setSpeed(xspeed,0);
		}

		if(schoolx <= 0)
		{
			school->setSpeed(1,0);
			school->setSpriteState(1);
		}
		else if (schoolx >= 820)
		{
			school->setSpeed(-1,0);
			school->setSpriteState(0);
		}
		else if(schooly <=280)
		{
			school->setSpeed(xspeed,1);
		}
		else if(schooly >= 450)
		{
			school->setSpeed(xspeed,-1);
		}


		if(schoolrate > 0)
			schoolrate--;

		if(schoolroam > 0)
			schoolroam--;

		//textprintf(buffer, font, 0,20, colour_white, "Mouse x: %i", mouse_x);
		//textprintf(buffer, font, 0, 30, colour_white, "Mouse y: %i", mouse_y);
		//textprintf(buffer, font, 0, 40, colour_white, "Mouse z: %i", mouse_z);
		//textprintf(buffer, font, 0, 50, colour_white, "Fish Caught: %i", fishcount);
		//textprintf(buffer, font, 0, 60, colour_white, "Ctimer: %i", catchtimer);
		//textprintf(buffer, font, 0, 70, colour_white, "Crand: %i", catchrand);
		//textprintf(buffer, font, 0, 80, colour_white, "line x: %i", endoflinex);
		//textprintf(buffer, font, 0, 90, colour_white, "line y: %i", endofliney);

		int scorearray[4];

		for(int i = 0; i<5;i++)
		{
			scorearray[i] = 0;
		}

		for(i=0; i<score;i++)
		{
			scorearray[4]++;
			
			if(scorearray[4] == 10)
			{
				scorearray[3]++;
				scorearray[4] = 0;
			}
			
			if(scorearray[3] == 10)
			{
				scorearray[2]++;
				scorearray[3] = 0;
			}
			
			if(scorearray[2] == 10)
			{
				scorearray[1]++;
				scorearray[2] = 0;
			}

			if(scorearray[1] == 10)
			{
				scorearray[0]++;
				scorearray[1] = 0;
			}

		}

		int xx = 0;

		for(i = 0; i < 5; i++)
		{
			masked_stretch_blit(numbers,buffer,(scorearray[i] * 25),0,25,25,xx,0,25,25);
			xx =xx + 27;
		}

		textprintf(buffer, font, 0, 730, colour_white, "press 'B' to return to game");

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

		if(key[KEY_B])
		{
			ingame = false;
		}

	}while(ingame);

	*exp = score / 10;
	*yourcatch = fishcount;


	delete fishmg;
	delete fish;
	delete school;

	destroy_bitmap (buffer);
	destroy_bitmap (slider);
	destroy_bitmap (slider2);
	destroy_bitmap (aimbar);
	destroy_bitmap (hook);
	destroy_bitmap (numbers);



	return 0;
}
