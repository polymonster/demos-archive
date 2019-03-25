#include "Scene.h"
#include "Fonts.h"
#include "TimerSingleton.h"
#include "modelManager.h"
#include "textureSingleton.h"
#include "water.h"
#include "toolbars.h"
#include "atmosphere.h"
#include "lighting.h"

#include <iostream>

bool waterPressed = false;

void CScene::newScene()
{
	CTextureMan::getInstance()->deleteAll();
	CTextureMan::getInstance()->loadBasePack();

	CModelManager::getInstance()->cleanUp();

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
		delete m_sceneObject.at(i);
		m_sceneObject.erase(remove);
	}

	m_sceneObject.clear();

	CModelManager::getInstance()->cleanUp();

	m_sceneObject.resize(m_sceneObject.size() + 1);
	m_sceneObject.at(m_sceneObject.size() - 1) = new CView();
	m_sceneObject.resize(m_sceneObject.size() + 1);
	m_sceneObject.at(m_sceneObject.size() - 1) = new CPropList();

	octree = new COctreeNode(SVector3(0,0,0),SVector3(1000,1000,1000));

	m_atmosphere = NULL;
}

void CScene::save()
{
	char *file = saveFile("Organic Editor Scene File","txt");
	if(file == NULL) return;

	std::ofstream fout;
	fout.open(file);

	CTextureMan::getInstance()->output(fout);
	CModelManager::getInstance()->output(fout);

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "view") 
		{
			m_sceneObject.at(i)->output(fout);
		}
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
		{
			m_sceneObject.at(i)->output(fout);
		}
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "props") 
		{
			m_sceneObject.at(i)->output(fout);
		}
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "water") 
		{
			m_sceneObject.at(i)->output(fout);
		}
	}

	for(int i = 0; i < m_sceneObject.size(); i++)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "atmosphere") 
		{
			m_sceneObject.at(i)->output(fout);
		}
	}

	octree->output(fout);

	fout.close();
}

void CScene::load()
{
	wglMakeCurrent(g_hDC, g_hRC);

	CTextureMan::getInstance()->deleteAll();
	CTextureMan::getInstance()->loadBasePack();

	CModelManager::getInstance()->cleanUp();

	g_numberOfWaterPlanes = 0;
	m_atmosphere = NULL;

	//delete water planes
	for(int i = m_sceneObject.size() - 1; i >= 0 ; i--)
	{
		if(m_sceneObject.at(i)->nameIdentifier == "water" || m_sceneObject.at(i)->nameIdentifier == "atmosphere")
		{
			std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
			delete m_sceneObject.at(i);
			m_sceneObject.erase(remove);
		}
	}

	char *file = openFile("Organic Editor Scene File","txt");
	if(file == NULL) return;

	std::cout << "loading file" << "\n";

	std::ifstream fin;
	std::string word;

	fin.open(file);

	while(!fin.eof())
	{
		fin >> word;

		if(word == "texture_manager")
		{
			std::cout << "loading texture manager" << "\n";
			CTextureMan::getInstance()->readIn(fin);
		}
		else if(word == "model_manager")
		{
			std::cout << "loading model manager" << "\n";
			CModelManager::getInstance()->readIn(fin);
		}
		else if(word == "props")
		{
			std::cout << "loading props" << "\n";
			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				std::string nameId = m_sceneObject.at(i)->nameIdentifier;

				if(nameId == "props") 
				{
					CPropList *prop = (CPropList*) m_sceneObject.at(i);
					prop->readIn(fin);

					i = m_sceneObject.size();
					word = "nothing";
				}
			}
		}
		else if(word == "terrain")
		{
			bool newTerrain = true;

			std::cout << "loading terrain" << "\n";
			for(int i = m_sceneObject.size() - 1; i >= 0 ; i--)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
				{
					std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
					delete m_sceneObject.at(i);
					m_sceneObject.erase(remove);


					m_sceneObject.resize(m_sceneObject.size() + 1);
					m_sceneObject.at(m_sceneObject.size() - 1) = new CTerrain(fin);

					int i = m_sceneObject.size();

					newTerrain = false;

					word = "nothing";
				}
			}

			if(newTerrain)
			{
				m_sceneObject.resize(m_sceneObject.size() + 1);
				m_sceneObject.at(m_sceneObject.size() - 1) = new CTerrain(fin);
				word = "nothing";
			}
		}
		else if(word == "water")
		{
			std::cout << "loading water" << "\n";
			m_sceneObject.resize(m_sceneObject.size() + 1);
			m_sceneObject.at(m_sceneObject.size() - 1) = new CWaterPlane(g_numberOfWaterPlanes++);

			CWaterPlane *water = (CWaterPlane*) m_sceneObject.at(m_sceneObject.size() - 1);
			water->readIn(fin);

			word = "nothing";
		}
		else if(word == "view_options")
		{
			std::cout << "loading options" << "\n";
			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "view") 
				{
					CView *view = (CView*) m_sceneObject.at(i);
					view->readIn(fin);
				}
			}

			word = "nothing";
		}
		else if(word == "atmosphere")
		{
			m_sceneObject.resize(m_sceneObject.size() + 1);
			m_sceneObject.at(m_sceneObject.size() - 1) = new CAtmosphere(fin);
			m_atmosphere = (CAtmosphere *) m_sceneObject.at(m_sceneObject.size() - 1);

			word = "nothing";
		}
		else if(word == "octree_params")
		{
			std::cout << "loading octree" << "\n";

			octree->readIn(fin);

			word = "nothing";
		}
	}

	fin.close();

	std::cout << "load complete" << "\n";

}

