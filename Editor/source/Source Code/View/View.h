#ifndef _VIEW_H
#define _VIEW_H

#include "Globals.h"
#include "sceneObject.h"

class CView : public CSceneObject
{
	public:

		CView();
		~CView(){};

		void execute();
		void output(std::ofstream &fout);
		void readIn(std::ifstream &fin);

	private:

		int m_gridSize;
		int m_cellSize;

		bool m_snapToGrid;
		bool m_viewGrid;

		SVector3 m_cameraCenter;

		void hideWindows();
		void getWindowInput();
		void drawGrid();
		void positionCamera();
		bool snapToGrid(int *snap);
		void resetFocus();
		void render();
		void renderAux();

		bool m_pressed;
};



#endif