#include "menu.h"
#include "outputText.h"
#include "HCI.h"
#include "textureSingleton.h"

int closeButtonID;
int boxID;
int dropButtonID;
int dropBackID;
int cursorID;
int textBoxID;
bool netConnect;
bool inLobby = false;

CMenu::CMenu()
{
	m_numberOfElements = 0;
	m_activeGameMode = MENU_LOOP;
}

CMenu::CMenu(int menuType)
{
	m_numberOfElements = 0;

	m_element = new CFormElement*;

	if(menuType == 0) // main bar
	{
		int y = 20;
		int spacing = 70;

		m_element[m_numberOfElements++] = new CFormElement("single_player",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/single.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("network_sub",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/network.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("profile_sub",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/profile.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("settings_sub",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/settings.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("credits_sub",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/credits.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("controls_sub",75,y += spacing,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/controls.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("exit",xScreenSize - 120,yScreenSize - 70,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/quit.bmp"));
	}
	else if(menuType == 1) // network menu
	{
		m_element= new CFormElement*[20];

		m_element[m_numberOfElements++] = new CEditBox("network_name",xScreenSize - 380,165,260,50,0,64,textBoxID);

		m_element[m_numberOfElements++] = new CFormElement("network_host",xScreenSize - 390,400,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/host.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("network_join_options",xScreenSize - 210,400,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/join.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("network_join",xScreenSize - 300,460,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/connect.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("network_game_start",xScreenSize - 300,460,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/start.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("close",xScreenSize - 145,87,30,30,0,closeButtonID);

		//set name slots to inactive
		m_element[m_numberOfElements] = new CStaticText("player1_name",xScreenSize - 370,280,50,50,0,0);
		m_element[m_numberOfElements++]->m_active = false;

		m_element[m_numberOfElements] = new CStaticText("player2_name",xScreenSize - 370,300,50,50,0,0);
		m_element[m_numberOfElements++]->m_active = false;

		m_element[m_numberOfElements] = new CStaticText("network_head",xScreenSize - 390,87,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Network Play");

		m_element[m_numberOfElements] = new CStaticText("hint_host",xScreenSize - 380,140,280,40,0,1);
		m_element[m_numberOfElements++]->setText("Your host computer name is");

		m_element[m_numberOfElements] = new CStaticText("hint_join",xScreenSize - 380,140,280,40,0,1);
		m_element[m_numberOfElements++]->setText("Enter the host computer name or IP address");

		m_element[m_numberOfElements] = new CStaticText("hint_start",xScreenSize - 380,210,280,40,0,1);
		m_element[m_numberOfElements]->setText("You are the party leader, click start game to play");
		m_element[m_numberOfElements++]->m_active = false;

		m_element[m_numberOfElements] = new CStaticText("hint_wait",xScreenSize - 380,210,280,40,0,1);
		m_element[m_numberOfElements]->setText("Awaiting party leader to start the game.....");
		m_element[m_numberOfElements++]->m_active = false;
	
	}
	else if(menuType == 2) // settings menu
	{

		m_element[m_numberOfElements] = new CDropDownBox("resolution",xScreenSize - 240,150,100,30,0,0,boxID,dropButtonID,dropBackID);
		m_element[m_numberOfElements]->addItem("800 x 600");
		m_element[m_numberOfElements]->addItem("1024 x 768");
		m_element[m_numberOfElements]->addItem("1280 x 1024");
		m_element[m_numberOfElements]->addItem("1440 x 900");
		m_element[m_numberOfElements]->addItem("1680 x 1050");
		m_element[m_numberOfElements++]->setInitialValue(user::getInstance()->settings.resolutionID);

		m_element[m_numberOfElements] = new CDropDownBox("shadow_quality",xScreenSize - 240,210,100,30,0,0,boxID,dropButtonID,dropBackID);
		m_element[m_numberOfElements]->addItem("High");
		m_element[m_numberOfElements]->addItem("Low");
		m_element[m_numberOfElements]->addItem("Off");
		m_element[m_numberOfElements++]->setInitialValue(user::getInstance()->settings.shadowQualityID);

		m_element[m_numberOfElements] = new CDropDownBox("texture_quality",xScreenSize - 240,270,100,30,0,0,boxID,dropButtonID,dropBackID);
		m_element[m_numberOfElements]->addItem("High");
		m_element[m_numberOfElements]->addItem("Medium");
		m_element[m_numberOfElements]->addItem("Low");
		m_element[m_numberOfElements]->m_active = false;
		m_element[m_numberOfElements++]->setInitialValue(user::getInstance()->settings.textureQualityID);

		m_element[m_numberOfElements++] = new CFormElement("apply_settings",xScreenSize - 300,460,100,50,0,
		CTextureMan::getInstance()->addMenuTexture("data/menu/apply.bmp"));

		m_element[m_numberOfElements++] = new CFormElement("close",xScreenSize - 145,87,30,30,0,closeButtonID);

		m_element[m_numberOfElements] = new CStaticText("res",xScreenSize - 390,150,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Resolution");

		m_element[m_numberOfElements] = new CStaticText("shad",xScreenSize - 390,210,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Shadow quality");


		m_element[m_numberOfElements] = new CStaticText("settings_head",xScreenSize - 390,87,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Settings");

	}
	else if(menuType == 3) // credits menu
	{
		m_element= new CFormElement*[20];

		m_element[m_numberOfElements++] = new CFormElement("close",xScreenSize - 145,87,30,30,0,closeButtonID);

		m_element[m_numberOfElements] = new CStaticText("credits_head",xScreenSize - 390,87,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Credits");

		int y = 100;
		int spacing = 20;
		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Alex Dixon");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-All game programming");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-Environment models");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-Environment textures / baking");

		y += spacing;

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Mark Chambers");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-Artwork for HUD / Menu");

		y += spacing;

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Ben Jacob");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-Music / Atmosphere");

		y += spacing;

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Sound");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-Taken from OpenArena");

		y += spacing;

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Character Models");

		m_element[m_numberOfElements] = new CStaticText("a",xScreenSize - 370,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("-From ioquake3.org");



	}
	else if(menuType == 4) // controls menu
	{
		m_element= new CFormElement*[20];

		m_element[m_numberOfElements++] = new CFormElement("close",xScreenSize - 145,87,30,30,0,closeButtonID);

		m_element[m_numberOfElements] = new CStaticText("controls_head",xScreenSize - 390,87,50,50,0,0);
		m_element[m_numberOfElements++]->setText("Controls");

		int y = 100;
		int spacing = 23;
		m_element[m_numberOfElements] = new CStaticText("w",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[W] - Walk Forward");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[S] - Walk Backwards");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[A] - Strafe Left");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[D] - Strafe Right");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[Left Mouse] - Shoot");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[Right Mouse] - Toggle Aim");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[Q] - Run");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[C] - Crouch");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[R] - Reload");

		m_element[m_numberOfElements] = new CStaticText("s",xScreenSize - 390,y += spacing,50,50,0,0);
		m_element[m_numberOfElements++]->setText("[Key 1 - 4] - Change Weapon");
	}
	else if(menuType == 5)
	{
		m_element= new CFormElement*[4];

		m_element[m_numberOfElements] = new CStaticText("1",(xScreenSize / 2) - 190,(yScreenSize / 2) - 180,50,50,0,0);
		m_element[m_numberOfElements++]->setText("blank");

		m_element[m_numberOfElements] = new CStaticText("2",(xScreenSize / 2) - 190,(yScreenSize / 2) - 160,50,50,0,0);
		m_element[m_numberOfElements++]->setText("blank");

		m_element[m_numberOfElements++] = new CFormElement("close_message",(xScreenSize / 2) - 50,(yScreenSize / 2) - 100,100,50,0,
			CTextureMan::getInstance()->addMenuTexture("Data/Menu/close.bmp"));
		
	}

	m_activeGameMode = MENU_LOOP;

	m_menuMode = 0;

}

void CMenu::draw()
{
	for(int i = 0; i < m_numberOfElements; i++)
	{
		if(m_element[i]->m_active) m_element[i]->draw();
	}

	//variables to aid design speed
	if(user::getInstance()->input.key['`']) outputCursorPos();
}

void CMenu::check()
{
	for(int i = 0; i < m_numberOfElements; i++)
	{
		if(m_element[i]->m_active) m_element[i]->check();
	}

	elementProc();

}

int CMenu::callback()
{
	return m_activeGameMode;
}

void CMenu::setActiveStates()
{
	for(int i = 0; i < m_numberOfElements; i++)
	{
		switch(m_menuMode)
		{

			//NETWORK MENU HOME
			case  0 :

				if((m_element[i]->m_callbackID == "network_game_start") || (m_element[i]->m_callbackID == "network_name") 
					|| (m_element[i]->m_callbackID == "network_join") || (m_element[i]->m_callbackID == "hint_host") 
					|| (m_element[i]->m_callbackID == "hint_join") || (m_element[i]->m_callbackID == "hint_wait")
					|| (m_element[i]->m_callbackID == "hint_start"))
				{
					m_element[i]->m_active = false;
					if(m_element[i]->m_callbackID == "network_name") strcpy(m_element[i]->m_contents,"");
				}
				else if((m_element[i]->m_callbackID == "network_host") || (m_element[i]->m_callbackID == "network_join_options"))
				{
					m_element[i]->dontCheck = false;
				}

			break;

			//HOST GAME OPTIONS
			case  1 :

				if(m_element[i]->m_callbackID == "network_game_start")
				{
					m_element[i]->m_active = true;
				}
				else if(m_element[i]->m_callbackID == "network_name")
				{
					m_element[i]->m_active = true;
					m_element[i]->dontCheck = true;
				}
				else if(m_element[i]->m_callbackID == "hint_host")
				{
					m_element[i]->m_active = true;
				}
				else if(m_element[i]->m_callbackID == "network_join")
				{
					m_element[i]->m_active = false;
				}
				else if(m_element[i]->m_callbackID == "network_host")
				{
					m_element[i]->dontCheck = true;
				}
				else if(m_element[i]->m_callbackID == "hint_join")
				{
					m_element[i]->m_active = false;
				}

			break;

			//JOIN GAME OPTIONS
			case  2 :

				if(m_element[i]->m_callbackID == "network_name")
				{
					m_element[i]->dontCheck = false;
					m_element[i]->m_active = true;
				}
				else if(m_element[i]->m_callbackID == "network_game_start")
				{
					m_element[i]->m_active = false;
				}
				else if(m_element[i]->m_callbackID == "network_join")
				{
					//if(!netConnect) m_element[i]->m_active = true;
					//else m_element[i]->m_active = false;

					m_element[i]->m_active = true;
				}
				else if(m_element[i]->m_callbackID == "hint_join")
				{
					m_element[i]->m_active = true;
				}
				else if(m_element[i]->m_callbackID == "network_join_options")
				{
					m_element[i]->dontCheck = true;
				}
				else if(m_element[i]->m_callbackID == "hint_host")
				{
					m_element[i]->m_active = false;
				}
				else if(m_element[i]->m_callbackID == "hint_wait")
				{
					//if the players in the lobby with the host,
					//tell them that the other player has to start the match
					if(inLobby) m_element[i]->m_active = true;
					else m_element[i]->m_active = false;
				}


			break;

		}
	}
}

void CMenu::elementProc()
{
	for(int i = 0; i < m_numberOfElements; i++)
	{
		if(m_element[i]->m_callbackID == "network_name")
		{
			computerName = m_element[i]->m_contents;
			//return;
		}
		else if(m_element[i]->m_callbackID == "exit") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = EXIT_GAME;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "single_player") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = GAME_LOOP;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "forge") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = EDITOR_LOOP;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "network_sub") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = NETWORK_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "network_host") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_menuMode = 1;
				m_activeGameMode = NETWORK_HOST;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "network_join") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				inLobby = !inLobby;
				m_activeGameMode = NETWORK_JOIN;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "network_join_options")
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_menuMode = 2;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "network_game_start") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				for(int j = 0; j < m_numberOfElements; j++)
				{
					if((m_element[j]->m_callbackID == "player2_name")
						|| (m_element[j]->m_callbackID == "player1_name"))
					{

						if(!m_element[j]->m_active) 
						{
							//message to say "you cant connect until other players"
							m_activeGameMode = WAIT_FOR_PLAYERS_MESSAGE;
							return;
						}
					}

				}

				for(j = 0; j < m_numberOfElements; j++)
				{
					if((m_element[j]->m_callbackID == "player2_name")
						|| (m_element[j]->m_callbackID == "player1_name"))
					{
						m_element[j]->m_active = false;
					}

				}

				m_activeGameMode = NETWORK_GAME_START;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "profile_sub") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = PROFILE_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "settings_sub") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = SETTINGS_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "credits_sub") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = CREDITS_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "controls_sub") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = CONTROLS_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "close") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_menuMode = 0;

				for(int j = 0; j < m_numberOfElements; j++)
				{
					if((m_element[j]->m_callbackID == "player2_name")
						|| (m_element[j]->m_callbackID == "player1_name"))
					{
						m_element[j]->m_active = false;
					}

				}

				m_activeGameMode = NULL_SUB;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "apply_settings") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = WRITE_SETTINGS;
				return;
			}
		}
		else if(m_element[i]->m_callbackID == "close_message") 
		{
			if(m_element[i]->callBack() == SELECTED) 
			{
				m_activeGameMode = CLOSE_MESSAGE;

				return;
			}
		}
	}

	setActiveStates();

	m_activeGameMode = MENU_LOOP; //if all else fails saty in the menu

}

