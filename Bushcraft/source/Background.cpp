#include <fstream>
#include "Background.h"
#include "pbmlibrary.h"
#include "Node.h"

using namespace std;

CBackground::CBackground() {

	m_binitialised = false;
	m_backgroundimage = NULL;
	m_imageWidth = 0; 
	m_imageHeight = 0;
	m_windowWidth = 0; 
	m_windowHeight = 0;
	m_collisionmap = NULL;
	m_bcancheckCollision = false;
	m_grid = NULL;
	
}

CBackground::~CBackground() {

	if (m_backgroundimage != NULL)
		destroy_bitmap(m_backgroundimage);
	m_backgroundimage = NULL;

	if (m_collisionmap != NULL)
		delete m_collisionmap;
	m_collisionmap = NULL;

	if (m_grid!=NULL)
		delete m_grid;
	m_grid = NULL;

}

int CBackground::initialise(const char filename[], const char collisionmapfilename[], int windowWidth, int windowHeight, int screenleft, int screentop, int screenwidth, int screenheight) {

	if (m_binitialised) {

		allegro_message("The background has already been initialised");
		return 0;

	}


	m_backgroundimage = load_bitmap(filename,0);

	if (m_backgroundimage == NULL) {
		return 0;
	}

	m_imageWidth = m_backgroundimage->w;
	m_imageHeight = m_backgroundimage->h;

	int tempw, temph;
	bool tempcolor;

	char msg[256];

	if (collisionmapfilename!=NULL) {

		m_collisionmap = ReadPBM(collisionmapfilename, &tempw, &temph, &tempcolor, msg);

		if ((m_collisionmap==NULL)||(m_imageWidth!=tempw)||(m_imageHeight!=temph)) {
			allegro_message(msg);
			delete m_collisionmap;
			m_collisionmap = NULL;
			destroy_bitmap(m_backgroundimage);
			m_backgroundimage = NULL;

			return 0;
		}

		m_bcancheckCollision = true;

	} else {

		m_bcancheckCollision = false;
		m_collisionmap = NULL;

	}



	if ((m_imageWidth<windowWidth)||(m_imageHeight<windowHeight)) {

		allegro_message("Visible window dimension can not be larger than the background image dimension");
		destroy_bitmap(m_backgroundimage);
		m_backgroundimage = NULL;

		return 0;

	}

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_binitialised = true;

	m_screenTopborder = screentop;
	m_screenLeftborder = screenleft;
	m_screenWidth = screenwidth;
	m_screenHeight = screenheight;

	m_dx = (double) m_screenWidth/m_windowWidth;
	m_dy = (double) m_screenHeight/m_windowHeight;

	m_topleftx = 0; 
	m_toplefty = 0;

	return 1;

}

