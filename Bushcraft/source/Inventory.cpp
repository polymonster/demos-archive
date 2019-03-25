#include <string>
#include <fstream>
#include "Inventory.h"
#include "Definitions.h"
#include <iostream>

using namespace std;



CInventory::CInventory()
{

	for(int i = 0; i<20;i++)
	{
		m_inventoryItem[i] = 0;
	}

	for(i = 0; i<20;i++)
	{
		m_baseItem[i] = 0;
	}

	for(i = 0; i<5;i++)
	{
		m_tools[i] = 0;
	}

	m_tools[4] = 1;
	m_nextEmpty = 0;
	m_mousetimer = 0;
	m_inventoryOpen = false;
	m_lightFire = false;
	m_selectedItem = -1;
	m_equippedItem = 0;
	m_spearAmmo = 0;
	m_woodCount = 0;
	m_woodSlotOpened = false;
	m_spaceAvail = true;
	m_returnAmmo = 0;
	m_healthIncrease = 0;
	m_hardSpearAmmo = 0;
	m_hasKindling = false;
	m_hasBone = false;
	m_hasSinew = false;
	m_hasSewingKit = false;
	m_hasCuredRabbitHide = false;
	m_hasRabbitHide = false;
	m_flaskWaterLevel = 0;
	m_hasWaterFlask = false;
	m_drinking = false;
	m_craftToolSelected = false;
	m_fireToolSelected = false;
	m_makeToolSelected = false;
	m_weaponToolSelected = false;
	m_inWaterMenu = false;
	m_equippedMoveItem = false;
	m_baseWoodCount = 0;

	m_moveitem = 0;

	m_nextEmptyBase = 0;

	mtx = 0;
	ftx = 0;
	ctx = 0;
	wtx = 0;

	inventoryStick = NULL;
	inventoryStone = NULL;
	inventorySharpStick = NULL;
	inventoryKindling = NULL;
	inventoryHandDrill = NULL;
	inventoryBg = NULL;
	meat=NULL;
	bone=NULL;
	sinew=NULL;
	cookedmeat=NULL;
	toolbg = NULL;
	make = NULL;
	weaponEmpty = NULL;
	weaponSpear = NULL;
	weaponHardenedSpear = NULL;
	hardenedSpear = NULL;
	sewingTab = NULL;
	makeTab = NULL;
	rabbit = NULL;
	rabbitMeat = NULL;
	rabbitHide = NULL;
	curedRabbitHide = NULL;
	leatherFlask = NULL;
	waterWindow = NULL;
	lootTable = NULL;
	flint = NULL;
	fishone = NULL;
	cookedfishone = NULL;
	cookedrabbitmeat = NULL;
	storeinventory = NULL;


	for(i = 0;i<100;i++)
	{
		foodHeal[i] = 0;
	}

	foodHeal[COOKED_MEAT] = 20;
	foodHeal[COOKED_RABBIT_MEAT] = 15;
	foodHeal[COOKED_FISH_ONE] = 10;


	colour_white = makecol(225,222,194);
	colour_grey = makecol(128,128,128);
	colour_yellow = makecol(225,222,0);
	colour_green = makecol(0,190,0);
	colour_dkred = makecol(190,0,0);
	colour_blue = makecol(0,0,190);
	
}

CInventory::~CInventory() 
{
	if(m_binitialised)
	{
		destroy_bitmap(inventoryStick);
		destroy_bitmap(inventoryStone);
		destroy_bitmap(inventorySharpStick);
		destroy_bitmap(inventoryKindling);
		destroy_bitmap(inventoryHandDrill);
		destroy_bitmap(inventoryBg);
		destroy_bitmap(meat);
		destroy_bitmap(bone);
		destroy_bitmap(sinew);
		destroy_bitmap(cookedmeat);
		destroy_bitmap(toolbg);
		destroy_bitmap(make);
		destroy_bitmap(weaponEmpty);
		destroy_bitmap(weaponSpear);
		destroy_bitmap(weaponHardenedSpear);
		destroy_bitmap(hardenedSpear);
		destroy_bitmap(makeTab);
		destroy_bitmap(sewingTab);
		destroy_bitmap(rabbit);
		destroy_bitmap(rabbitMeat);
		destroy_bitmap(rabbitHide);
		destroy_bitmap(curedRabbitHide);
		destroy_bitmap(leatherFlask);
		destroy_bitmap(waterWindow);
		destroy_bitmap(lootTable);
		destroy_bitmap(flint);
		destroy_bitmap(fishone);
		destroy_bitmap(cookedfishone);
		destroy_bitmap(cookedrabbitmeat);
	}

}

bool CInventory::Initialise()
{

	inventoryStick =load_bitmap("data/WoodBig.bmp",0);
	inventoryStone =load_bitmap("data/SharpStoneWeapButton.bmp",0);
	inventorySharpStick = load_bitmap("data/SharpStick.bmp",0);
	inventoryKindling = load_bitmap("data/Kindling.bmp",0);
	inventoryHandDrill = load_bitmap("data/HandDrillButton.bmp",0);
	inventoryBg = load_bitmap("data/InventoryPane.bmp",0);
	meat = load_bitmap("data/Meat.bmp",0);
	bone = load_bitmap("data/Bone.bmp",0);
	sinew = load_bitmap("data/Sinew.bmp",0);
	cookedmeat = load_bitmap("data/MeatCooked.bmp",0);
	toolbg = load_bitmap("data/InventPaneRight.bmp",0);
	make = load_bitmap("data/MakeButton.bmp",0);
	weaponSpear = load_bitmap("data/SharpStickWeapButton.bmp",0);
	weaponEmpty = load_bitmap("data/NoneWeaponButton.bmp",0);
	weaponHardenedSpear = load_bitmap("data/SharpStickHardenedWeapButton.bmp",0);
	hardenedSpear = load_bitmap("data/SharpStickHardened.bmp",0);
	makeTab = load_bitmap("data/InventPaneRightTabCraft.bmp",0);
	sewingTab = load_bitmap("data/InventPaneRightTabSewing.bmp",0);
	rabbit = load_bitmap("data/Rabbit.bmp",0);
	rabbitMeat = load_bitmap("data/RabbitMeat.bmp",0);
	rabbitHide = load_bitmap("data/RabbitHide.bmp",0);
	curedRabbitHide = load_bitmap("data/RabbitHideCured.bmp",0);
	leatherFlask = load_bitmap("data/Flask.bmp",0);
	waterWindow = load_bitmap("data/WaterMenu.bmp",0);
	lootTable =load_bitmap("data/LootWindow.bmp",0);
	flint = load_bitmap("data/Flint.bmp",0);
	fishone = load_bitmap("data/FishRaw.bmp",0);
	cookedfishone = load_bitmap("data/FishCooked.bmp",0);
	cookedrabbitmeat = load_bitmap("data/RabbitMeatCooked.bmp",0);
	storeinventory = load_bitmap("data/campGUI.bmp",0);

	m_toolSelected = false;

	return m_binitialised = true;

}