void CMenu::outputCursorPos()
{
	int mx = user::getInstance()->input.mousex;
	int my = user::getInstance()->input.mousey;

	COutput display;

	display.output(20,20,"Mouse X :",mx);
	display.output(20,30,"Mouse Y :",my);

}

CMainMenu::CMainMenu()
{
	m_numberOfSubMenus = 0;
	m_activeSubMenu = -1;

	CTextureMan::getInstance()->addMenuTexture("data/menu/background.bmp");
	dropBackID =CTextureMan::getInstance()->addMenuTexture("data/menu/container.bmp");

	closeButtonID = CTextureMan::getInstance()->addMenuTexture("data/menu/x.bmp");

	boxID = CTextureMan::getInstance()->addMenuTexture("data/menu/dropdown_textbox.bmp");
	dropButtonID = CTextureMan::getInstance()->addMenuTexture("data/menu/down_arrow.bmp");

	cursorID = CTextureMan::getInstance()->addMenuTexture("data/menu/cursor.bmp");

	textBoxID = CTextureMan::getInstance()->addMenuTexture("data/menu/text_box.bmp");

	mainBar = new CMenu(0);
	networkMenu = new CMenu(1);
	settingsMenu = new CMenu(2);
	creditsMenu = new CMenu(3);
	controlsMenu = new CMenu(4);
	messageBox = new CMenu(5);

	messageDisplay = false;
	networkConnected = false;
	netConnect = networkConnected; // a loose variable to access in the network sub menu
}

