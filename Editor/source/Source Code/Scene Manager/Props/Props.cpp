#include "props.h"
#include "input.h"
#include "modelManager.h"
#include "shaderManager.h"
#include "helpers.h"
#include "textureSingleton.h"

int g_propEditMode;

bool clonePressed = false;

CProp::CProp(int modelID)
{
	m_modelID = modelID;

	m_position.x = 0;
	m_position.y = 0;
	m_position.z = 0;

	m_rotation.x = 0;
	m_rotation.y = 0;
	m_rotation.z = 0;

	m_scale.x = 1;
	m_scale.y = 1;
	m_scale.z = 1;

	m_selectedAxis.x = 0;
	m_selectedAxis.y = 0;
	m_selectedAxis.z = 0;

	g_propEditMode = 0;

	m_lod = 0;

	toDelete = false;
}

CProp::CProp(int modelID,SVector3 pos,SVector3 scale, SVector3 rotation)
{
	m_modelID = modelID;

	m_position = pos;

	m_rotation = rotation;

	m_scale = scale;

	m_selectedAxis.x = 0;
	m_selectedAxis.y = 0;
	m_selectedAxis.z = 0;

	g_propEditMode = 0;

	m_lod = 0;

	toDelete = false;

}

SCopyData CProp::getCopyData()
{
	SCopyData data;

	data.modelID = m_modelID;
	data.pos = m_position;
	data.rot = m_rotation;
	data.scale = m_scale;

	return data;
}

void CProp::backBufferRender()
{
	switch(g_propEditMode)
	{
		case TRANSLATE:
				
				useTranslateWidget(&m_selectedAxis,&m_position);

		break;

		case ROTATE:
				
				useRotateWidget(&m_selectedAxis,&m_position,&m_rotation);

		break;

		case SCALE:
				
				useScaleWidget(&m_selectedAxis,&m_position,&m_scale);
		break;
	}
}

//
void CProp::increment(SVector3 pos, SVector3 rot, SVector3 scale)
{
	m_position += pos;
	m_rotation += rot;
	m_scale += scale;
}

void CProp::renderPicking(colorByte color)
{
	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);

	glRotated(m_rotation.x,1,0,0);
	glRotated(m_rotation.y,0,1,0);
	glRotated(m_rotation.z,0,0,1);

	glScaled(m_scale.x,m_scale.y,m_scale.z);

	CModelManager::getInstance()->renderPicking(m_modelID,color);

	glPopMatrix();
}

void CProp::execute()
{

}

void CProp::renderAux(bool widgets)
{
	if(g_editMode == EDIT_PROPS)
	{
		if(widgets)
		{
			switch(g_propEditMode)
			{
				case TRANSLATE:
						
						translateWidget(m_position,m_selectedAxis,0);

				break;

				case ROTATE:
						
						rotateWidget(m_position,m_selectedAxis);

				break;

				case SCALE:
						
						translateWidget(m_position,m_selectedAxis,1);

				break;
			}
		}

		glColor3f(1,1,1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPushMatrix();
			glTranslated(m_boundingBoxPos.x,m_boundingBoxPos.y,m_boundingBoxPos.z);
			glRotated(m_rotation.x,1,0,0);
			glRotated(m_rotation.y,0,1,0);
			glRotated(m_rotation.z,0,0,1);
			glScaled(m_boundingBoxScale.x,m_boundingBoxScale.y,m_boundingBoxScale.z);
			cube();
		glPopMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void CProp::render()
{
	SVector3 camPos = g_input.pos;

	CShaderManager::getInstance()->attachShader(g_shaderId.normalMap);

	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "shadowMap"), 0);
    glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "decal"), 1);
	glUniform1iARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "normalMap"), 2);
	
	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "lightPos"), 0,0,0, 1.0f);

	glUniform4fARB(glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "cameraPos"), camPos.x, camPos.y, camPos.z, 1.0f);

	glPushMatrix();

	float orientationMatrix[16];

	glLoadIdentity();

	glTranslated(m_position.x,m_position.y,m_position.z);
	
	glRotated(m_rotation.x,1,0,0);
	glRotated(m_rotation.y,0,1,0);
	glRotated(m_rotation.z,0,0,1);

	glScaled(m_scale.x,m_scale.y,m_scale.z);

	glGetFloatv(GL_MODELVIEW_MATRIX,orientationMatrix);

	glPopMatrix();

	glUniformMatrix4fvARB(
		glGetUniformLocationARB(CShaderManager::getInstance()->getShaderProgram(g_shaderId.normalMap), "orientationMatrix"),
		1,
		1,
		orientationMatrix);


	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);

	glRotated(m_rotation.x,1,0,0);
	glRotated(m_rotation.y,0,1,0);
	glRotated(m_rotation.z,0,0,1);

	glScaled(m_scale.x,m_scale.y,m_scale.z);


	CModelManager::getInstance()->renderLOD(m_modelID, m_lod);

	glPopMatrix();

	CModelManager::getInstance()->getBox(m_modelID,&m_boundingBoxPos,&m_boundingBoxScale);

	m_boundingBoxPos = m_boundingBoxPos + m_position;
	m_boundingBoxScale = m_boundingBoxScale * m_scale;
}

