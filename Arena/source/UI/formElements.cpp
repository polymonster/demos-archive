#include "menu.h"
#include "textureSingleton.h"
#include "HCI.h"
#include "outputText.h"

COutput text;

CFormElement::CFormElement(string callbackID,int x, int y, int width, int height, bool center,int iconID)
{
	//set the call back ID to identify it by in the menu
	//and the texture ID to bin to it
	m_callbackID = callbackID;
	m_iconID = iconID;

	//send data to create a rectangle
	calculateRect(x,y,width,height,center);

	m_state = 0;
	m_selectedState = false;
	m_active = true;
}

void CFormElement::calculateRect(int x, int y, int width, int height, bool center)
{
	if(!center)
	{
		//create a right aligned rectangle
		m_rect.topRight.x = x;
		m_rect.topRight.y = y;

		m_rect.topLeft.x = x + width;
		m_rect.topLeft.y = y;

		m_rect.bottomRight.x = x;
		m_rect.bottomRight.y = y + height;

		m_rect.bottomLeft.x = x + width;
		m_rect.bottomLeft.y = y + height;
	}
	else
	{
		//create the rectangle centered at (x,y)
		SVector2 halfSize;

		halfSize.x = x / 2;
		halfSize.y = y / 2;

		m_rect.topRight.x = x - halfSize.x;
		m_rect.topRight.y = y - halfSize.y;

		m_rect.topLeft.x = x + halfSize.x;
		m_rect.topLeft.y = y - halfSize.y;


		m_rect.bottomRight.x = x - halfSize.x;
		m_rect.bottomRight.y = y + halfSize.y;


		m_rect.bottomLeft.x = x + halfSize.x;
		m_rect.bottomLeft.y = y + halfSize.y;

	}
}

void CFormElement::addItem(const char *name){};
void CFormElement::setInitialValue(int id){};

void CFormElement::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glColor4f(0,0.39,0.56,0.9);

	if(m_state == MOUSE_OVER) glColor4f(0.56,0,0.2,0.9);

	glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindMenuTexture(m_iconID);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(m_rect.topRight.x,m_rect.topRight.y);
		glTexCoord2f(1,0); glVertex2f(m_rect.topLeft.x,m_rect.topLeft.y);
		glTexCoord2f(1,1); glVertex2f(m_rect.bottomLeft.x,m_rect.bottomLeft.y);
		glTexCoord2f(0,1); glVertex2f(m_rect.bottomRight.x,m_rect.bottomRight.y);
	glEnd();
}

bool CFormElement::check()
{
	SVector2 mousePos;

	mousePos.x = user::getInstance()->input.mousex;
	mousePos.y = user::getInstance()->input.mousey;

	m_state = BLANK;

	if((mousePos.x > m_rect.topRight.x) && (mousePos.y > m_rect.topRight.y)
		&& (mousePos.x < m_rect.bottomLeft.x) && (mousePos.y < m_rect.bottomLeft.y))
	{
		m_state = MOUSE_OVER;

		if(user::getInstance()->input.leftMouse)
		{
			m_state = SELECTED;

			user::getInstance()->input.leftMouse = false;

			//m_selectedState = !m_selectedState;

			return true;
		}
	}

	return false;
}

CEditBox::CEditBox(string callbackID,int x, int y, int width, int height, bool center,int arraySize,int iconID)
{
	m_characterPos = 0;
	m_nextCharacter = 0;

	m_contents = new char[arraySize];
	strcpy(m_contents,"");

	m_callbackID = callbackID;
	m_iconID = iconID;

	//send data to create a rectangle
	calculateRect(x,y,width,height,center);

	m_state = 0;
	m_selectedState = false;
	
	m_arraySize = arraySize;

	m_active = true;

	dontCheck = false;
}

void CEditBox::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glColor4f(0,0.39,0.56,0.9);
	if(m_state == MOUSE_OVER) glColor4f(0.56,0,0.2,0.9);
	if(m_selectedState) glColor3f(1,0,1);

	glEnable(GL_TEXTURE_2D);
	CTextureMan::getInstance()->bindMenuTexture(m_iconID);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(m_rect.topRight.x,m_rect.topRight.y);
		glTexCoord2f(1,0); glVertex2f(m_rect.topLeft.x,m_rect.topLeft.y);
		glTexCoord2f(1,1); glVertex2f(m_rect.bottomLeft.x,m_rect.bottomLeft.y);
		glTexCoord2f(0,1); glVertex2f(m_rect.bottomRight.x,m_rect.bottomRight.y);
	glEnd();


	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	SVector2 centerText;

	centerText.x = m_rect.topRight.x + 2;
	centerText.y = m_rect.topRight.y + ((m_rect.bottomRight.y - m_rect.topRight.y) / 2);

	glColor4f(1,1,1,1);
	text.output(centerText.x,centerText.y,"",m_contents,18);
}