void CMainMenu::registerPlayer(CPlayer *player)
{
	m_player = player;
	m_profileMenu = new CProfileEditor(m_player);
}

void CMainMenu::draw()
{
	//background
	glPushMatrix();

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindMenuTexture(0);

	glColor3f(0.5,0.5,0.5);

	glTranslated(0,0,-1);

	int diff = xScreenSize - yScreenSize;

	glBegin(GL_QUADS);
		glTexCoord2f(-0.2,0); glVertex2f(0,0);
		glTexCoord2f(1.2,0); glVertex2f(xScreenSize,0);
		glTexCoord2f(1.2,1); glVertex2f(xScreenSize,yScreenSize);
		glTexCoord2f(-0.2,1); glVertex2f(0,yScreenSize);
	glEnd();

	int m_subMenuArrayPos = m_activeSubMenu + 10;

	if(m_activeSubMenu != -1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		glColor4f(0,0.39,0.56,0.9);

		CTextureMan::getInstance()->bindMenuTexture(1);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(xScreenSize - 100,75);
			glTexCoord2f(1,0); glVertex2f(xScreenSize - 400,75);
			glTexCoord2f(1,1); glVertex2f(xScreenSize - 400,520);
			glTexCoord2f(0,1); glVertex2f(xScreenSize - 100,520);
		glEnd();

		if((m_subMenuArrayPos == NETWORK_SUB) && (networkMenu->m_menuMode != 0))
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex2f(xScreenSize - 120,250);
				glTexCoord2f(1,0); glVertex2f(xScreenSize - 380,250);
				glTexCoord2f(1,1); glVertex2f(xScreenSize - 380,390);
				glTexCoord2f(0,1); glVertex2f(xScreenSize - 120,390);
			glEnd();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			COutput players;

			glColor4f(1,1,1,1);
			players.output(xScreenSize - 370,270,"PLAYERS","",18);
		}
	}

	glPopMatrix();


	mainBar->draw();

	switch(m_subMenuArrayPos)
	{
	case PROFILE_SUB:

		m_profileMenu->draw();
		

	break;

	case NETWORK_SUB:

		networkMenu->draw();

	break;

	case SETTINGS_SUB:

		settingsMenu->draw();

	break;

	case CREDITS_SUB:

		creditsMenu->draw();

	break;

	case CONTROLS_SUB:

		controlsMenu->draw();

	break;

	default :

		m_activeSubMenu = -1;

	break;

	}

	if(messageDisplay)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		CTextureMan::getInstance()->bindMenuTexture(1);

		glColor4f(0,0.39,0.56,0.9);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f((xScreenSize / 2) - 200,(yScreenSize / 2) - 200);
			glTexCoord2f(1,0); glVertex2f((xScreenSize / 2) + 200,(yScreenSize / 2) - 200);
			glTexCoord2f(1,1); glVertex2f((xScreenSize / 2) + 200,(yScreenSize / 2));
			glTexCoord2f(0,1); glVertex2f((xScreenSize / 2) - 200,(yScreenSize / 2));
		glEnd();

		messageBox->draw();
	}

	//drawCursor();

	if(m_subMenuArrayPos == PROFILE_SUB) m_profileMenu->renderModel(m_player);


}