void CScene::sceneOptionHandler(WPARAM &wParam)
{
	switch(LOWORD(wParam))
	{
		case ID_IMPORT_3DS:

			return;
		
		break;

		case ID_IMPORT_CAL3DANIMATEDMESH:

			return;

		break;

		case ID_EXPORT_TANKYPANKYMAP:

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "props") 
				{
					CPropList *prop = (CPropList*) m_sceneObject.at(i);
					prop->exportToTanky();
				}
			}

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
				{
					CTerrain *terrain = (CTerrain*) m_sceneObject.at(i);
					terrain->export();
				}
			}

			return;

		break;

		case IDD_EXPORT_SF_MAP:

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "props") 
				{
					CPropList *prop = (CPropList*) m_sceneObject.at(i);
					prop->exportToTanky();
				}
			}

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
				{
					CTerrain *terrain = (CTerrain*) m_sceneObject.at(i);
					terrain->exportSF();
				}
			}

			return;

		break;

		case ID_EXPORT_TEXTUREMANAGERLIBRARY:
			
			CTextureMan::getInstance()->export();

		break;

		case ID_IMPORT_TEXTUREMANAGERLIBRARY:

			CTextureMan::getInstance()->import();

		break;

		case ID_EXPORT_MODELMANAGERLIBRARY:
			
			CModelManager::getInstance()->export();

		break;

		case ID_IMPORT_MODELMANAGERLIBRARY:

			CModelManager::getInstance()->import();

		break;

		case ID_FILE_SAVEFILE:

			save();

		break;

		case ID_FILE_LOADFILE:

			g_loadNewScene = true;

		break;


		case ID_FILE_NEW40043:

			newScene();

		break;
		
	}
}

