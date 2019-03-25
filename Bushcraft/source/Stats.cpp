#include "Stats.h"
#include <fstream>
#include <string>

using namespace std;

CStats::CStats()
{
	statsWindow = NULL;

	m_playerLevel = 1;
	m_playerLevelExp = 1;
	m_expToPlayerLvl = 0;
	m_agility = 1;
	m_strength = 1;

	colour_white = makecol(225,222,194);
}

CStats::~CStats()
{
	if(m_binitialise)
	{
		destroy_bitmap(statsWindow);
	}


}

bool CStats::initialise()
{
	statsWindow = load_bitmap("PlayerStats.bmp",0);
	
	FireSkill(NULL);
	TrappingSkill(NULL);
	HuntingSkill(NULL);
	FishingSkill(NULL);
	CookingSkill(NULL);
	
	return m_binitialise = true;
	

}

void CStats::DrawStats(BITMAP *buffer)
{
	

	masked_stretch_blit(statsWindow,buffer,0,0,212,326,0,290,212,326);
	textprintf_centre_ex(buffer, font,144,353, colour_white,-1, "%i", m_strength);
	textprintf_centre_ex(buffer, font,144,388, colour_white,-1, "%i", m_agility);
	textprintf_centre_ex(buffer, font,144,424, colour_white,-1, "%i", m_playerLevel);

	textprintf_right_ex(buffer, font,190,455, colour_white,-1, "%i", m_huntingSkillLevel);
	textprintf_right_ex(buffer, font,190,485, colour_white,-1, "%i", m_fireSkillLevel);
	textprintf_right_ex(buffer, font,190,515, colour_white,-1, "%i", m_fishingSkillLevel);
	textprintf_right_ex(buffer, font,190,545, colour_white,-1, "%i", m_trappingSkillLevel);
	textprintf_right_ex(buffer, font,190,575, colour_white,-1, "%i", m_cookingSkillLevel);

	StatsModal();
	if(m_mouseOverStat == 0) // HUNTING
	{
		textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Exp to next level: %i", m_expToHuntingLvl);
	}
	else if(m_mouseOverStat == 1) // FIRE
	{
		textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Exp to next level: %i", m_expToFireLvl);
	}
	else if(m_mouseOverStat == 2) // FISHING
	{
		textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Exp to next level: %i", m_expToFishingLvl);
	}
	else if(m_mouseOverStat == 3) // TRAPPING
	{
		textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Exp to next level: %i", m_expToTrappingLvl);
	}
	else if(m_mouseOverStat == 4) // COOKING
	{
		textprintf_ex(buffer, font,mouse_x + 10,mouse_y, colour_white,1, "Exp to next level: %i", m_expToCookingLvl);
	}




}

void CStats::FireSkill(int increase)
{
	m_fireSkillLevelExp = m_fireSkillLevelExp + increase;

	m_expToFireLvl = ((m_fireSkillLevel * 10000) - m_fireSkillLevelExp) ;

	if(m_expToFireLvl <= 0)
	{
		m_fireSkillLevel++;
		m_fireSkillLevelExp = 0;
	}

	m_expToFireLvl = ((m_fireSkillLevel * 10000) - m_fireSkillLevelExp) ;
}

void CStats::TrappingSkill(int increase)
{
	m_trappingSkillLevelExp = m_trappingSkillLevelExp + increase;

	m_expToTrappingLvl = ((m_trappingSkillLevel * 10000) - m_trappingSkillLevelExp) ;

	if(m_expToTrappingLvl == 0)
	{
		m_trappingSkillLevel++;
		m_trappingSkillLevelExp = 0;
	}
}

void CStats::FishingSkill(int increase)
{
	m_fishingSkillLevelExp = m_fishingSkillLevelExp + increase;

	m_expToFishingLvl = ((m_fishingSkillLevel * 10000) - m_fishingSkillLevelExp) ;

	if(m_expToFishingLvl == 0)
	{
		m_trappingSkillLevel++;
		m_trappingSkillLevelExp = 0;
	}
}

void CStats::HuntingSkill(int increase)
{
	m_huntingSkillLevelExp = m_huntingSkillLevelExp + increase;

	m_expToHuntingLvl = ((m_fireSkillLevel * 10000) - m_huntingSkillLevelExp) ;

	if(m_expToHuntingLvl == 0)
	{
		m_huntingSkillLevel++;
		m_huntingSkillLevelExp = 0;
	}
}

void CStats::CookingSkill(int increase)
{
	m_cookingSkillLevelExp = m_cookingSkillLevelExp + increase;

	m_expToCookingLvl = ((m_cookingSkillLevel * 10000) - m_cookingSkillLevelExp) ;

	if(m_expToCookingLvl == 0)
	{
		m_cookingSkillLevel++;
		m_cookingSkillLevelExp = 0;
	}

}

void CStats::StatsModal()
{
	int ymin, ymax;

	ymin = 449;
	ymax = 472;

	m_mouseOverStat = -1;

	for(int i = 0; i < 5;i++)
	{	
		if((mouse_x > 15) && (mouse_x < 195) && (mouse_y < ymax) && (mouse_y > ymin))
		{
			m_mouseOverStat = i;
		}

		ymin = ymax + 10;
		ymax = ymin + 20;
		
	}

}

void CStats::Load(const char datafile[])
{

	ifstream statFile;
	ifstream mainfile;

	char buf[1024];
	char file[1024];

	mainfile.open(datafile);

	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');

	statFile.open(file);

	if (!statFile.is_open()) {
		allegro_message("Unable to open inventory file");
	}

	statFile.getline(buf,1024,'\n');
		
	m_fireSkillLevel = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_fireSkillLevelExp = atoi(buf);

	statFile.getline(buf,1024,'\n');
		
	m_trappingSkillLevel  = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_trappingSkillLevelExp  = atoi(buf);

	statFile.getline(buf,1024,'\n');
		
	m_fishingSkillLevel = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_fishingSkillLevelExp  = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_huntingSkillLevel  = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_huntingSkillLevelExp  = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_cookingSkillLevel  = atoi(buf);

	statFile.getline(buf,1024,'\n');

	m_cookingSkillLevelExp  = atoi(buf);

	
	statFile.close();
	mainfile.close();

}

void CStats::Save(const char datafile[])
{

	ifstream mainfile;

	char file[1024];

	mainfile.open(datafile);

	mainfile.getline(file,1024, '\n');
	mainfile.getline(file,1024, '\n');

	ofstream savStat;
	
	savStat.open(file, ios::out);

	savStat << m_fireSkillLevel << "\n";
	savStat << m_fireSkillLevelExp << "\n";
	savStat << m_trappingSkillLevel << "\n";
	savStat << m_trappingSkillLevelExp << "\n";
	savStat << m_fishingSkillLevel << "\n";
	savStat<< m_fishingSkillLevelExp << "\n";
	savStat << m_huntingSkillLevel << "\n";
	savStat << m_huntingSkillLevelExp << "\n";
	savStat << m_cookingSkillLevel << "\n";
	savStat << m_cookingSkillLevelExp << "\n";


	savStat.close();

}