void CMainMenu::drawCursor()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	CTextureMan::getInstance()->bindMenuTexture(cursorID);

	int mx = user::getInstance()->input.mousex;
	int my = user::getInstance()->input.mousey;

	glColor3f(1,1,1);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(mx,my);
		glTexCoord2f(1,0); glVertex2f(mx + 25,my);
		glTexCoord2f(1,1); glVertex2f(mx + 25,my + 25);
		glTexCoord2f(0,1); glVertex2f(mx,my + 25);
	glEnd();

}

void CMainMenu::check()
{

	computerName = networkMenu->computerName;

	if(messageDisplay)
	{
		messageBox->check();
	}	

	int m_subMenuArrayPos = m_activeSubMenu + 10;

	mainBar->check();

	switch(m_subMenuArrayPos)
	{
		case PROFILE_SUB:
		{

			m_profileMenu->check();

			int prof = m_profileMenu->callback();

			if(prof == NULL_SUB) 
			{
				m_activeSubMenu = NULL_SUB;
			}

		}
		break;

		case NETWORK_SUB:

			networkMenu->check();
			if(networkMenu->callback() == NULL_SUB)
			{
				m_activeSubMenu = NULL_SUB;
			}

		break;

		case SETTINGS_SUB:

			settingsMenu->check();
			//check to switch sub menu

			if(settingsMenu->callback() == NULL_SUB) m_activeSubMenu = NULL_SUB;

		break;

		case CREDITS_SUB:

			creditsMenu->check();
			if(creditsMenu->callback() == NULL_SUB) m_activeSubMenu = NULL_SUB;

		break;


		case CONTROLS_SUB:

			controlsMenu->check();
			if(controlsMenu->callback() == NULL_SUB) m_activeSubMenu = NULL_SUB;

		break;

	}

	netConnect = networkConnected; // a loose variable to access in the network sub menu

	inNetworkLobby = inLobby;
}

