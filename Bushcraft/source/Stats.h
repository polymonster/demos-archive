#include <allegro.h>
#ifndef INC_STATS_H
#define INC_STATS_H

class CStats 
{

protected:

	int m_playerLevel;
	int m_playerLevelExp;
	int m_expToPlayerLvl;
	int m_agility;
	int m_strength;

	int m_fireSkillLevel;
	int m_fireSkillLevelExp;
	int m_expToFireLvl;

	int m_huntingSkillLevel;
	int m_huntingSkillLevelExp;
	int m_expToHuntingLvl;

	int m_cookingSkillLevel;
	int m_cookingSkillLevelExp;
	int m_expToCookingLvl;

	int m_trappingSkillLevel;
	int m_trappingSkillLevelExp;
	int m_expToTrappingLvl;

	int m_fishingSkillLevel;
	int m_fishingSkillLevelExp;
	int m_expToFishingLvl;

	int m_mouseOverStat;

	bool m_binitialise;

	BITMAP *statsWindow;

	int colour_white;



public:

	CStats();
	~CStats();

	bool initialise();

	void DrawStats(BITMAP *buffer);
	
	void PlayerLevel();
	void FireSkill(int increase);
	void HuntingSkill(int increase);
	void CookingSkill(int increase);
	void TrappingSkill(int increase);
	void FishingSkill(int increase);
	
	void StatsModal();

	void Load(const char datafile[]);
	void Save(const char datafile[]);

	int getFireSkill() {return m_fireSkillLevel;};
	int getHuntingSkill() {return m_huntingSkillLevel;};
	int getCookingSkill() {return m_cookingSkillLevel;};
	int getTrappingSkill() {return m_trappingSkillLevel;};
	int getFishingSkill() {return m_fishingSkillLevel;};


};

#endif