#include "modelManager.h"
#include "TimerSingleton.h"
#include "toolbars.h"
#include "sceneObject.h"
#include "input.h"
#include "textureSingleton.h"
#include "3ds.h"

CModelManager* CModelManager::m_instance = NULL; 

t3DModel modla;

void CModelManager::create(){

	if(m_instance == NULL)
	{
		m_instance = new CModelManager;
	}
}

void CModelManager::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

bool CModelManager::selectedIsValid()
{
	if(m_selectedModel >= 0 && m_selectedModel < modelContainer.size())
	{
		return true;
	}

	return false;
}

void CModelManager::drawDebug(int id)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	modelContainer.at(id).mesh->drawNormals();
	modelContainer.at(id).mesh->drawTangents();
}

void CModelManager::destroyObjects(){

	for(int i = 0; i < m_numberOfModels; i++)
	{
		modelContainer.at(i).mesh->Unload_3ds();
	}
}

void CModelManager::getBox(int id, SVector3 *pos,SVector3 *scale)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	modelContainer.at(id).mesh->getBox(pos,scale);
}

void CModelManager::destroyModel(int id)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	modelContainer.at(id).mesh->Unload_3ds();

	delete modelContainer.at(id).mesh;

	std::vector<SModelContainer>::iterator remove = modelContainer.begin() + id;
	modelContainer.erase(remove);

	updateLookup(id);

	updateListBox();

	m_selectedModel = -1;
}

void CModelManager::parseFilename(SModelContainer *nameModel, char *file)
{
	//Parse the FILENAME file/file
	bool keepGoing = true;
	
	char mask[6];
	char root[256];

	int len = strlen(file);

	int readPos = len - 1;

	while(keepGoing)
	{
		for(int i = 5; i >= 0; i--)
		{
			int offset = 5 - i; 
			if(offset < 0) mask[i] = 0;
			mask[i] = file[readPos - offset];
		}

		readPos--;

		if
		(
			   (mask[0] == '\\' || mask[0] == '/' || mask[0] == 0)
			&& (mask[1] == 'd' || mask[1] == 'D')
			&& (mask[2] == 'a' || mask[2] == 'A')
			&& (mask[3] == 't' || mask[3] == 'T')
			&& (mask[4] == 'a' || mask[4] == 'A')
			&& (mask[5] == '\\' || mask[5] == '/')
		)
		{
			keepGoing = false;
		}

	}
	
	int fromPos = readPos - 3;

	int counter = 0;

	//read it into root
	for(int i = fromPos; i < len; i++)
	{
		root[counter++] = file[i];
	}

	root[counter] = '\0';

	nameModel->filename = root;
}

void CModelManager::parseName(SModelContainer *nameModel, char *file)
{
	//get the models actual name file/file/model_name.3ds = model_name
	char name[128];
	char nameBuffer[128];
	char buf;

	strcpy(name,"\0");
	strcpy(nameBuffer,"\0");

	int len = strlen(file) -1;

	buf = file[len];
	len--;

	int counter = 0;

	while(buf != '\\' && buf != '/')
	{
		nameBuffer[counter++] = buf;
		buf = file[len--];
	}

	nameBuffer[counter] = '\0';

	len = strlen(nameBuffer) - 1;
	buf = nameBuffer[len--];

	counter = 0;

	while(buf != '.')
	{
		name[counter++] = buf;

		buf = nameBuffer[len--];
	}

	name[counter] = '\0';

	nameModel->name = name;
}

int CModelManager::checkForDuplicates(SModelContainer *model)
{
	for(int i = 0; i < modelContainer.size(); i++)
	{
		if(model->filename == modelContainer.at(i).filename)
		{
			return modelContainer.at(i).raw_id;
		}
	}

	return -1;
}