bool CEditBox::check()
{
	if(!dontCheck)
	{
		SVector2 mousePos;

		mousePos.x = user::getInstance()->input.mousex;
		mousePos.y = user::getInstance()->input.mousey;

		m_state = BLANK;

		if(m_selectedState) getUserInput();

		if((mousePos.x > m_rect.topRight.x) && (mousePos.y > m_rect.topRight.y)
			&& (mousePos.x < m_rect.bottomLeft.x) && (mousePos.y < m_rect.bottomLeft.y))
		{
			m_state = MOUSE_OVER;

			if(user::getInstance()->input.leftMouse)
			{
				m_state = SELECTED;
				m_nextCharacter = strlen(m_contents);
				user::getInstance()->input.leftMouse = false;
				m_selectedState = !m_selectedState;
				return true;
			}
		}
	}

	return false;
}

void CEditBox::getUserInput()
{
	for(int i = 32; i < 127; i++)
	{
		if(user::getInstance()->input.key[i])
		{
			if(m_nextCharacter < m_arraySize)
			{
				m_contents[m_nextCharacter++] = i;
				user::getInstance()->input.key[i] = false;

				m_contents[m_nextCharacter] = 0;
			}
		}
	}

	//BACKSPACE
	if(user::getInstance()->input.key[8])
	{
		m_nextCharacter--;

		if(m_nextCharacter >= 0)
		{
			m_contents[m_nextCharacter] = 0;
			user::getInstance()->input.key[8] = false;
		}
		else if(m_nextCharacter < 0)
		{
			m_nextCharacter = 1;
		}
	}

	//DELETE
	if(user::getInstance()->input.key[8])
	{
		m_contents[m_nextCharacter] = 0;
		user::getInstance()->input.key[8] = false;

		m_nextCharacter--;
	}

	//TAB
	if(user::getInstance()->input.key[11])
	{
		if(m_selectedState) m_selectedState = false;
	}	
}

CDropDownBox::CDropDownBox(string callbackID,int x, int y, int width, int height, bool center,int arraySize, int bgId, int buttonId, int dropId)
{
	m_callbackID = callbackID;
	//m_iconID = iconID;
	m_boxID = bgId;
	m_buttonID = buttonId;
	m_dropID = dropId;

	//send data to create a rectangle
	calculateRect(x,y,width,height,center);

	m_state = 0;
	m_selectedState = false;
	
	m_chosenItem = 0;

	m_numberOfItems = 0;

	m_active = true;

}

void CDropDownBox::addItem(const char *name)
{
	menuItems[m_numberOfItems] = new char[sizeof(name)];
	strcpy(menuItems[m_numberOfItems++],name);
}