void CProp::cleanUp()
{
	//CModelManager::getInstance()->deleteVBO(m_modelID);
}


CPropList::CPropList()
{
	nameIdentifier = "props";
	m_addedProp = false;
}

void CPropList::foliageRandomisation(SVector3 *pos, SVector3 *scale, SVector3 *rotation)
{
	char text[255];

	float maxBound;
	float minBound;
	float range;
	float ratio;

	//RANDOMISE SCALE
	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_SCALE_FROM, text, 255); 
	minBound = atof(text);

	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_SCALE_TO, text, 255); 
	maxBound = atof(text);

	range = maxBound - minBound;

	srand(time(NULL));
	ratio = rand() % 100;
	ratio *= 0.01;

	float randomScale = minBound + (range  * ratio);

	*scale = SVector3(randomScale,randomScale,randomScale);

	//RANDAMISE ROTATION
	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_ROTATE_FROM, text, 255);
	minBound = atof(text);

	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_ROTATE_TO, text, 255);
	maxBound = atof(text);

	range = maxBound - minBound;

	srand(time(NULL));
	ratio = rand() % 100;
	ratio *= 0.01;

	rotation->y = minBound + (range * ratio );

	//OFFSET POSITIONS
	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_X, text,255);
	pos->x += atof(text);

	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_Y, text,255);
	pos->y += atof(text);

	GetDlgItemText(g_hFoliageProp, IDC_FOLIAGE_OFFSET_Z, text,255);
	pos->z += atof(text);
}

void CPropList::execute()
{
	if(g_editMode == EDIT_PROPS)
	{
		ShowWindow(g_hFoliageProp, SW_HIDE);
		ShowWindow(g_hPropProp, SW_SHOW);

		dialogHandler();

		renderModelViewer();
	}
	else if(g_editMode == EDIT_FOLIAGE)
	{
		ShowWindow(g_hPropProp, SW_HIDE);
		ShowWindow(g_hFoliageProp, SW_SHOW);

		HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
		int pos = (int)SendMessage(foliageCombo, CB_GETCURSEL, 0, 0);

		HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
		SendMessage(list, LB_SETCURSEL, pos, 0); 

		HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
		SendMessage(g_hPropProp, CB_SETCURSEL, pos, 0);

		if(g_input.leftMouse)
		{
			SVector3 pos = g_input.mousePosOnTerrain;
			SVector3 scale(1,1,1);
			SVector3 rotation(0,0,0);
			
			foliageRandomisation(&pos,&scale,&rotation);
			
			int selectedModel = CModelManager::getInstance()->selectedModel();

			CProp *empty = new CProp();
			m_nodes.push_back(empty);
			m_nodes.back() = new CProp(selectedModel,pos,scale,rotation);
			m_selectedProp = m_nodes.size() - 1;

			g_input.leftMouse = false;
		}

		renderModelViewer();
	}
	else
	{
		ShowWindow(g_hPropProp, SW_HIDE);
		ShowWindow(g_hFoliageProp, SW_HIDE);
	}
}

