#include "map.h"
#include "modelManager.h"
#include "shader manager/shaderManager.h"
#include "timerSingleton.h"
#include "HCI.h"

void CMap::endStats()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	OrthoMode(0,0,xScreenSize,yScreenSize);

	SVector2 screenMiddle;
	screenMiddle.x = xScreenSize / 2;
	screenMiddle.y = yScreenSize / 2;

	glColor4f(0,0.39,0.56,0.9);

	CTextureMan::getInstance()->bindMenuTexture(1);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

	glEnable(GL_ALPHA_TEST);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(screenMiddle.x - 200,screenMiddle.y - 300);
		glTexCoord2f(1,0); glVertex2f(screenMiddle.x + 200,screenMiddle.y - 300);
		glTexCoord2f(1,1); glVertex2f(screenMiddle.x + 200,screenMiddle.y + 100);
		glTexCoord2f(0,1); glVertex2f(screenMiddle.x - 200,screenMiddle.y + 100);
	glEnd();

	glDisable(GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	for(int i = 0; i < 1; i++)
	{
		m_formElement[i]->draw();
		m_formElement[i]->check();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glColor4f(1,1,1,1);

	COutput stats;

	stats.output(screenMiddle.x - 190,screenMiddle.y - 270,"","GAME OVER",18);
	stats.output(screenMiddle.x - 190,screenMiddle.y - 250,"","STATS",18);

	int winner = 0;
	int loser = 1;
	bool tie = false;

	if(m_player[0]->kills() > m_player[1]->kills())
	{
		winner = 0;
		loser = 1;
	}
	else if(m_player[1]->kills() > m_player[0]->kills())
	{
		winner = 1;
		loser = 0;
	}
	else tie = true;

	if(!tie)
	{
		//YOU ARE THE WIENER
		stats.output(screenMiddle.x - 190,screenMiddle.y - 160,"","WINNER");

		stats.output(screenMiddle.x - 190,screenMiddle.y - 130,"","1");
		stats.output(screenMiddle.x - 175,screenMiddle.y - 130,"",m_player[winner]->m_playerName,18);

		stats.output(screenMiddle.x,screenMiddle.y - 160,"","Kills");
		stats.output(screenMiddle.x,screenMiddle.y - 130,"",m_player[winner]->kills(),18);


		//L-WHO-SA-HER
		stats.output(screenMiddle.x - 190,screenMiddle.y - 60,"","LOSER");

		stats.output(screenMiddle.x - 190,screenMiddle.y - 30,"","2");
		stats.output(screenMiddle.x - 175,screenMiddle.y - 30,"",m_player[loser]->m_playerName,18);

		stats.output(screenMiddle.x,screenMiddle.y - 60,"","Kills");
		stats.output(screenMiddle.x,screenMiddle.y - 30,"",m_player[loser]->kills(),18);
	}
	else
	{
		//YOU ARE THE WIENER
		stats.output(screenMiddle.x - 190,screenMiddle.y - 160,"","TIE GAME");

		stats.output(screenMiddle.x - 190,screenMiddle.y - 130,"","1");
		stats.output(screenMiddle.x - 175,screenMiddle.y - 130,"",m_player[0]->m_playerName,18);

		stats.output(screenMiddle.x,screenMiddle.y - 160,"","Kills");
		stats.output(screenMiddle.x,screenMiddle.y - 130,"",m_player[0]->kills(),18);


		//L-WHO-SA-HER
		stats.output(screenMiddle.x - 190,screenMiddle.y - 60,"","TIE GAME");

		stats.output(screenMiddle.x - 190,screenMiddle.y - 30,"","2");
		stats.output(screenMiddle.x - 175,screenMiddle.y - 30,"",m_player[1]->m_playerName,18);

		stats.output(screenMiddle.x,screenMiddle.y - 60,"","Kills");
		stats.output(screenMiddle.x,screenMiddle.y - 30,"",m_player[1]->kills(),18);

	}
}