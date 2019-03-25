#include <allegro.h>
#ifndef INC_INVENTORY_H
#define INC_INVENTORY_H

class CInventory {

protected:
	
	int m_inventoryItem[20];
	int m_tools[5];
	int m_nextEmpty;
	bool m_inventoryOpen;
	bool m_binitialised;
	int m_selectedItem;
	bool m_craftToolSelected;
	bool m_fireToolSelected;
	bool m_toolSelected;
	int m_woodCount;
	bool m_hasKindling;
	int m_spearCount;
	int m_mousetimer;
	bool m_lightFire;
	int m_equippedItem;
	int m_weapon;
	int m_item;
	int m_justUsed;
	int m_spearAmmo;
	int m_hardSpearAmmo;
	bool m_woodSlotOpened;
	int m_returnAmmo;
	bool m_spaceAvail;
	bool hasCraft;
	int m_healthIncrease;
	bool m_hasBone;
	bool m_hasSinew;
	int colour_white;
	int colour_grey;
	int colour_yellow;
	int colour_green;
	int colour_dkred;
	int colour_blue;
	bool m_makeToolSelected;
	bool m_weaponToolSelected;
	int selectedOption;
	bool m_making;
	bool m_sewing;
	bool m_hasSewingKit;
	bool m_hasRabbit;
	bool m_hasCuredRabbitHide;
	bool m_hasRabbitHide;
	int m_flaskWaterLevel;
	bool m_inWaterMenu;
	int m_returnTimer;
	bool m_drinking;
	int ftx,wtx,ctx,mtx;
	bool m_hasWaterFlask;
	bool m_equippedMoveItem;
	bool m_looting;
	int m_lootitem[6];
	int m_returnLoot;
	int foodHeal[100];
	bool m_buildcamp;
	int m_baseItem[20];
	int m_moveitem;
	int m_nextEmptyBase;
	bool m_baseSpaceAvail;
	int m_moceitemCount;
	int m_baseWoodCount;

	BITMAP *inventoryStick;
	BITMAP *inventoryStone;
	BITMAP *inventorySharpStick;
	BITMAP *inventoryKindling;
	BITMAP *inventoryHandDrill;
	BITMAP *inventoryBg;
	BITMAP *meat;
	BITMAP *bone;
	BITMAP *sinew;
	BITMAP *spark;
	BITMAP *cookedmeat;
	BITMAP *toolbg;
	BITMAP *make;
	BITMAP *weaponEmpty;
	BITMAP *weaponSpear;
	BITMAP *weaponHardenedSpear;
	BITMAP *hardenedSpear;
	BITMAP *sewingTab;
	BITMAP *makeTab;
	BITMAP *rabbit;
	BITMAP *rabbitMeat;
	BITMAP *rabbitHide;
	BITMAP *curedRabbitHide;
	BITMAP *leatherFlask;
	BITMAP *waterWindow;
	BITMAP *lootTable;
	BITMAP *flint;
	BITMAP *fishone;
	BITMAP *cookedfishone;
	BITMAP *cookedrabbitmeat;
	BITMAP *storeinventory;



public:

	CInventory();
	~CInventory();
	
	bool Initialise();

	void AddItem(int itemNumber);

	void InventModal(bool *inventoryopen, BITMAP *buffer, bool *startbuilding);

	void InventDespawn(int quantity, int item);
	void Sort();

	bool lightFire() { return m_lightFire;};
	void setfirebool();

	void CInventory::equippedItem(BITMAP *buffer);
	int returnEquippedItem() {return m_equippedItem;};
	int returnEquippedWeapon() {return m_weapon;};
	int returnWood() {return m_woodCount;};
	void itemUsed();
	void closeMenus();
	
	void MakeTool(BITMAP *buffer, bool *startbuilding);
	void CraftTool(BITMAP *buffer);
	void FireTool(BITMAP *buffer);
	void WeaponTool(BITMAP *buffer);
	
	void useAmmo(int weapon);
	void getAmmo(int weapon, int *m_returnAmmo); 
	
	
	void healthIncreased();
	int healthIncrease() {return m_healthIncrease;};

	void UseFlask(int mode);

	void WaterMenu(BITMAP *buffer, int xpos, int ypos,int *option);

	bool returnWaterMenu() {return m_inWaterMenu;};

	int returnTimer() {return m_returnTimer;};

	void timerCount();

	int Drinking() { return m_drinking;};

	int returnLoot() { return m_returnLoot;};

	bool returnLootingBool() {return m_looting;};

	void LootNode(int loot1,int loot2,int loot3,int loot4,int loot5,int loot6,BITMAP *buffer,int boxx, int boxy);

	void Load(const char datafile[]);

	void Save(const char datafile[]);

	void updateLoot(int *loot1,int *loot2,int *loot3,int *loot4,int *loot5,int *loot6);

	void storeInventory(BITMAP *buffer,bool storeopen, bool *close, bool *save, bool *sleep, bool *buildbase);

	void SortSecondStore();

	void DrawMiniItem(BITMAP *buffer, int itemnumber);
	

};

#endif