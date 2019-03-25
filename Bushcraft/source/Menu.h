#include <allegro.h>


#define ITEMWIDTH	200
#define ITEMHEIGHT	50
#define ITEMSPACING	5
#define EDGEWIDTH	60	

class CMenu {

protected:
	char *m_title;
	BITMAP *m_buffer;
	unsigned int m_totalItem;
	char **m_itembkground;
	char **m_itemText;
	int* m_returnValue;
	unsigned int m_maxTotalItem;
	bool m_binitialised;
	void reset();
	int m_textColor;

public:

	CMenu();
	~CMenu();
	bool Initialise(const char title[], unsigned int maximumnumberofitem, int textcol);
	void AddMenuItem(const char itembkground[], const char itemtext[], int returnvalue);
	int DoModal();
};