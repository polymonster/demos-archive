#include <allegro.h>
#include <math.h>
#include "Node.h"

CNode::CNode() {
	m_parentNode = NULL;
	Fn=0;
	Gn=0;
	Hn=0;
}

CNode::~CNode() {


}

unsigned int CNode::calculateFn() {

	return (Gn+Hn);

}

unsigned int CNode::calculateHn(CNode* targetNode) {

	unsigned int a = (abs(targetNode->x-x) + abs(targetNode->y-y))*10;

	return a;

}

unsigned int CNode::calculateGn(CNode* neighNode) {

	unsigned int a = abs(neighNode->y-y);
	unsigned int b = abs(neighNode->y-y);

	if ((a>1)||(b>1)) {

		allegro_message("The node is nor neighbour");
		return 0;

	}

	if (a+b==1)
		return (10+neighNode->Gn);
	else
		return (14+neighNode->Gn);
}



/*

int main(void)
{

	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();

	set_window_title("Path finding demo");

	set_color_depth( 32 );
	
	if ( set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1024, 1024 , 0 , 0 ) < 0 )
	{
		allegro_message( "Unable initialize graphics module\n % s\n", allegro_error );
		return  -1;
	}

	set_window_close_button(0);


	int startx = 12;
	int starty = 12;
	int finishx = 1012;
	int finishy = 1012;
	int w = 20;

	int width = (finishx-startx)/w;
	int height = (finishy-starty)/w;

	int white = makecol(255,255,255);
	int black = 0;

	unsigned char* grid = new unsigned char[width*height];

	CNode** NodeArray = new CNode*[width*height];
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			int idx = y*width+x;
			NodeArray[idx]=new CNode();
			NodeArray[idx]->x=x;
			NodeArray[idx]->y=y;
		}
	}

	CNode** OpenList = new CNode*[width*height];

	int togglecolour[3];
	togglecolour[0] = makecol(255,0,0);
	togglecolour[1] = makecol(0,0,255);
	togglecolour[2] = 0;

	int startpointx, startpointy, finishpointx, finishpointy;

	for (int p=0;p<3;p++) {

		clear_to_color(screen, white);
		show_mouse(screen);

		for (int i=0;i<=50;i++)
			hline(screen,startx, i*w+starty, finishx, black);

		for (i=0;i<=50;i++)
			vline(screen,i*w+startx, starty, finishy, black);

		for (i=0;i<width*height;i++)
			grid[i]=0;

		int toggle = 0;

		bool startselected = false;
		bool finishselected = false;

		do {
			
			if (key[KEY_A])
				toggle = 0;
			else if (key[KEY_B])
				toggle = 1;
			else if (key[KEY_C])
				toggle = 2;

			if (mouse_b) {
				int x = (mouse_x - startx)/w;
				int y = (mouse_y - starty)/w;
				if (grid[y*width+x]==0) {
					scare_mouse();

					switch (toggle) {
					case 0:
						if (startselected) {
							rectfill(screen,startpointx*w+startx,startpointy*w+starty,(startpointx+1)*w+startx,(startpointy+1)*w+starty,makecol(255,255,255));
							rect(screen,startpointx*w+startx,startpointy*w+starty,(startpointx+1)*w+startx,(startpointy+1)*w+starty,0);
							grid[startpointy*width+startpointx]=0;
						} 

						rectfill(screen,x*w+startx,y*w+starty,(x+1)*w+startx,(y+1)*w+starty,togglecolour[toggle]);
						grid[y*width+x]=NODESTART;
						startpointx=x;
						startpointy=y;
						startselected = true;
						break;
					case 1:
						if (finishselected) {
							rectfill(screen,finishpointx*w+startx,finishpointy*w+starty,(finishpointx+1)*w+startx,(finishpointy+1)*w+starty,makecol(255,255,255));
							rect(screen,finishpointx*w+startx,finishpointy*w+starty,(finishpointx+1)*w+startx,(finishpointy+1)*w+starty,0);
							grid[startpointy*width+startpointx]=0;
						} 

						rectfill(screen,x*w+startx,y*w+starty,(x+1)*w+startx,(y+1)*w+starty,togglecolour[toggle]);
						grid[y*width+x]=NODEFINISH;
						finishpointx=x;
						finishpointy=y;
						finishselected = true;
						break;
					case 2:
						rectfill(screen,x*w+startx,y*w+starty,(x+1)*w+startx,(y+1)*w+starty,togglecolour[toggle]);
						grid[y*width+x]=NODEWALL;
						break;
					}

					unscare_mouse();
				}
			}

		} while (!key[KEY_ESC]);


		if (startselected&&finishselected) {

			int startidx = startpointy*width+startpointx;
			int finishidx = finishpointy*width+finishpointx;

			NodeArray[startidx]->Gn=0;

			int currentidx = startidx;
			int currentx = startpointx;
			int currenty = startpointy;

			int count = 0;		// length of open list starting with start point

			bool iterate = true;
			bool foundtarget = false;

			while (iterate&&!foundtarget) {

				grid[currentidx] = NODECLOSE;

				for (i=-1;i<=1;i++) {
					int y = currenty+i;
					if ((y>=0)&&(y<height)) {
						for (int j=-1;j<=1;j++) {
							int x = currentx+j;
							if ((x>=0)&&(x<width)) {
								int checkidx1 = y*width+currentx;
								int checkidx2 = currenty*width+x;
								int idx = y*width+x;
								if ((grid[idx] != NODECLOSE)&&(grid[idx] != NODEOPEN)&&(grid[idx] != NODEWALL)&&(grid[checkidx1] != NODEWALL)&&(grid[checkidx2] != NODEWALL)) {
									NodeArray[idx]->Gn = NodeArray[idx]->calculateGn(NodeArray[currentidx]);
									NodeArray[idx]->Hn = NodeArray[idx]->calculateHn(NodeArray[finishidx]);
									NodeArray[idx]->Fn = NodeArray[idx]->calculateFn();
									NodeArray[idx]->m_parentNode = NodeArray[currentidx];
									if (grid[idx]!=NODEFINISH) {
										OpenList[count] = NodeArray[idx];
										grid[idx] = NODEOPEN;
										count++;
									} else
										foundtarget = true;
								} else if (grid[idx] == NODEOPEN) {
									if (NodeArray[idx]->calculateGn(NodeArray[currentidx]) < NodeArray[idx]->Gn) {
										NodeArray[idx]->Gn = NodeArray[idx]->calculateGn(NodeArray[currentidx]);
										NodeArray[idx]->m_parentNode = NodeArray[currentidx];
									}
								}
							}
						}
					}
				}

				if (!foundtarget) {

					if (count>0) {

						currentidx = findSmallestFn(OpenList, count);
						currentx = OpenList[currentidx]->x;
						currenty = OpenList[currentidx]->y;

						for (i=currentidx;i<count;i++)
							OpenList[i] = OpenList[i+1];

						currentidx = currenty*width+currentx;

						count--;

					} else

						iterate = false;
				}

			}

			if (foundtarget) {

				iterate = true;

				currentidx = finishidx;
				currentx = finishpointx;
				currenty = finishpointy;

				scare_mouse();

				while (iterate) {

					circlefill(screen,currentx*w+startx+w/2,currenty*w+starty+w/2,w/4,makecol(255,255,0));
					if (currentidx==startidx)
						iterate = false;
					else {
						currentx = NodeArray[currentidx]->m_parentNode->x;
						currenty = NodeArray[currentidx]->m_parentNode->y;
						currentidx = currenty*width+currentx;
					}

				}

				unscare_mouse();

				do {

				} while (!key[KEY_SPACE]);

			}
		}
	}

	for (int i=0;i<width*height;i++)
		delete NodeArray[i];
	delete NodeArray;

	delete OpenList;

	delete grid;


	return 0;

} END_OF_MAIN();*/