void CPropList::deleteModel()
{
	HWND propCombo = GetDlgItem(g_hPropProp, IDC_PROP_MODEL_LIST);
	int pos = (int)SendMessage(propCombo, CB_GETCURSEL, 0, 0);

	HWND list = GetDlgItem(g_hMod,IDC_MODEL_LIST);
	SendMessage(list, LB_SETCURSEL, pos, 0); 

	HWND foliageCombo = GetDlgItem(g_hFoliageProp, IDC_FOLIAGE_MODEL_LIST);
	SendMessage(foliageCombo, CB_SETCURSEL, pos, 0);
			
	if(m_selectionList.size() <= 0)
	{
		if(m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
		{
			std::vector<CProp*>::iterator remove = m_nodes.begin() + m_selectedProp;
			m_nodes.erase(remove); 
			m_selectedProp = - 1;
		}
	}
	else
	{
		for(int i = 0; i < m_selectionList.size(); i++)
		{
			int id = m_selectionList.at(i);

			m_nodes.at(id)->toDelete = true;
		}

		m_selectionList.clear();
		m_selectedProp = - 1;

		for(int i = m_nodes.size() - 1; i >= 0; i--)
		{
			if(m_nodes.at(i)->toDelete)
			{
				std::vector<CProp*>::iterator remove = m_nodes.begin() + i;
				m_nodes.erase(remove); 

			}
		}
	}
}

void CPropList::cloneModel()
{
			if(m_selectionList.size() <= 0)
			{
				if(m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
				{
					SCopyData data = m_nodes.at(m_selectedProp)->getCopyData();

					CProp *empty = new CProp(data.modelID,data.pos,data.scale,data.rot);
					m_nodes.push_back(empty);
					m_selectedProp = m_nodes.size() - 1;

				}
			}
			else
			{
				std::vector<int> newSelectionList;

				for(int i = 0; i < m_selectionList.size(); i++)
				{
					int id = m_selectionList.at(i);
					
					SCopyData data = m_nodes.at(id)->getCopyData();

					CProp *empty = new CProp(data.modelID,data.pos,data.scale,data.rot);
					m_nodes.push_back(empty);

					newSelectionList.push_back(m_nodes.size() - 1);
				}

				m_selectionList.clear();

				m_selectionList = newSelectionList;

			}
}

void CPropList::addModel()
{
	int newModelInstance = CModelManager::getInstance()->selectedModel();

	if(newModelInstance != -1)
	{
		CProp *empty = new CProp();
		m_nodes.push_back(empty);
		m_nodes.back() = new CProp(newModelInstance);
		m_selectedProp = m_nodes.size() - 1;
	}
}

void CPropList::selectModel()
{

		if(g_input.leftMouse)
		{
			colorByte pix = user::getInstance()->getPixelAtMouse();
			int newSelected = (pix.r - 1 + (pix.b * 255) + (pix.g * 65025));

			if(newSelected >= 0 && newSelected < m_nodes.size()) 
			{
				if(g_input.key[VK_SHIFT] || g_input.key[VK_CONTROL])
				{
					bool alreadyExists = false;

					for(int i = 0; i < m_selectionList.size(); i++)
					{
						int id = m_selectionList.at(i);
						if(id == newSelected) alreadyExists = true;

						if(g_input.key[VK_CONTROL] && alreadyExists)
						{
							std::vector<int>::iterator remove = m_selectionList.begin() + i;
							m_selectionList.erase(remove);
							i = m_selectionList.size();
							return;
						}
					}

					if(g_input.key[VK_SHIFT])
					{
						if(!alreadyExists)
						{
							if(m_selectionList.size() <= 0)
							{
								m_selectionList.push_back(m_selectedProp);
							}

							m_selectionList.push_back(newSelected);
							int counter = 0;

							m_sharedWidgetPos = SVector3(0,0,0);

							for(int i = 0; i < m_selectionList.size(); i++)
							{
								int id = m_selectionList.at(i);
								m_sharedWidgetPos += m_nodes.at(id)->getPos();
								counter++;
							}

							m_sharedWidgetPos.x = m_sharedWidgetPos.x / counter;
							m_sharedWidgetPos.y = m_sharedWidgetPos.y / counter;
							m_sharedWidgetPos.z = m_sharedWidgetPos.z / counter;
						}
					}
				}
				else
				{
					m_selectedProp = newSelected;
					m_selectionList.clear();

					m_sharedWidgetPos = m_nodes.at(m_selectedProp)->getPos();
				}
			}
		}

		if(g_input.rightMouse)
		{
			bool regionChanged = false;
			SVector2 <int> newRegionDimensions;

			if(!m_selectionBox)
			{
				m_selectionRegionStart = g_input.mousePos;
				m_selectionBox = true;
				regionChanged = true;

				m_selectionRegionDimensions.x = 0;
				m_selectionRegionDimensions.y = 0;

				newRegionDimensions.x = 1;
				newRegionDimensions.y = 1;
			}
			else
			{
				newRegionDimensions.x = m_selectionRegionDimensions.x + g_input.mouseIncrement.x;
				newRegionDimensions.y = m_selectionRegionDimensions.y + g_input.mouseIncrement.y;

				regionChanged = true;
			}

			if(newRegionDimensions.x != m_selectionRegionDimensions.x 
				|| newRegionDimensions.y != m_selectionRegionDimensions.y)
			{
				regionChanged = true;
			}

			if(regionChanged)
			{
				int fromX = m_selectionRegionStart.x;
				int toX = newRegionDimensions.x;

				int fromY = m_selectionRegionStart.y;
				int toY = newRegionDimensions.y;

				m_selectionRegionDimensions = newRegionDimensions;

				if(toX < 0)
				{	
					fromX += toX;
					toX *= -1;
				}
				
				if(toY < 0)
				{
					fromY += toY;
					toY *= -1;
				}

				int size = (toX + 1) * (toY + 1) * 3;
				if(size < 0) return;

				unsigned char *pixel = new unsigned char[size];

				GLint viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);

				glReadPixels(fromX, (viewport[3]) - (fromY + toY), toX, toY, GL_RGB, GL_UNSIGNED_BYTE, pixel);

				for(int i = 0; i < size; i+= 3)
				{
					int newSelected = (pixel[i] - 1 + (pixel[i + 1] * 255) + (pixel[i + 2] * 65025));

					if(newSelected >= 0 && newSelected < m_nodes.size()) 
					{
						bool alreadyExists = false;

						for(int i = 0; i < m_selectionList.size(); i++)
						{
							int id = m_selectionList.at(i);
							if(id == newSelected) alreadyExists = true;
						}

							if(!alreadyExists)
								m_selectionList.push_back(newSelected);
					}
				}

				delete[] pixel;
			}
		}
		else
		{
			m_selectionBox = false;
		}

}

void CPropList::dialogHandler()
{
	//check add model
	HWND wind = GetDlgItem(g_hPropProp,IDC_ADD_PROP);
	if(SendMessage(wind,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		if(!m_addedProp)
		{
			addModel();
			m_addedProp = true;
		}
	}
	else
	{
		m_addedProp = false;
	}

	//check delete model
	wind = GetDlgItem(g_hPropProp,IDC_DELETE_PROP);
	if(SendMessage(wind,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		deleteModel();
	}

	if(g_input.key[VK_DELETE] && !g_input.keyLocked[VK_DELETE])
	{
		deleteModel();
		g_input.keyLocked[VK_DELETE] = true;
	}

	//check clone
	if(g_input.key[VK_CONTROL]
		&& g_input.key[VK_C]
		&& !g_input.keyLocked[VK_C])
	{
		cloneModel();
		g_input.keyLocked[VK_C] = true;
	}

	HWND clone = GetDlgItem(g_hMainTool,IDC_COPY);
	if(SendMessage(clone,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		if(!clonePressed) cloneModel();
	}
	else
	{
		clonePressed = false;
	}

	//check translate mode
	HWND translate = GetDlgItem(g_hPropProp,IDC_TRANSLATE);
	if(SendMessage(translate,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_propEditMode = TRANSLATE;
	}

	//check rotate mode
	HWND rotate = GetDlgItem(g_hPropProp,IDC_ROTATE);
	if(SendMessage(rotate,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_propEditMode = ROTATE;
	}

	//check scale mode
	HWND scale = GetDlgItem(g_hPropProp,IDC_SCALE);
	if(SendMessage(scale,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_propEditMode = SCALE;
	}

	//check select mode
	wind = GetDlgItem(g_hPropProp,IDC_SELECT_PROP);
	if(SendMessage(wind,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		g_propEditMode = SELECT_PROPS;
	}

	HWND center = GetDlgItem(g_hPropProp,IDC_FOCUS_MODEL);
	if(SendMessage(center,BM_GETSTATE,0,0) & BST_PUSHED)
	{
		//if(m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
		SVector3 pos = m_nodes.at(m_selectedProp)->getPos();
			user::getInstance()->centerAt(pos);
	}

	if(m_selectionList.size() <= 0 && m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
	{
		enableControls(true);
	}
	else
	{
		enableControls(false);
	}
}

void CPropList::enableControls(bool enable)
{
	if(enable)
	{
		char text[64];

		HWND control;

		//POSITION X
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_position.x);
		SetDlgItemText(g_hPropProp,IDC_PROP_POS_X,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_X);
		Edit_Enable(control,false);

		//Y
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_position.y);
		SetDlgItemText(g_hPropProp,IDC_PROP_POS_Y,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_Y);
		Edit_Enable(control,false);

		//Z
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_position.z);
		SetDlgItemText(g_hPropProp,IDC_PROP_POS_Z,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_Z);
		Edit_Enable(control,false);

		//SCALE X
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_scale.x);
		SetDlgItemText(g_hPropProp,IDC_PROP_SCALE_X,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_X);
		Edit_Enable(control,false);

		//Y
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_scale.y);
		SetDlgItemText(g_hPropProp,IDC_PROP_SCALE_Y,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_Y);
		Edit_Enable(control,false);

		//Z
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_scale.z);
		SetDlgItemText(g_hPropProp,IDC_PROP_SCALE_Z,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_Z);
		Edit_Enable(control,false);

		//ROTATION X
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_rotation.x);
		SetDlgItemText(g_hPropProp,IDC_PROP_ROT_X,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_X);
		Edit_Enable(control,false);

		//Y
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_rotation.y);
		SetDlgItemText(g_hPropProp,IDC_PROP_ROT_Y,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_Y);
		Edit_Enable(control,false);

		//Z
		sprintf(text,"%3g",m_nodes.at(m_selectedProp)->m_rotation.z);
		SetDlgItemText(g_hPropProp,IDC_PROP_ROT_Z,text);
		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_Z);
		Edit_Enable(control,false);

	}
	else
	{
		HWND control;

		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_X);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_Y);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_POS_Z);
		Edit_Enable(control,false);

		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_X);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_Y);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_SCALE_Z);
		Edit_Enable(control,false);

		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_X);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_Y);
		Edit_Enable(control,false);
		control = GetDlgItem(g_hPropProp,IDC_PROP_ROT_Z);
		Edit_Enable(control,false);
	}
}