int CMainMenu::callback()
{
	//sub menu controller
	int res = mainBar->callback();
	int net = networkMenu->callback();
	int msg = messageBox->callback();
	int set = settingsMenu->callback();
	int prof = m_profileMenu->callback();

	if(msg == CLOSE_MESSAGE) 
	{
		messageDisplay = false;
	}

	if((net == NETWORK_JOIN) || (net == NETWORK_HOST) || (net == NETWORK_GAME_START)) 
	{
		if(net == NETWORK_JOIN) networkConnected = true;

		clearPlayerList();
		return net;
	}
	else if(net == WAIT_FOR_PLAYERS_MESSAGE)
	{
		addMessage("Waiting for another player","before starting the game");
	}

	if(set == WRITE_SETTINGS)
	{
		ofstream fout;

		fout.open("Data\\Settings\\settings.dat");

		fout << "settings";
		fout << "\nresolution " << settingsMenu->m_element[0]->m_contents << " " << settingsMenu->m_element[0]->m_chosenItem;
		fout << "\nshadow_quality " << settingsMenu->m_element[1]->m_contents << " " << settingsMenu->m_element[1]->m_chosenItem;
		fout << "\ntexture_quality " << settingsMenu->m_element[2]->m_contents << " " << settingsMenu->m_element[2]->m_chosenItem;
	
		addMessage("Your new settings will be applied","next time you restart the game");
	}

	if(prof == PROFILE_APPLIED_MESSAGE)
	{
		addMessage("You profile information has been updated","");
	}

	if(user::getInstance()->input.specialKey[4]) return EDITOR_LOOP;

	if((res == GAME_LOOP) || (res == EDITOR_LOOP) || (res == EXIT_GAME))
	{
		//need to change the main games state so return
		m_activeSubMenu = NULL_SUB;
		return res;
	}
	else if(res == MENU_LOOP)
	{
		return MENU_LOOP;
	}
	else
	{
		m_activeSubMenu = res - 10;

		return MENU_LOOP;
	}
}