int CBackground::initialise(const char filename[], int windowWidth, int windowHeight, int screenleft, int screentop, int screenwidth, int screenheight) {

	// TILES_PER_ROW	TILES_PER_COLOUMN
	// WIDTH_OF_TILE	HEIGHT_OF_TILE
	// TOTAL_NUMBER_OF_TILE_TYPE
	// TILE_COLLISION_ID (0 BLANK AND 1 COLLISION)
	// FILENAME
	// MAP

	if (m_binitialised) {

		allegro_message("The background has already been initialised");
		return 0;

	}

	ifstream fileIn;
	char next = 0;
	char buf[1024];

	fileIn.open(filename);

	// Reading the first value THE NUMBER OF TILES IN EACH ROW

	fileIn.getline(buf,100,' ');

	int tiles_per_row = atoi(buf);

	if (tiles_per_row<=0) {
		allegro_message("The stated number of tiles per row is too small");
		fileIn.close();
		return 0;
	}

	// Reading the second value THE NUMBER OF TILES IN EACH COLOUMN

	fileIn.getline(buf,100,'\n');

	int tiles_per_col = atoi(buf);

	if (tiles_per_col<=0) {
		allegro_message("The stated number of tiles per coloumn is too small");
		fileIn.close();
		return 0;
	}

	// Reading the third value THE WIDTH OF THE TILES

	fileIn.getline(buf,100,' ');

	int tiles_width = atoi(buf);

	if (tiles_width<=0) {
		allegro_message("The stated tiles width is too small");
		fileIn.close();
		return 0;
	}

	// Reading the fourth value THE HEIGHT OF THE TILES

	fileIn.getline(buf,100,'\n');

	int tiles_height = atoi(buf);

	if (tiles_height<=0) {
		allegro_message("The stated tiles height is too small");
		fileIn.close();
		return 0;
	}

	// Reading the fifth value THE NUMBER OF TILES

	fileIn.getline(buf,100,'\n');

	int tiles_number = atoi(buf);

	if (tiles_number<=0) {
		allegro_message("The stated tiles number is too small");
		fileIn.close();
		return 0;
	}

	// Reading the sixth value THE TILE COLLISION ID

	unsigned char* tile_collisionid = new unsigned char [tiles_number];
	fileIn.getline(buf,100,'\n');

	for (int i=0;i<tiles_number;i++) {
		tile_collisionid[i] = buf[i]-48;
	}

	fileIn.getline(buf,100,'\n');

	BITMAP* blockimage = load_bitmap(buf,0);

	if (blockimage == NULL) {
		return 0;
	}


	BITMAP** blocks = new BITMAP*[tiles_number];

	for (i=0; i<tiles_number; i++) {

		blocks[i] = create_bitmap(tiles_width, tiles_height);
		blit(blockimage, blocks[i], i*tiles_width, 0, 0, 0, tiles_width, tiles_height);

	}

	destroy_bitmap(blockimage);

	m_imageWidth = tiles_per_row*tiles_width;
	m_imageHeight = tiles_per_col*tiles_height;

	m_backgroundimage = create_bitmap(m_imageWidth, m_imageHeight);
	m_collisionmap = new unsigned char[m_imageWidth*m_imageHeight];
	for (i=0;i<m_imageWidth*m_imageHeight;i++)
		m_collisionmap[i]=0;

	unsigned char* mask = new unsigned char[tiles_width];
	for (i=0;i<tiles_width;i++)
		mask[i]=255;

	m_grid = new unsigned char[tiles_per_col*tiles_per_row];
	for (i=0;i<tiles_per_col*tiles_per_row;i++)
		m_grid[i]=NODENOTHING;

	int idx;

	for (i=0;i<tiles_per_col;i++) {
		fileIn.getline(buf,1024,'\n');
		for (int j=0;j<tiles_per_row;j++) {
			idx = -1;
			if ((buf[j]>='0')&&(buf[j]<='9'))
				idx = (int) buf[j]-'0';
			else if ((buf[j]>='A')&&(buf[j]<='Z'))
				idx = (int) buf[j]-'A';

			if ((idx>=0)&&(idx<tiles_number)) {
				blit(blocks[idx],m_backgroundimage,0,0,j*tiles_width,i*tiles_height,tiles_width, tiles_height);
				if (tile_collisionid[idx]) {
					int rowstart=i*tiles_height;
					int rowend=(i+1)*tiles_height;
					for (int r=rowstart;r<rowend;r++)
						memcpy(m_collisionmap+r*m_imageWidth+(j*tiles_width),mask,tiles_width);
					m_grid[i*tiles_per_row+j]=	NODEWALL;
				}
			}

		}
	}

	delete mask;
	m_tilesperrow = tiles_per_row; 
	m_tilespercol = tiles_per_col; 
	m_tilewidth = tiles_width;
	m_tileheight = tiles_height;

	for (i=0;i<tiles_number;i++)
		destroy_bitmap(blocks[i]);
	delete blocks;

	delete tile_collisionid;

	fileIn.close();

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_binitialised = true;

	m_screenTopborder = screentop;
	m_screenLeftborder = screenleft;
	m_screenWidth = screenwidth;
	m_screenHeight = screenheight;

	m_dx = (double) m_screenWidth/m_windowWidth;
	m_dy = (double) m_screenHeight/m_windowHeight;

	m_topleftx = 0; 
	m_toplefty = 0;

	m_bcancheckCollision = true;

	return 1;


}


void CBackground::drawBackground(BITMAP* buffer, int centrex, int centrey, bool transparent) {

	if (m_binitialised) {

			m_topleftx = centrex - m_windowWidth/2;
			if (m_topleftx < 0)
				m_topleftx = 0;

			int brx = centrex + m_windowWidth/2;
			if (brx >= m_imageWidth)
				m_topleftx = m_imageWidth - m_windowWidth;

			m_toplefty = centrey - m_windowHeight/2;
			if (m_toplefty < 0)
				m_toplefty = 0;

			int bry = centrey + m_windowHeight/2;
			if (bry >= m_imageHeight)
				m_toplefty = m_imageHeight - m_windowHeight;

			if (transparent)
				masked_stretch_blit(m_backgroundimage, buffer,m_topleftx,m_toplefty,m_windowWidth,m_windowHeight,m_screenLeftborder, m_screenTopborder, m_screenWidth, m_screenHeight);
			else
				stretch_blit(m_backgroundimage, buffer,m_topleftx,m_toplefty,m_windowWidth,m_windowHeight,m_screenLeftborder, m_screenTopborder, m_screenWidth, m_screenHeight);

	} else {

		allegro_message("The background has NOT been initialised");
		return;

	}
}

