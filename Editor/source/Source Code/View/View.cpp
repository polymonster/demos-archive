#include "View.h"
#include "Input.h"

CView::CView()
{
	nameIdentifier = "view";

	m_gridSize = 20;
	m_cellSize = 20;
	m_viewGrid = true;
	m_pressed = false;

}

void CView::drawGrid()
{
	if(m_viewGrid)
	{
		grid(m_cellSize,m_gridSize,10);
	}
}

void CView::renderAux()
{
	drawGrid();
}

void CView::render()
{
	drawGrid();
}

bool CView::snapToGrid(int *snap)
{
	*snap = m_cellSize;

	return m_snapToGrid;
}

void CView::positionCamera()
{
	user::getInstance()->positionCamera();
}

void CView::resetFocus()
{

}

void CView::execute()
{
	if(g_editMode == EDIT_VIEW) 
	{
		ShowWindow(g_hViewTool, SW_SHOW);

		getWindowInput();
	}
	else hideWindows();
}

void CView::hideWindows()
{
	ShowWindow(g_hViewTool, SW_HIDE);
}

void CView::getWindowInput()
{
	//check for camera mode
	HWND pivot = GetDlgItem(g_hViewTool,IDC_PIVOT);
	if(SendMessage(pivot,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_input.cameraMode = 0;
	}

	HWND free = GetDlgItem(g_hViewTool,IDC_FREE);
	if(SendMessage(free,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_input.cameraMode = 1;
	}

	HWND center = GetDlgItem(g_hViewTool,IDC_ORIGIN);
	if(SendMessage(center,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		user::getInstance()->centerAt(SVector3(0,0,0));
	}

	HWND toggle = GetDlgItem(g_hViewTool,IDC_TOGGLE_GRID);
	if(SendMessage(toggle,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		if(!m_pressed)
		{
			m_viewGrid = !m_viewGrid;
			m_pressed = true;
		}
	}
	else
	{
		m_pressed = false;
	}

	char text[255];

	GetDlgItemText(g_hViewTool, IDC_GRID_SIZE,text,255); 
	m_gridSize = atof(text);

	GetDlgItemText(g_hViewTool, IDC_CELL_SIZE,text,255); 
	m_cellSize = atof(text);
}

void CView::output(std::ofstream &fout)
{
	fout << "view_options" << "\n";
	fout << g_input.cameraMode << " ";
	fout << g_input.pos.x << " ";
	fout << g_input.pos.y << " ";
	fout << g_input.pos.z << " ";
	fout << g_input.rot.x << " ";
	fout << g_input.rot.y << " ";
	fout << g_input.zoom << "\n";
	fout << m_viewGrid << " ";
	fout << m_cellSize << " ";
	fout << m_gridSize << " ";
}

void CView::readIn(std::ifstream &fin)
{
	fin >> g_input.cameraMode;
	fin >> g_input.pos.x;
	fin >> g_input.pos.y;
	fin >> g_input.pos.z;
	fin >> g_input.rot.x;
	fin >> g_input.rot.y;
	fin >> g_input.zoom;
	fin >> m_viewGrid;
	fin >> m_cellSize;
	fin >> m_gridSize;

	char text[255];

	itoa(m_gridSize,text,10);
	SetDlgItemText(g_hViewTool,IDC_GRID_SIZE, text); 

	itoa(m_cellSize,text,10);
	SetDlgItemText(g_hViewTool,IDC_CELL_SIZE, text); 
}