void CMainMenu::setNetworkHostName(const char *name)
{
	strcpy(networkMenu->m_element[0]->m_contents,name);
}

void CMainMenu::addToNetworkPlayerList(CPlayer *player)
{
	//ADD PLAYER ONE NAME
	for(int i = 0; i < networkMenu->m_numberOfElements; i++)
	{
		CFormElement *element = networkMenu->m_element[i];

		if(element->m_callbackID == "player1_name")
		{
			if(!element->m_active) 
			{
				element->m_active = true;
				element->setText(player->m_playerName);
				return;
			}
		}
	}

	//IF WE GET THIS FAR ADD PLAYER TWO NAME
	for(i = 0; i < networkMenu->m_numberOfElements; i++)
	{
		CFormElement *element = networkMenu->m_element[i];

		if(element->m_callbackID == "player2_name")
		{
			if(!element->m_active) 
			{
				element->m_active = true;
				element->setText(player->m_playerName);
				return;
			}
		}
	}
}

void CMainMenu::clearPlayerList()
{
	for(int i = 0; i < networkMenu->m_numberOfElements; i++)
	{
		CFormElement *element = networkMenu->m_element[i];

		if((element->m_callbackID == "player2_name") 
			|| (element->m_callbackID == "player1_name"))
		{
			element->m_active = false; 
		}
	}
}

void CMainMenu::addMessage(char *messageLine1, char *messageLine2)
{
	messageBox->m_element[0]->setText(messageLine1);
	messageBox->m_element[1]->setText(messageLine2);
	messageDisplay = true;
}

CMenu::~CMenu()
{

}

void CMainMenu::setInLobby(bool b)
{
	inLobby = b;
}