int CModelManager::load(char *file,bool parse,int cullMode)
{

	if(file == NULL) return -1;

	//Load in the new model
	SModelContainer newModel;
	
	if(parse) 
	{
		parseFilename(&newModel,file);
		parseName(&newModel,file);
		strcpy(file,newModel.filename.c_str());
	}
	else 
	{
		newModel.filename = file;
		parseName(&newModel,file);
	}

	
	int duplicateID = checkForDuplicates(&newModel);

	if(duplicateID != -1)
	{
		MessageBox(g_hWnd, 
		"The chosen model has already been loaded", 
		"Duplication", MB_OK);

		return duplicateID;
	}

	newModel.cullMode = 0;
	newModel.collision = false;
	newModel.numberOfLod = 0;

	Import3DS(&modla, file);
	newModel.mesh = new C3dsLoader(modla,file);

	//set this baby as active
	m_selectedModel = modelContainer.size() -1;

	//clean up :)
	for(int i = 0; i < modla.numOfObjects; i++)	
	{
		delete [] modla.pObject[i].pFaces;
		delete [] modla.pObject[i].pNormals;
		delete [] modla.pObject[i].pVerts;
		delete [] modla.pObject[i].pTexVerts;
	}

	modla.numOfMaterials = 0;
	modla.numOfObjects = 0;

	newModel.raw_id = m_numberOfModels;
	newModel.cullMode = cullMode;

	//push on the stack
	modelContainer.push_back(newModel);

	addToLookup(m_numberOfModels,modelContainer.size()-1);

	updateListBox();

	HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
	SendMessage(list, LB_SETCURSEL, modelContainer.size()-1, NULL);

	HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
	SendMessage(propCombo, CB_SETCURSEL, modelContainer.size()-1, 0);

	HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
	SendMessage(foliageCombo, CB_SETCURSEL, modelContainer.size()-1, 0);

	g_input.viewerRot.x = 45;
	g_input.viewerRot.y = 45;

	SVector3 scale;
	SVector3 pos;
	modelContainer.back().mesh->getBox(&pos,&scale);

	g_input.viewerZoom = scale.z + 100;

	return m_numberOfModels++;
}

void CModelManager::loadChild(char *file, int childType)
{
	
}

void CModelManager::updateListBox(int removal)
{
	//clear it out
	for(int i = 0; i < modelContainer.size(); i++)
	{
		HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
		SendMessage(list, LB_DELETESTRING, 0, NULL);

		HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
		SendMessage(foliageCombo, CB_DELETESTRING, 0, NULL);

		HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
		SendMessage(propCombo, CB_DELETESTRING, 0, NULL);
	}

	//fill it back up
	for(int i = 0; i < modelContainer.size(); i++)
	{
		//Add the data to the list box
		HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
		SendMessage(list, LB_INSERTSTRING, -1, (LPARAM)modelContainer.at(i).name.c_str());

		HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
		SendMessage(foliageCombo, CB_INSERTSTRING, -1, (LPARAM)modelContainer.at(i).name.c_str());

		HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
		SendMessage(propCombo, CB_INSERTSTRING, -1, (LPARAM)modelContainer.at(i).name.c_str());
	}
}

void CModelManager::addToLookup(int position, int id)
{
	m_lookupTable[position] = id;
}

int CModelManager::idLookup(int position)
{
	return m_lookupTable[position];
}

void CModelManager::updateLookup(int deleted_position)
{
	for(int i = 0; i < m_numberOfModels; i++)
	{
		if(m_lookupTable[i] > deleted_position)
		{
			m_lookupTable[i]--;
		}
		else if(m_lookupTable[i] == deleted_position)
		{
			m_lookupTable[i] = -1;
		}
	}
}

std::string CModelManager::getModelName(int id)
{
	return modelContainer.at(id).name;
}

void CModelManager::deleteVBO(int id)
{
	id = idLookup(id);
	m_selectedModel = id;

	if(selectedIsValid())
	{
		modelContainer.at(id).mesh->deleteVBO();
	}
}

void CModelManager::cleanUp()
{
	//clear it out
	for(int i = 0; i < modelContainer.size(); i++)
	{
		HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
		SendMessage(list, LB_DELETESTRING, 0, NULL);

		HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
		SendMessage(foliageCombo, CB_DELETESTRING, 0, NULL);

		HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
		SendMessage(propCombo, CB_DELETESTRING, 0, NULL);
	}

	for(int i = modelContainer.size() - 1; i >= 0; i--)
	{
		modelContainer.at(i).mesh->Unload_3ds();
		modelContainer.at(i).mesh->deleteVBO();

		std::vector<SModelContainer>::iterator remove = modelContainer.begin() + i;
		modelContainer.erase(remove); 
	}

	m_numberOfModels = 0;
	m_numberOfCharacters = 0;
	m_deletedModelStart = 0;
	m_polyCounter = 0;

	modelContainer.clear();
}

void CModelManager::render(int id){

	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	setCullMode(modelContainer.at(id).cullMode);
	modelContainer.at(id).mesh->drawVBO();
}

void CModelManager::renderLOD(int id, int LOD){

	if(LOD == 0) 
	{
		render(id);
		return;
	}
	else
	{
		id = idLookup(id);

		if(id < 0 || id >= modelContainer.size()) return;

		if(modelContainer.at(id).numberOfLod >= LOD)
		{
			int LODIndex = LOD - 1;

			setCullMode(modelContainer.at(id).cullMode);
			modelContainer.at(id).lod[LODIndex]->drawVBO();
		}
		else
		{
			render(modelContainer.at(id).raw_id);
		}
	}
}