void CPropList::backBufferRender()
{
	if(g_editMode == EDIT_PROPS)
	{
		if(g_propEditMode == SELECT_PROPS)
		{
			quickClear();

			//if selection mode
			for(int i = 0; i < m_nodes.size(); i++)
			{
				int index = i + 1;

				colorByte color;
				color.r = 0;
				color.g = 0;
				color.b = 0;

				if(index > 65025)
				{
					int multiplier = index / 65025;
					color.g = multiplier;

					int remainder = index - (multiplier * 65025);

					if(remainder > 255)
					{
						multiplier = remainder / 255;
						color.b = multiplier;

						remainder = remainder - (multiplier * 255);
						color.r = remainder;

					}
					else
					{
						color.r = remainder;
					}

				}
				else if(index > 255)
				{
					int multiplier = index / 255;
					color.b = multiplier;

					int remainder = index - (multiplier * 255);
					color.r = remainder;

				} 
				else
				{
					color.r = index;
				}

				m_nodes.at(i)->renderPicking(color);
			}

			selectModel();
		}

		quickClear();

		//if edit sub selection mode
		if(m_selectionList.size() <= 0)
		{
			if(m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
				m_nodes.at(m_selectedProp)->backBufferRender();
		}
		else
		{
				SVector3 incrementPos = m_sharedWidgetPos;
				SVector3 incrementRot = SVector3(0,0,0);
				SVector3 incrementScale = SVector3(0,0,0);

				switch(g_propEditMode)
				{
					case TRANSLATE:
							
							useTranslateWidget(&m_sharedWidgetSelectedAxis,&incrementPos);

					break;

					case ROTATE:
							
							useRotateWidget(&m_sharedWidgetSelectedAxis,&incrementPos,&incrementRot);

					break;

					case SCALE:
							
							useScaleWidget(&m_sharedWidgetSelectedAxis,&incrementPos,&incrementScale);
					break;
				}
				
				incrementPos = incrementPos - m_sharedWidgetPos;

				m_sharedWidgetPos += incrementPos;

				for(int i = 0; i < m_selectionList.size(); i++)
				{
						int id = m_selectionList.at(i);
						m_nodes.at(id)->increment(incrementPos,incrementRot,incrementScale);
				}

		}
	}
}

void CPropList::render()
{
	for(int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes.at(i)->render();
	}
}

void CPropList::renderAux()
{

		if(m_selectionList.size() <= 0)
		{
			//Single Edit
			if(m_selectedProp >= 0 && m_selectedProp < m_nodes.size())
				m_nodes.at(m_selectedProp)->renderAux(true);
		}
		else
		{
			//Multiple Edit
			switch(g_propEditMode)
			{
				case TRANSLATE:
						
						translateWidget(m_sharedWidgetPos,m_sharedWidgetSelectedAxis,0);

				break;

				case ROTATE:
						
						rotateWidget(m_sharedWidgetPos,m_sharedWidgetSelectedAxis);

				break;

				case SCALE:
						
						translateWidget(m_sharedWidgetPos,m_sharedWidgetSelectedAxis,1);

				break;
			}

			for(int i = 0; i < m_selectionList.size(); i++)
			{
				int id = m_selectionList.at(i);
				m_nodes.at(id)->renderAux(false);
			}

		}

		if(m_selectionBox)
		{
			glEnable(GL_BLEND);

			glPushMatrix();
			orthoMode(0,0,g_screenWidth,g_screenHeight);
			glTranslated(m_selectionRegionStart.x,m_selectionRegionStart.y,0);
			glScaled(m_selectionRegionDimensions.x,-m_selectionRegionDimensions.y,1);
			glColor4f(0.3,0.3,0.3,1);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			quad();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor4f(0.5,0.5,0.5,0.5);
			quad();
			perspectiveMode();
			glPopMatrix();
		}
}

void CPropList::cleanUp()
{
	for(int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes.at(i)->cleanUp();
	}

}

CPropList::~CPropList()
{
	for(int i = 0; i < m_nodes.size(); i++)
	{
		std::vector<CProp*>::iterator remove = m_nodes.begin() + i;
		delete m_nodes.at(i);
		m_nodes.erase(remove);
	}

	m_nodes.clear();
}

void CPropList::renderModelViewer()
{
	if(g_editMode == EDIT_FOLIAGE)
	{
		wglMakeCurrent(g_hDCFoliage, g_hRCFoliage);
		
		CModelManager::getInstance()->renderModelViewer();
		
		SwapBuffers(g_hDCFoliage);

		wglMakeCurrent(g_hDC, g_hRC);
	}
	else if(g_editMode == EDIT_PROPS)
	{
		wglMakeCurrent(g_hDCProps, g_hRCProps);
		
		CModelManager::getInstance()->renderModelViewer();
		
		SwapBuffers(g_hDCProps);

		wglMakeCurrent(g_hDC, g_hRC);
	}
}

void CPropList::exportToTanky()
{
	std::ofstream fout;

	char *file = saveFile("export map","txt");

	if(file == NULL) return;

	fout.open(file);

	for(int i = 0; i < m_nodes.size(); i++)
	{ 
		std::string name = CModelManager::getInstance()->getModelName(m_nodes.at(i)->m_modelID);

		fout <<  name << " "
			<< m_nodes.at(i)->getPos().x << " " << m_nodes.at(i)->getPos().y << " " 
			<< m_nodes.at(i)->getPos().z << "\n";
	}

	fout.close();
}

void CPropList::output(std::ofstream &fout)
{
	fout << "props" << " " << m_nodes.size() << "\n";

	for(int i = 0; i < m_nodes.size(); i++)
	{
		fout << m_nodes.at(i)->m_modelID << " " 
			 << m_nodes.at(i)->m_position.x << " " 
			 << m_nodes.at(i)->m_position.y << " "
			 << m_nodes.at(i)->m_position.z << " "
			 << m_nodes.at(i)->m_rotation.x << " " 
			 << m_nodes.at(i)->m_rotation.y << " "
			 << m_nodes.at(i)->m_rotation.z << " "
			 << m_nodes.at(i)->m_scale.x << " " 
			 << m_nodes.at(i)->m_scale.y << " "
			 << m_nodes.at(i)->m_scale.z << "\n";
	}
}

void CPropList::readIn(std::ifstream &fin)
{
	int quantity;

	fin >> quantity;

	for(int i = 0; i < m_nodes.size(); i++)
	{
		delete m_nodes.at(i);
	}

	m_nodes.clear();

	if(m_nodes.size() > 0) 
	{
		int i = m_nodes.size();
		m_nodes.clear();
	}

	for(int i = 0; i < quantity; i++)
	{
		CProp *empty = new CProp();
		m_nodes.push_back(empty);
		m_nodes.back() = new CProp(0);

		fin >> m_nodes.back()->m_modelID
			>> m_nodes.back()->m_position.x
			>> m_nodes.back()->m_position.y
			>> m_nodes.back()->m_position.z
			>> m_nodes.back()->m_rotation.x 
			>> m_nodes.back()->m_rotation.y
			>> m_nodes.back()->m_rotation.z
			>> m_nodes.back()->m_scale.x
			>> m_nodes.back()->m_scale.y
			>> m_nodes.back()->m_scale.z;
	}
}

void CPropList::buildUndo()
{
	//g_undoStack
}