void CDropDownBox::draw()
{
	glPushMatrix();

	glTranslated(0,0,-1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		glColor4f(0,0.39,0.56,0.9);

		glEnable(GL_TEXTURE_2D);
		CTextureMan::getInstance()->bindMenuTexture(m_boxID);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(m_rect.topRight.x,m_rect.topRight.y);
			glTexCoord2f(1,0); glVertex2f(m_rect.topLeft.x,m_rect.topLeft.y);
			glTexCoord2f(1,1); glVertex2f(m_rect.bottomLeft.x,m_rect.bottomLeft.y);
			glTexCoord2f(0,1); glVertex2f(m_rect.bottomRight.x,m_rect.bottomRight.y);
		glEnd();

		if(m_state == MOUSE_OVER) glColor4f(0.56,0,0.2,0.9);
		if(m_selectedState) glColor3f(1,0,1);

		CTextureMan::getInstance()->bindMenuTexture(m_buttonID);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(m_rect.topLeft.x + 30,m_rect.topRight.y);
			glTexCoord2f(1,0); glVertex2f(m_rect.topLeft.x,m_rect.topLeft.y);
			glTexCoord2f(1,1); glVertex2f(m_rect.bottomLeft.x,m_rect.bottomLeft.y);
			glTexCoord2f(0,1); glVertex2f(m_rect.bottomLeft.x + 30,m_rect.bottomRight.y);
		glEnd();

		glColor3f(1,1,1);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	COutput items;

	glColor4f(1,1,1,1);

	int ypos = (m_rect.bottomLeft.y - m_rect.topLeft.y) / 2;

	items.output(m_rect.topRight.x + 2,m_rect.topLeft.y + ypos + 5,"",menuItems[m_chosenItem],18);

	glPopMatrix();

	if(m_selectedState)
	{
		int size = 18;
		int y = m_rect.bottomLeft.y;

		glColor4f(0,0.39,0.56,0.9);

		glEnable(GL_TEXTURE_2D);

		CTextureMan::getInstance()->bindMenuTexture(m_dropID);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(m_rect.topRight.x,y);
			glTexCoord2f(1,0); glVertex2f(m_rect.topLeft.x,y);
			glTexCoord2f(1,1); glVertex2f(m_rect.bottomLeft.x,y + (size * m_numberOfItems) + size);
			glTexCoord2f(0,1); glVertex2f(m_rect.bottomRight.x,y + (size * m_numberOfItems) + size);
		glEnd();

		y += size / 2;

		for(int i = 0; i < m_numberOfItems; i++)
		{
			COutput items;

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			glColor3f(1,1,1);

			if(m_mouseOverItem == i) glColor3f(1,0,0);

			items.output(m_rect.topRight.x + 2,y + (size/2),"",menuItems[i]);

			y += size;
		}
	}
}

void CDropDownBox::setInitialValue(int id)
{
	m_chosenItem = id;
	m_contents = menuItems[m_chosenItem];
}

bool CDropDownBox::check()
{
	SVector2 mousePos;

	mousePos.x = user::getInstance()->input.mousex;
	mousePos.y = user::getInstance()->input.mousey;

	m_state = BLANK;

	if((mousePos.x < m_rect.topLeft.x + 40) && (mousePos.y > m_rect.topRight.y)
		&& (mousePos.x > m_rect.bottomLeft.x) && (mousePos.y < m_rect.bottomLeft.y))
	{
		m_state = MOUSE_OVER;

			if(user::getInstance()->input.leftMouse)
			{
				m_state = SELECTED;

				user::getInstance()->input.leftMouse = false;
				m_selectedState = !m_selectedState;

				return true;
			}
	}

	if(m_selectedState)
	{
		m_mouseOverItem = -1;

		int y = m_rect.bottomLeft.y;
		int size = 18;

		for(int i = 0; i < m_numberOfItems; i++)
		{
			if((mousePos.x > m_rect.topRight.x) && (mousePos.y > y)
				&& (mousePos.x < m_rect.bottomLeft.x) && (mousePos.y < y + size))
			{

				m_mouseOverItem = i;

				if(user::getInstance()->input.leftMouse)
				{
					m_chosenItem = i;
					m_contents = menuItems[m_chosenItem];
					user::getInstance()->input.leftMouse = false;
					m_selectedState = !m_selectedState;

					return true;
				}

			}
			else if((mousePos.x < m_rect.topRight.x) || (mousePos.y < y)
			|| (mousePos.x > m_rect.bottomLeft.x) || (mousePos.y > (y + size * m_numberOfItems)))
			{
				if(user::getInstance()->input.leftMouse)
				{
					m_selectedState = !m_selectedState;

					return true;
				}
			}

			y += size;
		}
	}

	return false;
}

CStaticText::CStaticText(string callbackID,int x, int y, int width, int height, bool center, int iconID)
{
	m_state = BLANK;
	//set the call back ID to identify it by in the menu
	//and the texture ID to bin to it
	m_callbackID = callbackID;
	m_iconID = iconID;

	//send data to create a rectangle
	calculateRect(x,y,width,height,center);

	if(iconID == 0) m_state = 18;
	else m_state = 12;

	m_selectedState = false;
	m_active = true;

}

void CStaticText::draw()
{
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	/*glColor4f(0,1,0,1);

	glBegin(GL_QUADS);
		glVertex2f(m_rect.topRight.x,m_rect.topRight.y);
		glVertex2f(m_rect.topLeft.x,m_rect.topLeft.y);
		glVertex2f(m_rect.bottomLeft.x,m_rect.bottomLeft.y);
		glVertex2f(m_rect.bottomRight.x,m_rect.bottomRight.y);
	glEnd();*/

	
	COutput text;

	glColor4f(1,1,1,1);

	int ypos = (m_rect.topLeft.y - m_rect.bottomLeft.y) / 2;
	text.output(m_rect.topRight.x + 2,m_rect.topLeft.y - ypos,"",m_contents,m_state);
}

void CFormElement::setText(char *text)
{
	m_contents = new char[64];

	strcpy(m_contents,text);
}


setText(char *text);

int CFormElement::callBack()
{
	//if(m_state == SELECTED) return m_state = BLANK;
	return m_state;

}