int CBackground::getScreencoordinate(int xworldpos, int yworldpos, int* xscreenpos, int* yscreenpos) {

	if (m_binitialised) {

		*xscreenpos = (int) ((xworldpos-m_topleftx)*m_dx + m_screenLeftborder);
		*yscreenpos = (int) ((yworldpos-m_toplefty)*m_dy + m_screenTopborder);

		return 1;

	} else {

		allegro_message("The background has NOT been initialised");
		return 0;

	}

}

int CBackground::getMousecoordinate(int *mouseworldx, int *mouseworldy) {

	if (m_binitialised) {


		*mouseworldx = (int) (m_topleftx + (mouse_x/m_dx) - m_screenLeftborder );
		*mouseworldy = (int) (m_toplefty + (mouse_y/m_dy) - m_screenTopborder);

		return 1;

	} else {

		allegro_message("The background has NOT been initialised");
		return 0;

	}

}

bool CBackground::checkCollision(int* xcoord, int* ycoord, int numberofcoordinates) {

	if (m_binitialised&&m_bcancheckCollision) {

		for (int i=0;i<numberofcoordinates;i++) {
			if ((xcoord[i]>=0)&&(xcoord[i]<m_imageWidth)&&(ycoord[i]>=0)&&(ycoord[i]<m_imageHeight)) {
				if (m_collisionmap[ycoord[i]*m_imageWidth+xcoord[i]]==255)
					return true;
			}
		}

		return false;

	}

	return false;

}

void CBackground::getDimension(int *backgroundWidth, int *backgroundHeight) {

	*backgroundWidth = m_imageWidth;
	*backgroundHeight = m_imageHeight;

}

void CBackground::drawMiniMap(BITMAP* buffer, int screenx, int screeny, int width, int height) {

	if (m_binitialised) {

		stretch_blit(m_backgroundimage,buffer,0,0,m_imageWidth,m_imageHeight,screenx, screeny, width, height);

		int tlx = ((double) m_topleftx)/m_imageWidth*width + screenx;
		int tly = ((double) m_toplefty)/m_imageHeight*height + screeny;
		int brx = tlx + ((double) m_windowWidth)/m_imageWidth*width;
		int bry = tly + ((double) m_windowHeight)/m_imageHeight*height;
		
		rect(buffer,tlx,tly,brx,bry,0);

	}

}

bool CBackground::getScreentoWindowRatio(double* dx, double* dy) {

	if (m_binitialised) {

		*dx = m_dx;
		*dy = m_dy;

		return true;

	} else

		return false;

}

bool CBackground::getVisibleborder(int* top, int* left, int* bottom, int* right) {

	if (m_binitialised) {
		*top = m_toplefty;
		*left = m_topleftx;
		*bottom = m_toplefty + m_windowHeight;
		*right = m_topleftx + m_windowWidth;
		return true;
	} else {
		*top = 0;
		*left = 0;
		*bottom = 0;
		*right = 0;
		return false;
	}
}

bool CBackground::getScreenborder(int* left, int* top, int* right, int* bottom) {

	if (m_binitialised) {
		*top = m_screenTopborder;
		*left = m_screenLeftborder;
		*bottom = m_screenTopborder+m_screenHeight;
		*right = m_screenLeftborder + m_screenWidth;
		return true;
	} else {
		*top = 0;
		*left = 0;
		*bottom = 0;
		*right = 0;
		return false;
	}
}

unsigned char * CBackground::getCollisionGrid(int* tilesperrow, int* tilespercol, int* tilewidth, int* tileheight) {

	if (m_grid==NULL)
		return NULL;

	unsigned char* temp = new unsigned char[m_tilesperrow*m_tilespercol];
	if (temp==NULL)
		return NULL;

	*tilesperrow = m_tilesperrow;
	*tilespercol = m_tilespercol;
	*tilewidth = m_tilewidth;
	*tileheight = m_tileheight;

	memcpy(temp,m_grid,m_tilesperrow*m_tilespercol);

	return temp;

}