void CModelManager::drawBB(int id)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	modelContainer.at(id).mesh->drawBB();
}

void CModelManager::renderPicking(int id,colorByte color)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	modelContainer.at(id).mesh->renderPicking(color);
}

void CModelManager::enumerateModels()
{
	HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
	int pos = (int)SendMessage(list, LB_GETCURSEL, 0, 0); 

	HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
	SendMessage(propCombo, CB_SETCURSEL, pos, 0);

	HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
	SendMessage(foliageCombo, CB_SETCURSEL, pos, 0);
	
	if(pos < modelContainer.size() && pos >= 0)
	{
		m_selectedModel = pos;
	}

	if(selectedIsValid())
	{
		SetDlgItemText(g_hMod, IDC_MODEL_FILE, 
			modelContainer[m_selectedModel].filename.c_str());	
	}

	checkRemoval();

	wglMakeCurrent(g_hDCMod, g_hRCMod);

	renderModelViewer();

	SwapBuffers(g_hDCMod);

	wglMakeCurrent(g_hDC, g_hRC);
}

void CModelManager::renderModelViewer()
{
	//SETUP
	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);

	SVector3 camPos = g_input.pos;

	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "decal"), 1);
    glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "shadowMap"), 0);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "normalMap"), 2);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "lightPos"), 0,0,0, 1.0f);
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "cameraPos"), camPos.x, camPos.y, camPos.z, 1.0f);

	float aspect = (float) g_screenWidth/g_screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect, 1.0f, 30000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	quickClear();

	user::getInstance()->viewerMouseLook();
	user::getInstance()->viewerPositionLookAt();

	glEnable(GL_LIGHTING);

	if(selectedIsValid())
	{
		HWND combo = GetDlgItem(g_hMod,IDC_COMBO_CULLS);
		int cullMode = (int)SendMessage(combo, CB_GETCURSEL, 0, 0);
		modelContainer[m_selectedModel].cullMode  = cullMode;

		render(modelContainer.at(m_selectedModel).raw_id);
	}

	//ORTHO BORDER
	CShaderManager::getInstance()->detachShader();

	glDisable(GL_LIGHTING);

	CTextureMan::getInstance()->disableAllUnits();

	glPushMatrix();

	orthoMode(0,0,1024,1024);
	
	glColor3f(0.3,0.3,0.3);
	orthoBorder(5, 1024);

	perspectiveMode();

	glPopMatrix();
}

void CModelManager::setSelected(int id)
{
	id = idLookup(id);

	if(id < 0 || id >= modelContainer.size()) return;

	m_selectedModel = id;

	if(selectedIsValid())
	{
		return;
	}
	else
	{
		m_selectedModel = -1;
	}
}

int CModelManager::selectedModel()
{
	if(selectedIsValid())
	{
		return modelContainer.at(m_selectedModel).raw_id;
	}
	else
	{
		return -1;
	}
}

void CModelManager::checkRemoval()
{
	HWND del = GetDlgItem(g_hMod,IDC_REMOVE_MODEL);
	if(SendMessage(del,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		if(selectedIsValid())
		{
			destroyModel(modelContainer.at(m_selectedModel).raw_id);

			updateListBox(1);

			HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
			SendMessage(list, LB_SETCURSEL, -1, 0);

			m_selectedModel = -1;
		}
	}
}

void CModelManager::export()
{
	char *file = saveFile("Model Manager Library","oml");
	if(file == NULL) return;

	std::ofstream fout;

	fout.open(file);

	output(fout);

	fout.close();
}

void CModelManager::output(std::ofstream &fout)
{
	fout << "model_manager" << " " << modelContainer.size() << "\n";

	for(int i = 0; i < modelContainer.size(); i++)
	{
		fout << modelContainer.at(i).filename << " "
			 << modelContainer.at(i).cullMode << "\n";
	}
}

void CModelManager::import()
{
	char *file = openFile("Model Manager Library","oml");
	if(file == NULL) return;

	std::ifstream fin;

	fin.open(file);

	std::string word;

	fin >> word;

	if(word == "model_manager") readIn(fin);

	fin.close();

}

void CModelManager::readIn(std::ifstream &fin)
{
	int quantity;
	int cullMode;

	fin >> quantity;

	for(int i = 0; i < quantity; i++)
	{
		char file[256];
	
		fin >> file;
		fin >> cullMode;

		int id = load(file,false,cullMode);
	}

}