void CScene::addObjects()
{
	switch(g_editMode)
	{
		case EDIT_WATER:
		{
			ShowWindow(g_hTerrainProp, SW_HIDE);
			ShowWindow(g_hTerrainPaint, SW_HIDE);
			ShowWindow(g_hTerrainHeight, SW_HIDE);
			ShowWindow(g_hTerrainValues, SW_HIDE);
			ShowWindow(g_hAtmosphereProp, SW_HIDE);
			ShowWindow(g_hAtmosphereEnv, SW_HIDE);
			ShowWindow(g_hAtmosphereSky, SW_HIDE);
			ShowWindow(g_hAtmosphereManagement, SW_HIDE);

			bool isWater = false;

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "water") 
				{
					isWater = true;
				}
			}
			
			if(!isWater || g_selectedWaterPlane < 0 || g_selectedWaterPlane >= g_numberOfWaterPlanes)
			{
				HWND tab = GetDlgItem(g_hWaterProp,IDC_TAB_WATER_OPTIONS);
				TabCtrl_SetCurSel(tab,0);

				int i = TabCtrl_GetCurSel(tab);
				g_waterSubEditMode = i;

				ShowWindow(g_hWaterProp, SW_SHOW);
				ShowWindow(g_hWaterOptical, SW_HIDE);
				ShowWindow(g_hWaterTexture, SW_HIDE);
				ShowWindow(g_hWaterManagement, SW_SHOW);

				//check if the user has clicked to add a new water plane
				HWND newWater = GetDlgItem(g_hWaterManagement,IDC_WATER_ADD);
				if(SendMessage(newWater,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					if(!waterPressed)
					{
						//resize the vector array by 1
						m_sceneObject.resize(m_sceneObject.size() + 1);

						//insert a new water plane
						m_sceneObject.at(m_sceneObject.size() - 1) = new CWaterPlane(g_numberOfWaterPlanes);

						//keep count of the number of water planes
						g_selectedWaterPlane = g_numberOfWaterPlanes++;

						waterPressed = true;
					}
				}
				else
				{
					waterPressed = false;
				}

				HWND select = GetDlgItem(g_hWaterManagement,IDC_WATER_SELECT);
				if(SendMessage(select,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					g_waterEditMode = SELECT_WATER;
				}

			}
			else
			{
				HWND newWater = GetDlgItem(g_hWaterManagement,IDC_WATER_ADD);
				if(SendMessage(newWater,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					if(!waterPressed)
					{
						m_sceneObject.resize(m_sceneObject.size() + 1);
						m_sceneObject.at(m_sceneObject.size() - 1) = new CWaterPlane(g_numberOfWaterPlanes);

						g_selectedWaterPlane = g_numberOfWaterPlanes++;

						waterPressed = true;
					}
				}
				else
				{
					waterPressed = false;
				}

				HWND delWater = GetDlgItem(g_hWaterManagement,IDC_WATER_DELETE);
				if(SendMessage(delWater,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					if(g_selectedWaterPlane > 0 || g_selectedWaterPlane < g_numberOfWaterPlanes)
					{
						for(int i = 0; i < m_sceneObject.size(); i++)
						{
							if(m_sceneObject.at(i)->nameIdentifier == "water") 
							{
								CWaterPlane *water = (CWaterPlane *)m_sceneObject.at(i);
									
								if(water->m_id == g_selectedWaterPlane)
								{
									std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
									delete m_sceneObject.at(i);
									m_sceneObject.erase(remove);

									i = m_sceneObject.size();
									g_selectedWaterPlane = -1;
								}
							}
						}
					}
				}

				HWND tab = GetDlgItem(g_hWaterProp,IDC_TAB_WATER_OPTIONS);
				ShowWindow(g_hWaterProp, SW_SHOW);
				int i = TabCtrl_GetCurSel(tab);
				g_waterSubEditMode = i;

				HWND select = GetDlgItem(g_hWaterManagement,IDC_WATER_SELECT);
				if(SendMessage(select,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					g_waterEditMode = SELECT_WATER;
				}

				
			}
		}
		break;

		case EDIT_TERRAIN:
		{
			ShowWindow(g_hWaterProp, SW_HIDE);
			ShowWindow(g_hWaterOptical, SW_HIDE);
			ShowWindow(g_hWaterTexture, SW_HIDE);
			ShowWindow(g_hWaterManagement, SW_HIDE);
			ShowWindow(g_hAtmosphereProp, SW_HIDE);
			ShowWindow(g_hAtmosphereEnv, SW_HIDE);
			ShowWindow(g_hAtmosphereSky, SW_HIDE);
			ShowWindow(g_hAtmosphereManagement, SW_HIDE);

			bool isTerrain = false;

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
				{
					isTerrain = true;
				}
			}

			if(!isTerrain)
			{
				HWND tab = GetDlgItem(g_hTerrainProp,IDC_TAB_TERRAIN_OPTIONS);
				TabCtrl_SetCurSel(tab,0);

				ShowWindow(g_hTerrainProp, SW_SHOW);
				ShowWindow(g_hTerrainPaint, SW_HIDE);
				ShowWindow(g_hTerrainHeight, SW_HIDE);
				ShowWindow(g_hTerrainValues, SW_SHOW);

				HWND newTerrain = GetDlgItem(g_hTerrainValues,IDC_TERRAIN_ADD);
				if(SendMessage(newTerrain,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					int size = 100;
					int tileSize = 20;
					char data[32];

					HWND combo = GetDlgItem(g_hTerrainValues,IDC_TERRAIN_TILE_SIZE);
					int curSel = SendMessage(combo,CB_GETCURSEL,0,0);
					SendMessage(combo,CB_GETLBTEXT,curSel,(LPARAM)(LPCSTR)data);

					tileSize = atoi(data);
					
					combo = GetDlgItem(g_hTerrainValues,IDC_TERRAIN_GRID_SIZE);
					curSel = SendMessage(combo,CB_GETCURSEL,0,0);
					SendMessage(combo,CB_GETLBTEXT,curSel,(LPARAM)(LPCSTR)data);

					size = atoi(data);

					m_sceneObject.resize(m_sceneObject.size() + 1);
					m_sceneObject.at(m_sceneObject.size() - 1) = new CTerrain(size,size,tileSize);
				}
			}
			else
			{
				HWND deleteTerrain = GetDlgItem(g_hTerrainValues,IDC_TERRAIN_DELETE);
				if(SendMessage(deleteTerrain,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					for(int i = 0; i < m_sceneObject.size(); i++)
					{
						if(m_sceneObject.at(i)->nameIdentifier == "terrain") 
						{
							std::vector<CSceneObject*>::iterator remove = m_sceneObject.begin() + i;
							delete m_sceneObject.at(i);
							m_sceneObject.erase(remove);
						}
					}
				}
			}
		}
		break;

		case EDIT_ATMOSPHERE:
		{
			ShowWindow(g_hTerrainProp, SW_HIDE);
			ShowWindow(g_hTerrainPaint, SW_HIDE);
			ShowWindow(g_hTerrainHeight, SW_HIDE);
			ShowWindow(g_hTerrainValues, SW_HIDE);
			ShowWindow(g_hWaterProp, SW_HIDE);
			ShowWindow(g_hWaterOptical, SW_HIDE);
			ShowWindow(g_hWaterTexture, SW_HIDE);
			ShowWindow(g_hWaterManagement, SW_HIDE);

			bool isAtmosphere = false;

			for(int i = 0; i < m_sceneObject.size(); i++)
			{
				if(m_sceneObject.at(i)->nameIdentifier == "atmosphere") 
				{
					isAtmosphere = true;
				}
			}

			if(!isAtmosphere)
			{
				HWND tab = GetDlgItem(g_hAtmosphereProp,IDC_TAB_ATMOSPHERE_OPTIONS);
				TabCtrl_SetCurSel(tab,0);

				ShowWindow(g_hAtmosphereProp, SW_SHOW);
				ShowWindow(g_hAtmosphereEnv, SW_HIDE);
				ShowWindow(g_hAtmosphereSky, SW_HIDE);
				ShowWindow(g_hAtmosphereManagement, SW_SHOW);

				HWND newAtmosphere = GetDlgItem(g_hAtmosphereManagement,IDC_ATMOSPHERE_ADD);
				if(SendMessage(newAtmosphere,BM_GETSTATE,0,0) & BST_PUSHED)
				{
					m_sceneObject.resize(m_sceneObject.size() + 1);
					m_sceneObject.at(m_sceneObject.size() - 1) = new CAtmosphere();
					m_atmosphere = (CAtmosphere *) m_sceneObject.at(m_sceneObject.size() - 1);
				}

			}

		}
		break;


		default:

			ShowWindow(g_hTerrainProp, SW_HIDE);
			ShowWindow(g_hTerrainPaint, SW_HIDE);
			ShowWindow(g_hTerrainHeight, SW_HIDE);
			ShowWindow(g_hTerrainValues, SW_HIDE);
			ShowWindow(g_hWaterProp, SW_HIDE);
			ShowWindow(g_hWaterOptical, SW_HIDE);
			ShowWindow(g_hWaterTexture, SW_HIDE);
			ShowWindow(g_hWaterManagement, SW_HIDE);
			ShowWindow(g_hAtmosphereProp, SW_HIDE);
			ShowWindow(g_hAtmosphereEnv, SW_HIDE);
			ShowWindow(g_hAtmosphereSky, SW_HIDE);
			ShowWindow(g_hAtmosphereManagement, SW_HIDE);

		break;
	}
}


