#include "menu.h"
#include "outputText.h"
#include "HCI.h"

CProfileEditor::CProfileEditor(CPlayer *player)
{
	m_numberOfElements = 0;

	m_element = new CFormElement*;

	m_element[m_numberOfElements] = new CStaticText("name",xScreenSize - 390,150,50,50,0,0);
	m_element[m_numberOfElements++]->setText("Name");

	m_element[m_numberOfElements++] = new CEditBox("player_name",xScreenSize - 330,150,175,50,0,16,1);

	m_element[m_numberOfElements++] = new CFormElement("next",xScreenSize - 160,460,50,50,0,
	CTextureMan::getInstance()->addMenuTexture("data/menu/arrow_right.bmp"));

	m_element[m_numberOfElements++] = new CFormElement("previous",xScreenSize - 390,460,50,50,0,
	CTextureMan::getInstance()->addMenuTexture("data/menu/arrow_left.bmp"));

	m_element[m_numberOfElements++] = new CFormElement("apply",xScreenSize - 300,460,100,50,0,
	CTextureMan::getInstance()->addMenuTexture("data/menu/apply.bmp"));

	m_element[m_numberOfElements++] = new CFormElement("close",xScreenSize - 145,87,30,30,0,2);

	m_element[m_numberOfElements] = new CStaticText("profile_head",xScreenSize - 390,87,50,50,0,0);
	m_element[m_numberOfElements++]->setText("Player Profile");

	m_activeGameMode = MENU_LOOP;

	m_currentCharacter = 0;

	player->loadProfile();
	strcpy(m_element[1]->m_contents,player->m_playerName);
}

void CProfileEditor::renderModel(CPlayer *player)
{
	int playersSavedCharacter = player->m_activeChar;

	glDisable(GL_BLEND);
	player->renderCharacterChooser();

	for(int i = 0; i < m_numberOfElements; i++)
	{
		if(m_element[i]->callBack() == SELECTED) 
		{
			if(m_element[i]->m_callbackID == "next")
			{
				playersSavedCharacter++;
				if(playersSavedCharacter > 2) playersSavedCharacter = 0;

				player->changeCharacter(playersSavedCharacter);
			}
			else if(m_element[i]->m_callbackID == "previous")
			{
				playersSavedCharacter--;
				if(playersSavedCharacter < 0) playersSavedCharacter = 2;

				player->changeCharacter(playersSavedCharacter);
			}
			else if(m_element[i]->m_callbackID == "apply")
			{
				ofstream fout;

				fout.open("Data\\Settings\\profile.dat");

				fout << "character_model " << playersSavedCharacter << "\n";
				fout << "player_name " << m_element[1]->m_contents;

				m_activeGameMode = PROFILE_APPLIED_MESSAGE;
			}
			else if(m_element[i]->m_callbackID == "close")
			{
				player->loadProfile();
				strcpy(m_element[1]->m_contents,player->m_playerName);
			}
		}
	}
}

int CProfileEditor::callback()
{

	for(int i = 0; i < m_numberOfElements; i++)
	{
		if(m_element[i]->callBack() == SELECTED) 
		{
			if(m_element[i]->m_callbackID == "close")
			{
				return NULL_SUB;
			}
			else if(m_element[i]->m_callbackID == "apply")
			{
				return PROFILE_APPLIED_MESSAGE;
			}
		}
	}

	return -1;

}