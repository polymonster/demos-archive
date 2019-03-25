#ifndef INC_BACKGROUND_H
#define INC_BACKGROUND_H

#include <allegro.h>

class CBackground {
protected:
	bool m_binitialised;
	BITMAP *m_backgroundimage;
	int m_imageWidth, m_imageHeight;
	int m_windowWidth, m_windowHeight;
	int m_screenTopborder, m_screenLeftborder, m_screenWidth, m_screenHeight;
	bool m_bwrapx, m_bwrapy;
	double m_dx, m_dy;

	bool m_bcancheckCollision;
	unsigned char* m_collisionmap;

	int m_topleftx, m_toplefty;

	unsigned char* m_grid;
	int m_tilesperrow, m_tilespercol, m_tilewidth, m_tileheight;

public:
	CBackground();
	~CBackground();
	int initialise(const char filename[], const char collisionmapfilename[], int windowWidth, int windowHeight, int screenleft, int screentop, int screenwidth, int screenheight);
	int initialise(const char filename[], int windowWidth, int windowHeight, int screenleft, int screentop, int screenwidth, int screenheight);
	void drawBackground(BITMAP* buffer, int centrex, int centrey, bool transparent);
	BITMAP* getbackgroundimage() { return m_backgroundimage; };
	int getScreencoordinate(int xworldpos, int yworldpos, int* xscreenpos, int* yscreenpos);
	bool checkCollision(int* xcoord, int* ycoord, int numberofcoordinates);
	void getDimension(int *backgroundWidth, int *backgroundHeight);
	void drawMiniMap(BITMAP* buffer, int screenx, int screeny, int width, int height);
	bool getScreentoWindowRatio(double* dx, double* dy);
	bool getVisibleborder(int* top, int* left, int* bottom, int* right);
	bool getScreenborder(int* left, int* top, int* right, int* bottom);
	unsigned char * getCollisionGrid(int* tilesperrow, int* tilespercol, int* tilewidth, int* tileheight);
	int getMousecoordinate(int *mouseworldx, int *mouseworldy);

};

#endif /* INC_BACKGROUND_H */