void CInventory::AddItem(int itemNumber)
{

	Sort();

	if((itemNumber == STICK) && (m_woodSlotOpened))
	{
		m_woodCount++;
	}
	else if(itemNumber == STONE)
	{
		m_tools[0] = STONE;
		itemNumber = 0;
	}

	if((m_spaceAvail) && (itemNumber != 0))
	{
		if((itemNumber == STICK) && (!m_woodSlotOpened))
		{
			m_woodSlotOpened = true;
			m_inventoryItem[m_nextEmpty] = itemNumber;
			m_woodCount++;
		}

		if (itemNumber!=STICK)
		{
			m_inventoryItem[m_nextEmpty] = itemNumber;
		}
	}
	
	itemNumber = 0;


}

void CInventory::InventModal(bool *inventoryopen, BITMAP *buffer, bool *startbuilding)
{
	int numberofrows=4;

	int inventoryitemxpos[20];
	int inventoryitemypos[4];

	m_inventoryOpen = true;

			
	inventoryitemxpos[0] = 0;
	inventoryitemypos[0] = 560;

	int xmax, xmin, ymax, ymin;

		ymax = 610;
		ymin = 560;

	int itemcount = 0;

			for(int i = 1; i < 5; i++)
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

		masked_stretch_blit(inventoryBg,buffer,0,0,415,304,0, 312,415,304);

		if((m_toolSelected) && ((mouse_x > 616) || (mouse_y < 310)) && (mouse_b&1))
		{
			m_inventoryOpen = false;
			closeMenus();
		}
		else if((((mouse_x > 416) && (mouse_y < 615)) || (mouse_y < 310)) && (mouse_b&1) && (!m_toolSelected))
		{
			m_inventoryOpen = false;
			closeMenus();
		}
			
			//FOR ITEMS---------------------------------------------------------------------------------------------------------
			for(int j=0;j < numberofrows; j++)
			{
			
				int endOfRow = current + 5;					
				
				
				for(i=current;i<endOfRow;i++)
				{
					int x = i + 1;
					int y = j + 1;
					
					if((m_inventoryItem[i] == STICK) && (m_woodCount > 0))
					{
						textprintf(buffer,font,inventoryitemxpos[i]+5,inventoryitemypos[j]+5, colour_white, "%i", m_woodCount);
						masked_stretch_blit(inventoryStick,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if((m_inventoryItem[i] == SHARP_STICK) && (m_spearAmmo > 0))
					{
						textprintf(buffer,font,inventoryitemxpos[i]+5,inventoryitemypos[j]+5, colour_white, "%i", m_spearAmmo);
						masked_stretch_blit(inventorySharpStick,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}
					else if((m_inventoryItem[i] == HARDENED_SPEAR) && (m_hardSpearAmmo > 0))
					{
						textprintf(buffer,font,inventoryitemxpos[i]+5,inventoryitemypos[j]+5, colour_white, "%i", m_hardSpearAmmo);
						masked_stretch_blit(hardenedSpear,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}
					else if(m_inventoryItem[i] == KINDLING)
					{
						masked_stretch_blit(inventoryKindling,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == MEAT)
					{
						masked_stretch_blit(meat,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == BONE)
					{
						masked_stretch_blit(bone,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == SINEW)
					{
						masked_stretch_blit(sinew,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == COOKED_MEAT)
					{
						masked_stretch_blit(cookedmeat,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == RABBIT)
					{
						masked_stretch_blit(rabbit,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == RABBIT_MEAT)
					{
						masked_stretch_blit(rabbitMeat,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == RABBIT_HIDE)
					{
						masked_stretch_blit(rabbitHide,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}
					else if(m_inventoryItem[i] == CURED_RABBIT_HIDE)
					{
						masked_stretch_blit(curedRabbitHide,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);
					}

					else if(m_inventoryItem[i] == LEATHER_FLASK)
					{
						masked_stretch_blit(leatherFlask,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

						int barystart = inventoryitemypos[j] + 45;
						rectfill(buffer,inventoryitemxpos[i] + 75,barystart,inventoryitemxpos[i] + 80,barystart - m_flaskWaterLevel, colour_blue);

					}
					else if (m_inventoryItem[i] == FLINT)
					{
						masked_stretch_blit(flint,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}
					else if (m_inventoryItem[i] == FISH_ONE)
					{
						masked_stretch_blit(fishone,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}
					else if (m_inventoryItem[i] == COOKED_FISH_ONE)
					{
						masked_stretch_blit(cookedfishone,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}
					else if (m_inventoryItem[i] == COOKED_RABBIT_MEAT)
					{
						masked_stretch_blit(cookedrabbitmeat,buffer,0,0,82,50,inventoryitemxpos[i], inventoryitemypos[j], 82, 50);

					}

					current++;

					}

				}


				//FOR TOOLS----------------------------------------------------------------------------------------------------------------
				int Toolsy,Toolsx;
		
				Toolsy = 330;

				//m_tools[0] = STONE;

				//CRAFTING TOOLS-----------------------------------------------------------------------------------------------------------------
				if(m_craftToolSelected)
				{
					ctx = 82;
				}
				else
				{
					ctx = 0;
				}

				
				if(m_tools[0] == STONE)
				{
					Toolsx = 0;

					if((mouse_x > 0) && (mouse_x < 82) && (mouse_y < 400) && (mouse_y > 330))
					{
						ctx = 82;

						if((mouse_b & 1) && (m_mousetimer == 0) && (!m_craftToolSelected))
						{
							m_toolSelected = true;
							m_craftToolSelected = true;
							m_fireToolSelected = false;
							m_makeToolSelected = false;
							m_weaponToolSelected = false;
							m_mousetimer = 5;
						}
						else if((mouse_b & 1) && (m_mousetimer == 0) && (m_craftToolSelected))
						{
							m_toolSelected = false;
							m_craftToolSelected = false;
							m_mousetimer = 5;
						}

					}
					masked_stretch_blit(inventoryStone,buffer,ctx,0,82,50,Toolsx,Toolsy, 82, 50);
				}

				//FIRE TOOLS-------------------------------------------------------------------------------------------------------------------
				if(m_fireToolSelected)
				{
					ftx = 82;
				}
				else
				{
					ftx = 0;
				}

				if(m_tools[1] == HAND_DRILL)
				{
					Toolsx = 83;

					if((mouse_x > Toolsx) && (mouse_x < Toolsx + 82) && (mouse_y < 400) && (mouse_y > 330))
					{
						ftx = 82;

						if((mouse_b & 1) && (m_mousetimer == 0) && (!m_fireToolSelected))
						{
							m_toolSelected = true;
							m_fireToolSelected = true;
							m_craftToolSelected = false;
							m_makeToolSelected = false;
							m_weaponToolSelected = false;
							m_mousetimer = 5;
							ftx = 82;
						}
						else if((mouse_b & 1) && (m_mousetimer == 0) && (m_fireToolSelected))
						{
							m_toolSelected = false;
							m_fireToolSelected = false;
							m_mousetimer = 5;
							ftx = 0;
						}

					}

					masked_stretch_blit(inventoryHandDrill,buffer,ftx,0,82,50,Toolsx,Toolsy, 82, 50);
				}

				//WEAPONS--------------------------------------------------------------------------------------------------------------------
				if(m_weaponToolSelected)
				{
					wtx = 82;
				}
				else
				{
					wtx = 0;
				}


				if(m_tools[3] >= 0)
				{
					Toolsx = 247;

					if((mouse_x > Toolsx) && (mouse_x < Toolsx + 82) && (mouse_y < 400) && (mouse_y > 330))
					{
						wtx = 82;

						if((mouse_b & 1) && (m_mousetimer == 0) && (!m_weaponToolSelected))
						{
							m_toolSelected = true;
							m_fireToolSelected = false;
							m_craftToolSelected = false;
							m_makeToolSelected = false;
							m_weaponToolSelected = true;
							m_mousetimer = 5;
							wtx = 82;
						}
						else if((mouse_b & 1) && (m_mousetimer == 0) && (m_weaponToolSelected))
						{
							m_toolSelected = false;
							m_weaponToolSelected = false;
							m_mousetimer = 5;
							wtx = 0;
						}

					}

					if(m_tools[3] == SHARP_STICK)
					{
					masked_stretch_blit(weaponSpear,buffer,wtx,0,82,50,Toolsx,Toolsy, 82, 50);
					}
					else if (m_tools[3] == HARDENED_SPEAR)
					{
					masked_stretch_blit(weaponHardenedSpear,buffer,wtx,0,82,50,Toolsx,Toolsy, 82, 50);
					}
				}
				
				
				
				// MAKE

				if(m_makeToolSelected)
				{
					mtx = 82;
				}
				else
				{
					mtx = 0;
				}

				if(m_tools[4] == 1)
				{
					Toolsx = 329;

					if((mouse_x > Toolsx) && (mouse_x < Toolsx + 82) && (mouse_y < 400) && (mouse_y > 330))
					{
						rectfill(buffer,Toolsx,Toolsy,Toolsx + 81,Toolsy + 49, colour_dkred);
						mtx = 82;
						
						if((mouse_b & 1) && (m_mousetimer == 0) && (!m_makeToolSelected))
						{
							m_toolSelected = true;
							m_fireToolSelected = false;
							m_craftToolSelected = false;
							m_makeToolSelected = true;
							m_weaponToolSelected = false;
							m_mousetimer = 5;

						}
						else if((mouse_b & 1) && (m_mousetimer == 0) && (m_makeToolSelected))
						{
							m_toolSelected = false;
							m_makeToolSelected = false;
							m_mousetimer = 5;
						}

					}

						masked_stretch_blit(make,buffer,mtx,0,82,50,Toolsx,Toolsy, 82, 50);
				}


				

				//TOOLS MENU

				if(m_toolSelected)
				{	

					
					masked_stretch_blit(toolbg,buffer,0,0,200,304,415,312,200,304);
					
					int ymin, ymax;

					ymin = 420;
					ymax = 429;

					selectedOption = -1;
					
					for(i = 0; i < 15;i++)
					{
						if((mouse_x > 415) && (mouse_x < 615) && (mouse_y < ymax) && (mouse_y > ymin))
						{
							selectedOption = i + 1;
						}

						ymin = ymin + 10;
						ymax = ymax+ 10;
					}


					if(m_makeToolSelected)
					{
					MakeTool(buffer,&*startbuilding);
					}
					else if(m_craftToolSelected)
					{
					CraftTool(buffer);
					}
					else if(m_fireToolSelected)
					{
					FireTool(buffer);
					}
					else if(m_weaponToolSelected)
					{
					WeaponTool(buffer);
					}
					

				
				}

				m_selectedItem = -1;

				for(i=0; i < 4; i++)
				{
					xmax = 82;
					xmin = 0;

					
					for(int j = 0; j < 5; j++)
					{
				
						if((mouse_x < xmax) && (mouse_x > xmin) && (mouse_y < ymax) && (mouse_y > ymin)) 
						{
						
							m_selectedItem = itemcount;

							if((mouse_b & 2) && (m_mousetimer == 0))
							{
								m_mousetimer = 10;
								if((m_inventoryItem[m_selectedItem] != STICK) && (m_inventoryItem[m_selectedItem] != SHARP_STICK) && (m_inventoryItem[m_selectedItem] != HARDENED_SPEAR)) // if not a stacking item...
								{
									m_inventoryItem[m_selectedItem] = 0;
								}
								else if(m_inventoryItem[m_selectedItem] == STICK)
								{
									m_woodCount--;
								}
								else if(m_inventoryItem[m_selectedItem] == SHARP_STICK)
								{
									m_spearAmmo--;
								}
								else if(m_inventoryItem[m_selectedItem] == HARDENED_SPEAR)
								{
									m_hardSpearAmmo--;
								}

								
								//SPAWN ITEMS TO WORLD
							}
							else if((mouse_b & 1) && (m_mousetimer == 0) && (!m_equippedMoveItem)) // not holding a dragging item
							{
								m_equippedItem = m_inventoryItem[m_selectedItem];
								m_mousetimer = 10;
								m_justUsed = m_selectedItem;

							}

						}

						xmax = xmax + 82;
						xmin = xmin + 82;

						if(j != 4)
						{
						itemcount ++;
						}

					}

						ymax = ymax - 50;
						ymin = ymax - 50;
						
						itemcount ++;
				}

				if(key[KEY_M])
				{
					m_inventoryItem[m_nextEmpty] = COOKED_MEAT;
				}

				m_inventoryItem[-1] = 0;

				//MOUSE OVERS SELECTED ITEMS

				m_drinking = false;
				

				if(m_inventoryItem[m_selectedItem] == STICK)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Stick");
				}
				else if(m_inventoryItem[m_selectedItem] == SHARP_STICK)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Spear");
				}
				else if(m_inventoryItem[m_selectedItem] == KINDLING)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Kindling");
				}
				else if(m_inventoryItem[m_selectedItem] == MEAT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Gazelle Meat");
				}
				else if(m_inventoryItem[m_selectedItem] == BONE)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Bone");
				}
				else if(m_inventoryItem[m_selectedItem] == SINEW)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Sinew");
				}
				else if(m_inventoryItem[m_selectedItem] == COOKED_MEAT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Cooked Gazelle Meat");
				}
				else if(m_inventoryItem[m_selectedItem] == HARDENED_SPEAR)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Hardened Spear");
				}
				else if(m_inventoryItem[m_selectedItem] == RABBIT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Rabbit");
				}
				else if(m_inventoryItem[m_selectedItem] == RABBIT_MEAT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Rabbit Meat");
				}
				else if(m_inventoryItem[m_selectedItem] == RABBIT_HIDE)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Rabbit Hide");
				}
				else if(m_inventoryItem[m_selectedItem] == CURED_RABBIT_HIDE)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Cured Rabbit Hide");
				}
				else if(m_inventoryItem[m_selectedItem] == LEATHER_FLASK)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Leather Flask");
					if(mouse_b & 1)
					{
						UseFlask(0);
					}
				}
				else if(m_inventoryItem[m_selectedItem] == COOKED_RABBIT_MEAT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Cooked Rabbit Meat");
				}
				else if(m_inventoryItem[m_selectedItem] == COOKED_FISH_ONE)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Cooked Fish");
				}
				else if(m_inventoryItem[m_selectedItem] == FISH_ONE)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Fish");
				}
				else if(m_inventoryItem[m_selectedItem] == FLINT)
				{
					textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Flint");
				}


				if(m_mousetimer > 0)
					m_mousetimer --;

				textprintf(buffer, font,0,0, colour_yellow,"%i",foodHeal[COOKED_MEAT]);

				Sort();

				*inventoryopen = m_inventoryOpen;

}

void CInventory::InventDespawn(int quantity, int item)
{
	for(int i = 0; i < 20; i++)
	{
		if((m_inventoryItem[i] == item) && (quantity > 0))
		{
			m_inventoryItem[i] = 0;
			quantity--;
		}

	}

}

void CInventory::Sort()
{
	m_hasKindling = false;
	m_hasBone = false;
	m_hasSinew = false;
	m_hasRabbit = false;
	m_hasCuredRabbitHide = false;
	m_hasRabbitHide = false;

	int emptyslot = false;
	

	if(m_inventoryItem[19] > 0)
	{
		m_spaceAvail = false;
	}
	else
	{
		m_spaceAvail = true;
	}
	
	
	for(int i = 19; i >=0; i--)
	{	
		int j;

		if(i < 19)
		{
			j=i+1;
		}
		else
		{
			j = 19;
		}
		
		if((m_inventoryItem[j]>0) && (m_inventoryItem[i] == 0))
		{
			m_inventoryItem[i] = m_inventoryItem[j];
			m_inventoryItem[j] = 0;
		}
		else if((m_inventoryItem[i] == SHARP_STICK)&& (m_spearAmmo == 0))
		{
			m_inventoryItem[i] = 0;
		}
		else if((m_inventoryItem[i] == STICK) && (m_woodCount == 0))
		{
			m_inventoryItem[i] = EMPTY;
			m_woodSlotOpened = false;
		}
		else if((m_inventoryItem[i] == HARDENED_SPEAR)&& (m_hardSpearAmmo == 0))
		{
			m_inventoryItem[i] = 0;
		}

	}

	int stickcnt = 0;

	for(i = 0; i<21;i++)
	{

		if(m_inventoryItem[i] == KINDLING)
		{
			m_hasKindling = true;
		}
		else if(m_inventoryItem[i] == BONE)
		{
			m_hasBone = true;
		}
		else if(m_inventoryItem[i] == SINEW)
		{
			m_hasSinew = true;
		}
		else if(m_inventoryItem[i] == RABBIT)
		{
			m_hasRabbit = true;
		}
		else if(m_inventoryItem[i] == RABBIT_HIDE)
		{
			m_hasRabbitHide = true;
		}
		else if(m_inventoryItem[i] == CURED_RABBIT_HIDE)
		{
			m_hasCuredRabbitHide = true;
		}
		else if(m_inventoryItem[i] == LEATHER_FLASK)
		{
			m_hasWaterFlask = true;
		}
		else if((m_inventoryItem[i] == 0) && (i < 20))
		{
			m_nextEmpty = i;
			i = 21;
		}

		if(m_inventoryItem[i] == STICK)
		{
			stickcnt++;

			if(stickcnt > 1)
			{
				m_inventoryItem[i] = 0;
			}
		}


	}

}

void CInventory::setfirebool()
{
	m_lightFire = false;
}

void CInventory::equippedItem(BITMAP *buffer)
{

	if((m_equippedItem == SHARP_STICK) || (m_equippedItem == HARDENED_SPEAR)) // OR ANY WEAPON
	{
		m_weapon = m_equippedItem;
	}

	if(m_equippedItem == RABBIT_HIDE)
	{
		m_inventoryItem[m_justUsed] = EMPTY;

		m_equippedMoveItem = true;

		m_justUsed = -1;

		set_clip(buffer,0,0,1024,613);

		masked_stretch_blit(rabbitHide,buffer,0,0,82,50,mouse_x,mouse_y,41,25);

		set_clip(buffer,0,0,0,0);

		if((mouse_x > 0) && (mouse_x < 415) && (mouse_y > 312) && (mouse_y < 616))
		{
			if((mouse_b) && (m_mousetimer == 0))
			{
				m_inventoryItem[m_nextEmpty] = RABBIT_HIDE;
				m_equippedMoveItem = false;
				m_equippedItem = 0;
				m_mousetimer = 10;
			}
		}
	}
	else if(m_equippedItem == MEAT)
	{
		m_inventoryItem[m_justUsed] = EMPTY;

		m_equippedMoveItem = true;

		m_justUsed = -1;

		set_clip(buffer,0,0,1024,613);

		masked_stretch_blit(meat,buffer,0,0,82,50,mouse_x,mouse_y,41,25);

		set_clip(buffer,0,0,0,0);

		if((mouse_x > 0) && (mouse_x < 410) && (mouse_y > 312) && (mouse_b) && (m_mousetimer == 0))
		{
			m_inventoryItem[m_nextEmpty] = MEAT;
			m_equippedMoveItem = false;
			m_equippedItem = 0;
			m_mousetimer = 10;
		}
	}
	else if(m_equippedItem == RABBIT_MEAT)
	{
		m_inventoryItem[m_justUsed] = EMPTY;

		m_equippedMoveItem = true;

		m_justUsed = -1;

		set_clip(buffer,0,0,1024,613);

		masked_stretch_blit(rabbitMeat,buffer,0,0,82,50,mouse_x,mouse_y,41,25);

		set_clip(buffer,0,0,0,0);

		if((mouse_x > 0) && (mouse_x < 410) && (mouse_y > 312) && (mouse_b) && (m_mousetimer == 0))
		{
			m_inventoryItem[m_nextEmpty] = RABBIT_MEAT;
			m_equippedMoveItem = false;
			m_equippedItem = 0;
			m_mousetimer = 10;
		}
	}
	else if(m_equippedItem == FISH_ONE)
	{
		m_inventoryItem[m_justUsed] = EMPTY;

		m_equippedMoveItem = true;

		m_justUsed = -1;

		set_clip(buffer,0,0,1024,613);

		masked_stretch_blit(fishone,buffer,0,0,82,50,mouse_x,mouse_y,41,25);

		set_clip(buffer,0,0,0,0);

		if((mouse_x > 0) && (mouse_x < 410) && (mouse_y > 312) && (mouse_b) && (m_mousetimer == 0))
		{
			m_inventoryItem[m_nextEmpty] = FISH_ONE;
			m_equippedMoveItem = false;
			m_equippedItem = 0;
			m_mousetimer = 10;
		}
	}
	else if((foodHeal[m_equippedItem] > 0) && (m_equippedItem > 0))
	{
		m_healthIncrease = foodHeal[m_equippedItem];
		InventDespawn(1,m_equippedItem);
		m_equippedItem = 0;
	}
	
	// WEAPONS
	
	if((m_weapon == SHARP_STICK) && (m_spearAmmo > 0))
	{
		masked_stretch_blit(inventorySharpStick,buffer,0,0,82,50,295,650, 82, 50);
		textprintf_centre_ex(buffer, font,333,728, colour_yellow,-1, "Spear");
		textprintf(buffer, font,395,659, colour_yellow, "%i", m_spearAmmo);
		m_tools[3] = SHARP_STICK;

	}
	else if ((m_weapon == HARDENED_SPEAR) && (m_hardSpearAmmo > 0))
	{
		masked_stretch_blit(hardenedSpear,buffer,0,0,82,50,295,650, 82, 50);
		textprintf_centre_ex(buffer, font,333,718, colour_yellow,-1, "Hardened");
		textprintf_centre_ex(buffer, font,333,728, colour_yellow,-1, "Spear");
		textprintf(buffer, font,395,659, colour_yellow, "%i", m_hardSpearAmmo);
		m_tools[3] = HARDENED_SPEAR;

	}
	else
	{
		m_weapon = 0;
		m_tools[3] = EMPTY;
	}


}

void CInventory::itemUsed()
{
	m_item = 0;
	m_equippedItem = 0;
	m_equippedMoveItem = false;

}

void CInventory::closeMenus()
{
	m_toolSelected = false;
	ftx = 0;
	wtx = 0;
	mtx = 0;
	ctx = 0;
	m_fireToolSelected = false;
	m_craftToolSelected = false;
	m_makeToolSelected = false;
	m_weaponToolSelected = false;
	m_inventoryOpen = false;
}

void CInventory::useAmmo(int weapon)
{
	if(weapon == SHARP_STICK)
	{
		m_spearAmmo --;
	}
	else if(weapon == HARDENED_SPEAR)
	{
		m_hardSpearAmmo --;
	}

}

void CInventory::getAmmo(int weapon, int *m_returnAmmo)
{
	if(weapon == SHARP_STICK)
	{
		*m_returnAmmo = m_spearAmmo;
	}
	else if(weapon == HARDENED_SPEAR)
	{
		*m_returnAmmo = m_hardSpearAmmo;
	}

}
void CInventory::healthIncreased()
{
	m_healthIncrease = 0;
}

void CInventory::MakeTool(BITMAP *buffer, bool *startbuilding)
{
	
	masked_stretch_blit(makeTab,buffer,0,0,30,41,585,330, 30, 41);

	if(m_hasSewingKit)
		masked_stretch_blit(sewingTab,buffer,0,0,30,41,585,371, 30, 41);

	if((mouse_x >585) && (mouse_x <615) && (mouse_y > 330) && (mouse_y < 380) && (mouse_b & 1))
	{
		m_making = true;
		m_sewing = false;
	}
	else if((mouse_x >585) && (mouse_x <615) && (mouse_y > 371) && (mouse_y < 412) && (mouse_b & 1) && (m_hasSewingKit))
	{
		m_making = false;
		m_sewing = true;
	}


	if(m_making)
	{
	textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Construct");
	textprintf_centre_ex(buffer, font,515, 420, colour_white,-1, "Camp");
	textprintf_centre_ex(buffer, font,515, 430, colour_grey,-1, "Raft");


	if(selectedOption == 1)
	{
			textprintf_centre_ex(buffer, font,515, 420, colour_yellow,-1, "Camp");
			
			if(mouse_b & 1)
			{
				m_buildcamp = true;	
				*startbuilding = true;
				closeMenus();
			}
	}

	}
	else if((m_sewing) && (m_hasSewingKit))
	{
		textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Sewing ");
		textprintf_centre_ex(buffer, font,515, 420, colour_grey,-1, "Water Flask");
		
	if((m_hasCuredRabbitHide) && (!m_hasWaterFlask))
	{
		textprintf_centre_ex(buffer, font,515, 420, colour_white,-1, "Water Flask");

		if(selectedOption == 1)
		{
			textprintf_centre_ex(buffer, font,515, 420, colour_yellow,-1, "Water Flask");
			
			if(mouse_b & 1)
			{
				InventDespawn(1,CURED_RABBIT_HIDE);
				AddItem(LEATHER_FLASK);
			}

		}

	}
	}


}

void CInventory::CraftTool(BITMAP *buffer)
{
	if(m_craftToolSelected)
					{
						textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Crafting");
						textprintf_centre_ex(buffer, font,515, 420, colour_grey,-1, "Spear / Sharpened Stick");
						textprintf_centre_ex(buffer, font,515, 420, colour_grey,-1, "Spear / Sharpened Stick");
						textprintf_centre_ex(buffer, font,515, 430, colour_grey,-1, "Fire Kindling");
						textprintf_centre_ex(buffer, font,515, 440, colour_grey,-1, "Hand Drill");
						textprintf_centre_ex(buffer, font,515, 450, colour_grey,-1, "Needle and Thread Kit");
						textprintf_centre_ex(buffer, font,515, 460, colour_grey,-1, "Butcher Rabbit");


						if(m_woodCount > 0)
						{
							if(m_spearAmmo < 5)
							{
								textprintf_centre_ex(buffer, font,515, 420, colour_white,-1, "Spear / Sharpened Stick");
							}

							textprintf_centre_ex(buffer, font,515, 430, colour_white,-1, "Fire Kindling");

							if((selectedOption == 1) && ((m_spaceAvail) || (m_spearAmmo > 0))) // MAKE SPEAR
							{
							textprintf_centre_ex(buffer, font,515, 420, colour_yellow,-1, "Spear / Sharpened Stick");

								if((mouse_b & 1) && (m_mousetimer == 0))
								{if(m_spearAmmo == 0)
									
									{
									m_inventoryItem[m_nextEmpty] = SHARP_STICK;
									}

									m_spearAmmo++;
									m_woodCount--;
									m_mousetimer = 5;
								}
							}
							else if((selectedOption == 2) && (m_spaceAvail))
							{
							textprintf_centre_ex(buffer, font,515, 430, colour_yellow,-1, "Fire Kindling");
								if((mouse_b & 1) && (m_mousetimer == 0))
								{
									m_inventoryItem[m_nextEmpty] = KINDLING;
									m_woodCount--;
									m_mousetimer = 5;
								}
							}


						}

						
						if((m_spearAmmo > 0) && (m_woodCount > 0)  && (m_tools[1] != HAND_DRILL))
						{
							textprintf_centre_ex(buffer, font,515, 440, colour_white,-1, "Hand Drill");

							if(selectedOption == 3)
							{
							textprintf_centre_ex(buffer, font,515, 440, colour_yellow,-1, "Hand Drill");
						
								if((mouse_b & 1) && (m_mousetimer == 0))
								{
									m_tools[1] = HAND_DRILL;
									m_woodCount--;
									m_spearAmmo--;
									m_mousetimer = 5;
								}
							}

						}
						else if (m_tools[1] == HAND_DRILL)
						{
							textprintf_centre_ex(buffer, font,515, 440, colour_green,-1, "Hand Drill");
						}

						if((m_hasBone) &&(m_hasSinew) && (!m_hasSewingKit))
						{
							textprintf_centre_ex(buffer, font,515, 450, colour_white,-1, "Needle and Thread Kit");
							
							if(selectedOption == 4)
							{
							textprintf_centre_ex(buffer, font,515, 450, colour_yellow,-1, "Needle and Thread Kit");
						
								if((mouse_b & 1) && (m_mousetimer == 0))
								{
									m_tools[2] = SEWING_KIT;
									m_hasSewingKit = true;
									InventDespawn(1,BONE);
									InventDespawn(1,SINEW);
									m_mousetimer = 5;
								}
							}

						}
						else if(m_hasSewingKit)
						{
							textprintf_centre_ex(buffer, font,515, 450, colour_green,-1, "Needle and Thread Kit");
						}
						
						if(m_hasRabbit)
						{
							textprintf_centre_ex(buffer, font,515, 460, colour_white,-1, "Butcher Rabbit");
							if(selectedOption == 5)
							{
							textprintf_centre_ex(buffer, font,515, 460, colour_yellow,-1, "Butcher Rabbit");
						
								if((mouse_b & 1) && (m_mousetimer == 0))
								{

									AddItem(RABBIT_MEAT);
									m_nextEmpty++;
									AddItem(RABBIT_HIDE);
									m_nextEmpty++;
									InventDespawn(1,RABBIT);
									Sort();
									m_mousetimer = 5;
								}
							}
							
						}

					}

}

void CInventory::FireTool(BITMAP *buffer)
{
		if(m_fireToolSelected)
		{
			textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Fire Making");

			if((m_woodCount >= 4) && (m_hasKindling))
				{
							textprintf_centre_ex(buffer, font,515, 420, colour_white,-1, "Make Fire");

							if(selectedOption == 1)
							{
							textprintf_centre_ex(buffer, font,515, 420, colour_yellow,-1, "Make Fire");

								if((mouse_b & 1) && (m_mousetimer == 0))
								{
									m_woodCount = m_woodCount - 4;
									InventDespawn(1,KINDLING);
									m_mousetimer = 5;
									m_lightFire = true;
								}
							}
						}
						else
						{
							textprintf_centre_ex(buffer, font,515, 420, colour_grey,-1, "Make Fire");
						}

		}
}

void CInventory::WeaponTool(BITMAP *buffer)
{
	
	if(m_tools[3] == SHARP_STICK)
	{
		textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Upgrade Spear");
		textprintf_centre_ex(buffer, font,515, 420, colour_grey,-1, "Poison Tip");
		textprintf_centre_ex(buffer, font,515, 430, colour_grey,-1, "Harden Tip");


		if(selectedOption == 1)
		{
			textprintf_centre_ex(buffer, font,515, 420, colour_yellow,-1, "Poison Tip");


		}
		else if(selectedOption == 2)
		{
			textprintf_centre_ex(buffer, font,515, 430, colour_yellow,-1, "Harden Tip");
			
			
			if((m_hardSpearAmmo == 0) && (mouse_b & 1) && (m_mousetimer == 0))
			{
				m_spearAmmo --;
				m_inventoryItem[m_nextEmpty] = HARDENED_SPEAR;
				m_hardSpearAmmo++;
				m_mousetimer = 5;

			}
			else if((m_hardSpearAmmo > 0) && (mouse_b & 1) && (m_mousetimer == 0))
			{
				m_spearAmmo --;
				m_hardSpearAmmo++;
				m_mousetimer = 5;
			}


		}
	}
	else if(m_tools[3] == HARDENED_SPEAR)
	{
		textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "Upgrade Hard Spear");
	}
	else
	{
		textprintf_centre_ex(buffer, font,515, 400, colour_grey,-1, "No Weapon Equipped");
	}

}

void CInventory::UseFlask(int mode)
{
	
	if((mode == 0) && (m_flaskWaterLevel > 0))
	{
		m_flaskWaterLevel --;
		m_drinking = true;
	}
	else if((mode == 1) && (m_flaskWaterLevel < 40))
	{
		m_flaskWaterLevel ++;
	}

}

void CInventory::WaterMenu(BITMAP *buffer, int xpos, int ypos,int *option)
{
	int waterOption = -1;

	m_inWaterMenu = true;

	masked_stretch_blit(waterWindow,buffer,0,0,184,128,xpos,ypos, 184, 128);

	int ymin, ymax;

	ymin = ypos + 50;
	ymax = ymin + 10;


	for(int i = 0; i < 5; i++)
	{
		if((mouse_x < xpos + 118) && (mouse_x > xpos + 10) && (mouse_y < ymax) && (mouse_y > ymin))
		{
			waterOption =i;
		}

		ymin = ymax;
		ymax = ymax + 10;
	}

	textprintf_centre_ex(buffer, font,xpos + 92,ypos + 50, colour_grey,-1, "Fill Flask");
	textprintf_centre_ex(buffer, font,xpos + 92,ypos + 60, colour_grey,-1, "Fish");
	textprintf_centre_ex(buffer, font,xpos + 92,ypos + 70, colour_grey,-1, "Set Trap");

	if((m_flaskWaterLevel < 40) && (m_hasWaterFlask))
	{	
		textprintf_centre_ex(buffer, font,xpos + 92,ypos + 50, colour_white,-1, "Fill Flask");

		if(waterOption == 0)
		{
			textprintf_centre_ex(buffer, font,xpos + 92,ypos + 50, colour_yellow,-1, "Fill Flask");

			if(mouse_b & 1)
			{
				m_flaskWaterLevel = 40;
			}
		}
	}
	
	if(waterOption == 1)
	{
		textprintf_centre_ex(buffer, font,xpos + 92,ypos + 60, colour_yellow,-1, "Fish");

			if(mouse_b & 1)
			{
				*option = waterOption;
			}

	}



	if((mouse_b & 1) && (mouse_x < xpos + 13) && (mouse_x > xpos) && (mouse_y < ypos + 13) && (mouse_y > ypos))
	{
		m_inWaterMenu = false;
		m_returnTimer = 10;
	}


}

void CInventory::timerCount()
{
	if(m_returnTimer > 0)
		m_returnTimer --;
}

void CInventory::Load(const char datafile[])
{

	ifstream invFile;
	ifstream mainfile;

	char buf[1024];
	char file[1024];

	mainfile.open(datafile);

	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');

	invFile.open(file);

	if (!invFile.is_open()) {
		allegro_message("Unable to open inventory file");
	}

	for(int i = 0; i < 20; i++)
	{
		invFile.getline(buf,1024,'\n');
		m_inventoryItem[i] = atoi(buf);
	}

	for(i = 0; i < 5; i++)
	{
		invFile.getline(buf,1024,'\n');
		m_tools[i] = atoi(buf);
	}

	invFile.getline(buf,1024,'\n');

	m_woodSlotOpened = atoi(buf);

	invFile.getline(buf,1024,'\n');

	m_woodCount = atoi(buf);

	invFile.getline(buf,1024,'\n');

	m_spearAmmo = atoi(buf);

	invFile.getline(buf,1024,'\n');

	m_hardSpearAmmo = atoi(buf);

	invFile.getline(buf,1024,'\n');

	m_hasSewingKit = atoi(buf);

	invFile.getline(buf,1024,'\n');

	m_flaskWaterLevel = atoi(buf);

	invFile.close();
	mainfile.close();

}

void CInventory::Save(const char datafile[])
{

	ifstream mainfile;

	char file[1024];

	mainfile.open(datafile);

	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');

	ofstream savInv;
	
	savInv.open(file, ios::out);


	for(int i = 0; i < 20; i++)
	{
		savInv << m_inventoryItem[i] << "\n";
	}
	
	for(i = 0; i < 5; i++)
	{
		savInv << m_tools[i] << "\n";
	}
	
	savInv << m_woodSlotOpened << "\n";
	savInv << m_woodCount << "\n";
	savInv << m_spearAmmo << "\n";
	savInv << m_hardSpearAmmo << "\n";
	savInv << m_hasSewingKit << "\n";
	savInv << m_flaskWaterLevel << "\n";


	savInv.close();

}

void CInventory::LootNode(int loot1,int loot2,int loot3,int loot4,int loot5,int loot6,BITMAP *buffer,int boxx, int boxy)
{

	m_lootitem[0] = loot1;
	m_lootitem[1] = loot2;
	m_lootitem[2] = loot3;
	m_lootitem[3] = loot4;
	m_lootitem[4] = loot5;
	m_lootitem[5] = loot6;

	
	int lootx,looty;

	m_looting = true;
	
	if(boxx > 512)
	{
		boxx=boxx-175;
	}
	else
	{
		boxx=boxx+15;
	}

	if(boxy > 384)
	{
		boxy=boxy-200;
	}
	else
	{
		boxy=boxy+15;
	}
	
	masked_stretch_blit(lootTable,buffer,0,0,184,241,boxx,boxy, 184, 241);

	lootx= boxx + 15;
	looty= boxy + 40;

	int cnt = 0;

	for(int i = 0; i < 3;i++)
	{
			for(int i = 0; i < 2;i++)
			{
				if(m_lootitem[cnt] == 1)
				{
					masked_stretch_blit(inventoryStick,buffer,0,0,82,50,lootx,looty,75, 50);
				}
				else if(m_lootitem[cnt] == MEAT)
				{
					masked_stretch_blit(meat,buffer,0,0,82,50,lootx,looty,75, 50);
				}
				else if(m_lootitem[cnt] == BONE)
				{
					masked_stretch_blit(bone,buffer,0,0,82,50,lootx,looty,75, 50);
				}
				else if(m_lootitem[cnt] == SINEW)
				{
					masked_stretch_blit(sinew,buffer,0,0,82,50,lootx,looty,75, 50);
				}
				else if(m_lootitem[cnt] == FLINT)
				{
					masked_stretch_blit(flint,buffer,0,0,82,50,lootx,looty,75, 50);
				}
				else if(m_lootitem[cnt] == RABBIT)
				{
					masked_stretch_blit(rabbit,buffer,0,0,82,50,lootx,looty,75, 50);
				}


				if((mouse_b & 1) && (mouse_x < lootx + 75) && (mouse_x > lootx) && (mouse_y < looty + 50) && (mouse_y > looty))
				{
					if((m_woodCount == 10) && (m_lootitem[cnt] == 1))
					{
						textprintf_centre_ex(buffer,font,boxx + 92,boxy + 210, colour_white,-1, "You Can't Carry");
						textprintf_centre_ex(buffer,font,boxx + 92,boxy + 220, colour_white,-1, "Anymore Wood");
					}
					else if((!m_spaceAvail) && (m_lootitem[cnt] != 1))
					{
						textprintf_centre_ex(buffer,font,boxx + 92,boxy + 220, colour_white,-1, "Inventory Full");
					}
					else
					{	
						AddItem(m_lootitem[cnt]);
						m_lootitem[cnt] = 0;
					}

				}


	
				lootx = lootx + 76;
				cnt++;
			}
		
		looty = looty + 51;
		lootx = lootx - 76;
		lootx = lootx - 76;
	}

		
	
	if((mouse_b & 1) && (mouse_x < boxx + 13) && (mouse_x > boxx) && (mouse_y < boxy + 13) && (mouse_y > boxy))
	{
		m_looting = false;
		m_returnTimer = 10;
	}


}

void CInventory::updateLoot(int *loot1,int *loot2,int *loot3,int *loot4,int *loot5,int *loot6)
{
	*loot1 = m_lootitem[0];
	*loot2 = m_lootitem[1];
	*loot3 = m_lootitem[2];
	*loot4 = m_lootitem[3];
	*loot5 = m_lootitem[4];
	*loot6 = m_lootitem[5];
	m_returnLoot = 0;

}

void CInventory::storeInventory(BITMAP *buffer, bool storeopen, bool *close, bool *save, bool *sleep, bool *buildbase)
{
		
		int tablex, tabley;
		int rows, columns;

		rows = 4;
		columns = 5; 

		tablex = 200;
		tabley = 200;

		int bagx = 218;
		int bagy = 244;
		int itemnumber = 0;

		*save = false;
		*sleep = false;
		*buildbase = false;

		if((mouse_x > tablex) && (mouse_x < tablex+ 13) && (mouse_y > tabley) && (mouse_y < tabley + 13) && (mouse_b & 1))
		{	
			*close = false;
		}
		
		masked_stretch_blit(storeinventory,buffer,0,0,653,192,tablex,tabley,653,192);
	
			for(int j=0;j <rows; j++)
			{				
				for(int i=0;i< columns;i++)
				{	
					if((m_inventoryItem[itemnumber] == STICK) && (m_woodCount > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17, colour_white, "%i", m_woodCount);
						masked_stretch_blit(inventoryStick,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if((m_inventoryItem[itemnumber] == SHARP_STICK) && (m_spearAmmo > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17, colour_white, "%i", m_spearAmmo);
						masked_stretch_blit(inventorySharpStick,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if((m_inventoryItem[itemnumber] == HARDENED_SPEAR) && (m_hardSpearAmmo > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17,colour_white, "%i", m_hardSpearAmmo);
						masked_stretch_blit(hardenedSpear,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if(m_inventoryItem[itemnumber] == KINDLING)
					{
						masked_stretch_blit(inventoryKindling,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == MEAT)
					{
						masked_stretch_blit(meat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == BONE)
					{
						masked_stretch_blit(bone,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == SINEW)
					{
						masked_stretch_blit(sinew,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == COOKED_MEAT)
					{
						masked_stretch_blit(cookedmeat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == RABBIT)
					{
						masked_stretch_blit(rabbit,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == RABBIT_MEAT)
					{
						masked_stretch_blit(rabbitMeat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == RABBIT_HIDE)
					{
						masked_stretch_blit(rabbitHide,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == CURED_RABBIT_HIDE)
					{
						masked_stretch_blit(curedRabbitHide,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_inventoryItem[itemnumber] == LEATHER_FLASK)
					{
						masked_stretch_blit(leatherFlask,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if (m_inventoryItem[itemnumber] == FLINT)
					{
						masked_stretch_blit(flint,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if (m_inventoryItem[itemnumber] == FISH_ONE)
					{
						masked_stretch_blit(fishone,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if (m_inventoryItem[itemnumber] == COOKED_FISH_ONE)
					{
						masked_stretch_blit(cookedfishone,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if (m_inventoryItem[itemnumber] == COOKED_RABBIT_MEAT)
					{
						masked_stretch_blit(cookedrabbitmeat,buffer,0,0,82,50,bagx,bagy,41,25);

					}

					if((mouse_b & 1) && (mouse_x > bagx) && (mouse_y > bagy) && (mouse_x < bagx + 41) && (mouse_y < bagy + 25) && (m_mousetimer == 0) && (m_moveitem == 0))
					{
						m_moveitem = m_inventoryItem[itemnumber];
						
						if(m_moveitem == STICK)
						{
							m_woodCount --;
						}
						else
						{
							m_inventoryItem[itemnumber] = 0;
						}
						
						m_mousetimer = 5;
					}

				itemnumber++;
				bagx = bagx + 53;

				}

				bagx = 218;
				bagy = bagy + 35;
			}

			bagx = 519;
			bagy = 245;

			itemnumber = 0;
			
			for(j=0;j <rows; j++)
			{				
				for(int i=0;i< columns;i++)
				{	
					if((m_baseItem[itemnumber] == STICK) && (m_baseWoodCount > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17, colour_white, "%i", m_baseWoodCount);
						masked_stretch_blit(inventoryStick,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if((m_baseItem[itemnumber] == SHARP_STICK) && (m_spearAmmo > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17, colour_white, "%i", m_spearAmmo);
						masked_stretch_blit(inventorySharpStick,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if((m_baseItem[itemnumber] == HARDENED_SPEAR) && (m_hardSpearAmmo > 0))
					{
						textprintf(buffer,font,bagx+33,bagy+17,colour_white, "%i", m_hardSpearAmmo);
						masked_stretch_blit(hardenedSpear,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if(m_baseItem[itemnumber] == KINDLING)
					{
						masked_stretch_blit(inventoryKindling,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == MEAT)
					{
						masked_stretch_blit(meat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == BONE)
					{
						masked_stretch_blit(bone,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == SINEW)
					{
						masked_stretch_blit(sinew,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == COOKED_MEAT)
					{
						masked_stretch_blit(cookedmeat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == RABBIT)
					{
						masked_stretch_blit(rabbit,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == RABBIT_MEAT)
					{
						masked_stretch_blit(rabbitMeat,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == RABBIT_HIDE)
					{
						masked_stretch_blit(rabbitHide,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == CURED_RABBIT_HIDE)
					{
						masked_stretch_blit(curedRabbitHide,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if(m_baseItem[itemnumber] == LEATHER_FLASK)
					{
						masked_stretch_blit(leatherFlask,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if (m_baseItem[itemnumber] == FLINT)
					{
						masked_stretch_blit(flint,buffer,0,0,82,50,bagx,bagy,41,25);
					}
					else if (m_baseItem[itemnumber] == FISH_ONE)
					{
						masked_stretch_blit(fishone,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if (m_baseItem[itemnumber] == COOKED_FISH_ONE)
					{
						masked_stretch_blit(cookedfishone,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					else if (m_baseItem[itemnumber] == COOKED_RABBIT_MEAT)
					{
						masked_stretch_blit(cookedrabbitmeat,buffer,0,0,82,50,bagx,bagy,41,25);

					}
					
					if((mouse_b & 1) && (mouse_x > bagx) && (mouse_y > bagy) && (mouse_x < bagx + 41) && (mouse_y < bagy + 25) && (m_mousetimer == 0) && (m_moveitem == 0))
					{
						m_moveitem = m_baseItem[itemnumber];
						
						if(m_moveitem == STICK)
						{
							m_baseWoodCount --;
						}
						else
						{
							m_baseItem[itemnumber] = 0;
						}
						m_mousetimer = 5;
					}


				itemnumber++;
				bagx = bagx + 53;
				}

				bagx = 519;
				bagy = bagy + 35;

			}
			
			
			if(m_moveitem > 0)
			{
				if((mouse_b & 1) && (mouse_x > tablex + 10) && (mouse_y > tabley + 45) && (mouse_x < tablex + 265) && (mouse_y < tabley + 185) && (m_mousetimer == 0) && (m_spaceAvail))
				{

					if((m_moveitem == STICK) && (m_woodSlotOpened) && (m_woodCount < 11))
					{
						m_woodCount++;
					}
					else if((m_moveitem == STICK) && (!m_woodSlotOpened))
					{
						m_woodCount++;
						m_inventoryItem[m_nextEmpty] = m_moveitem;
					}
					else
					{
						m_inventoryItem[m_nextEmpty] = m_moveitem;
					}
					
					m_moveitem = 0;
					m_mousetimer = 5;
				}
				else if((mouse_b & 1) && (mouse_x > tablex + 295) && (mouse_y > tabley + 45) && (mouse_x < tablex + 575) && (mouse_y < tabley + 185) && (m_mousetimer == 0) && (m_baseSpaceAvail))
				{
					if(m_moveitem == STICK)
					{
						m_baseWoodCount++;
					}
					
					m_baseItem[m_nextEmptyBase] = m_moveitem;
					m_moveitem = 0;
					m_mousetimer = 5;
				}

			}

			DrawMiniItem(buffer,m_moveitem);
			//textprintf(buffer,font,0,110, colour_white, "%i",m_moveitem);
			//textprintf(buffer,font,0,120, colour_white, "%i",m_baseItem[0]);
			//textprintf(buffer,font,0,130, colour_white, "%i",m_baseItem[1]);
			//textprintf(buffer,font,0,140, colour_white, "%i",m_baseItem[2]);
			//textprintf(buffer,font,0,150, colour_white, "%i",m_baseItem[3]);

			int ymin, ymax;

			ymin = 25;
			ymax = 55;

			int option = 0;

			for(int i = 0; i < 3; i++)
			{
	
				if((mouse_x > tablex + 575) && (mouse_x < tablex + 645) && (mouse_y > tabley + ymin) && (mouse_x > tabley + ymax) && (mouse_b & 1))
				{
					option = i+1;
				}

				ymin = ymax + 20;
				ymax = ymin + 30;
			}

			//	textprintf(buffer,font,0,160, colour_white, "%i",option);


			if(m_mousetimer > 0)
				m_mousetimer--;
		
			//SAVE

			if(option == 1)
			{
				*save = true;
			}
			else if(option == 3)
			{
				*sleep = true;
			}
			else if(option == 2)
			{
				*buildbase = true;
				*close = false; 
			}

			//LOAD

			//SLEEP	
			
			SortSecondStore();
}

void CInventory::SortSecondStore()
{
	int stickSlots = 0;
	bool stickOpen = false;

	for(int i = 0; i < 20;i++)
	{
		
		int j = i + 1;

		if((m_baseItem[i] == 0) && (m_baseItem[j] > 0) && ( j != 20))
		{
			m_baseItem[i] = m_baseItem[j];
			m_baseItem[j] = 0;
		}
		else if(m_baseItem[i] == 0)
		{
			m_nextEmptyBase = i;
			i = 20;
			m_baseSpaceAvail = true;
		}

		if(m_baseItem[i] == STICK)
		{
			stickSlots++;
		}


	}

	if(m_baseItem[19] > 0)
	{
		m_nextEmptyBase = 20;
		m_baseSpaceAvail = false;

	}

	if(stickSlots > 1)
	{
		m_baseItem[i] = 0;
		stickSlots--;
	}


	
}

void CInventory::DrawMiniItem(BITMAP *buffer, int itemnumber)
{
	
	if(itemnumber == STICK) 
	{
		masked_stretch_blit(inventoryStick,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == SHARP_STICK)
	{
		masked_stretch_blit(inventorySharpStick,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == KINDLING)
	{
		masked_stretch_blit(inventoryKindling,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == MEAT)
	{
		masked_stretch_blit(inventoryKindling,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == BONE)
	{
		masked_stretch_blit(bone,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == SINEW)
	{
		masked_stretch_blit(sinew,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == COOKED_MEAT)
	{
		masked_stretch_blit(cookedmeat,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == HARDENED_SPEAR)
	{
		masked_stretch_blit(hardenedSpear,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == RABBIT)
	{
		masked_stretch_blit(rabbit,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == RABBIT_MEAT)
	{
		masked_stretch_blit(rabbitMeat,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber  == RABBIT_HIDE)
	{
		masked_stretch_blit(rabbitHide,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber  == CURED_RABBIT_HIDE)
	{
		masked_stretch_blit(curedRabbitHide,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber  == LEATHER_FLASK)
	{
		masked_stretch_blit(leatherFlask,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber  == COOKED_RABBIT_MEAT)
	{
	masked_stretch_blit(cookedrabbitmeat,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber  == COOKED_FISH_ONE)
	{
		masked_stretch_blit(cookedfishone,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == FISH_ONE)
	{
		masked_stretch_blit(fishone,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}
	else if(itemnumber == FLINT)
	{
		masked_stretch_blit(flint,buffer,0,0,82,50,mouse_x,mouse_y,41,25);
	}


}