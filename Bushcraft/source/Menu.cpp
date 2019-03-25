#include <string>
#include "Menu.h"

using namespace std;

CMenu::CMenu() {

	m_buffer = NULL;
	m_totalItem = 0;
	m_itembkground = NULL;
	m_itemText = NULL;
	m_returnValue = NULL;
	m_maxTotalItem = 0;
	m_title = NULL;

	m_binitialised = false;

}


CMenu::~CMenu() {

	reset();

}

void CMenu::reset() {

	if (m_buffer != NULL) {
		destroy_bitmap(m_buffer);
		m_buffer = NULL;
	}

	if (m_title != NULL) {
		delete m_title;
		m_title = NULL;
	}

	if (m_itembkground != NULL) {
		for (unsigned int i=0;i<m_totalItem;i++) {
			if (m_itembkground[i]!=NULL) {
				delete m_itembkground[i];
				m_itembkground[i]=NULL;
			}
		}

		delete m_itembkground;
		m_itembkground=NULL;
	}

	if (m_itemText != NULL) {
		for (unsigned int i=0;i<m_totalItem;i++) {
			if (m_itemText[i]!=NULL) {
				delete m_itemText[i];
				m_itemText[i]=NULL;
			}
		}

		delete m_itemText;
		m_itemText=NULL;
	}

	if (m_returnValue != NULL) {
		delete m_returnValue;
		m_returnValue = NULL;
	}

	m_totalItem = 0;

	m_binitialised = false;

}


bool CMenu::Initialise(const char title[], unsigned int maximumnumberofitem, int textcol) {

	reset();

	m_itembkground = new char* [maximumnumberofitem];
	m_itemText = new char* [maximumnumberofitem];
	m_returnValue = new int[maximumnumberofitem];

	for (unsigned int i=0;i<maximumnumberofitem;i++) {
		m_itembkground[i] = NULL;
		m_itemText[i] = NULL;
	}

	m_maxTotalItem = maximumnumberofitem;

	m_title = new char[512];

	strcpy(m_title, title);

	m_binitialised = true;

	m_textColor = textcol;

	return true;
}

void CMenu::AddMenuItem(const char itembkground[], const char itemtext[], int returnvalue) {

	if (!m_binitialised) {
		allegro_message("This menu is not yet initialised");
		return;
	}

	if (m_totalItem>=m_maxTotalItem) {
		allegro_message("The maximum number of menu item has been exceeded");
		return;
	}

	m_itembkground[m_totalItem] = new char[256];
	if (itembkground!=NULL)
		strcpy(m_itembkground[m_totalItem],itembkground);
	else
		m_itembkground[m_totalItem] = NULL;

	m_itemText[m_totalItem] = new char[256];
	if (itemtext!=NULL)
		strcpy(m_itemText[m_totalItem], itemtext);
	else
		m_itemText[m_totalItem] = NULL;

	m_returnValue[m_totalItem] = returnvalue;

	m_totalItem++;

}

int CMenu::DoModal() {

	if (!m_binitialised) {
		allegro_message("This menu is not yet initialised");
		return -1;
	}

	if (m_totalItem<=0) {
		allegro_message("There is no item in this menu");
		return -1;
	}

	int windowwidth = 2*EDGEWIDTH + ITEMWIDTH;
	int windowheight = 2*ITEMSPACING + ITEMHEIGHT*m_totalItem +ITEMSPACING*(m_totalItem-1);

	// First set the title of the window we would like to create
	set_window_title(m_title);

	// Then set the colour depth in bits
	set_color_depth(32);
	
	if ( set_gfx_mode( GFX_AUTODETECT_WINDOWED, windowwidth, windowheight , 0 , 0 ) < 0 )	{
		allegro_message( "Unable initialize graphics module\n % s\n", allegro_error );
		return  false;
	}

	set_window_close_button(0);
	text_mode(-1);

	m_buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear_to_color(m_buffer, 0);
	show_mouse(screen);

	int xtopleft = EDGEWIDTH;
	int* ytopleft  = new int[m_totalItem];
	ytopleft[0] = ITEMSPACING;

	for (int i=0;i<m_totalItem;i++) {
		if (m_itembkground[i] != NULL) {
			BITMAP *temp = load_bitmap(m_itembkground[i],0);
			stretch_blit(temp, m_buffer, 0,0,temp->w,temp->h, xtopleft,ytopleft[i],ITEMWIDTH,ITEMHEIGHT);
			destroy_bitmap(temp);
		}
		if (m_itemText[i] != NULL)
			textout_centre(m_buffer, font, m_itemText[i], xtopleft+ITEMWIDTH/2, ytopleft[i]+ITEMHEIGHT/2, m_textColor);

		if (i<m_totalItem-1)
			ytopleft[i+1]=ytopleft[i]+ITEMSPACING + ITEMHEIGHT;

	}

	scare_mouse();
	blit(m_buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
	unscare_mouse();

	int oldmousex = 0;
	int oldmousey = 0;

	int lasthighlighted = -1;
	int currenthighlighted = -1;

	int whitecolor = makecol(255,255,255);

	bool chosen = false;

	do {

		if ((mouse_x != oldmousex)||(mouse_y != oldmousey)) {

			if ((mouse_x>=xtopleft)&&(mouse_x<=xtopleft+ITEMWIDTH)) {
				
				for (i=0;i<m_totalItem;i++) {

					if ((mouse_y>=ytopleft[i])&&(mouse_y<=ytopleft[i]+ITEMHEIGHT)) {
						currenthighlighted = i;
						break;
					}
				}

				if (currenthighlighted!=lasthighlighted) {
					scare_mouse();
					if (lasthighlighted!=-1)
						rect(screen, xtopleft, ytopleft[lasthighlighted], xtopleft+ITEMWIDTH,ytopleft[lasthighlighted]+ITEMHEIGHT, 0);
					if (currenthighlighted!=-1)
						rect(screen, xtopleft, ytopleft[currenthighlighted], xtopleft+ITEMWIDTH,ytopleft[currenthighlighted]+ITEMHEIGHT, whitecolor);
					unscare_mouse();
					lasthighlighted = currenthighlighted;
				}

			}
		}

		if (mouse_b && (currenthighlighted!=-1))
			chosen = true;

		oldmousex = mouse_x;
		oldmousey = mouse_y;

	} while (!chosen);

	delete ytopleft;

	return m_returnValue[currenthighlighted];

}







