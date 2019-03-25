#include "octree.h"
#include "helpers.h"
#include "input.h"

int g_octreeEditMode;
int g_octreeMaxObjects = 10;
int g_octreeMinSize = 100;


COctreeNode::COctreeNode(SVector3 pos,SVector3 scale)
{
	nodePos = pos;
	nodeScale = scale;
	built = false;

	m_draw = false;
	m_build = false;
	m_use = false;

	m_pushed[0] = false;
	m_pushed[1] = false;
	m_pushed[2] = false;

	g_octreeEditMode = -1; 
}

void COctreeNode::readIn(std::ifstream &fin)
{
	fin >> nodePos.x >> nodePos.y >> nodePos.z;
	fin >> nodeScale.x >> nodeScale.y >> nodeScale.z;
	fin >> g_octreeMaxObjects >> g_octreeMinSize;
	fin >> m_draw >> m_use;

	m_build = true;
}

void COctreeNode::clear()
{
	objects.clear();
}

void COctreeNode::addObject(CSceneObject *object)
{
	objects.push_back(object);
}

void COctreeNode::subdivide()
{
	built = true;

	children = false;

	int juice = objects.size();

	if(juice > g_octreeMaxObjects && ((nodeScale.x * 0.5f) > g_octreeMinSize))
	{
		childNodes.clear();

		children = true;

		childNodes.resize(8);

		SVector3 childScale = nodeScale * 0.5f;

		SVector3 childPositions[8];
		
		childPositions[0] = SVector3(nodePos.x - childScale.x,nodePos.y - childScale.y,nodePos.z - childScale.z);
		childPositions[1] = SVector3(nodePos.x + childScale.x,nodePos.y - childScale.y,nodePos.z - childScale.z);
		childPositions[2] = SVector3(nodePos.x - childScale.x,nodePos.y - childScale.y,nodePos.z + childScale.z);
		childPositions[3] = SVector3(nodePos.x + childScale.x,nodePos.y - childScale.y,nodePos.z + childScale.z);

		childPositions[4] = SVector3(nodePos.x - childScale.x,nodePos.y + childScale.y,nodePos.z - childScale.z);
		childPositions[5] = SVector3(nodePos.x + childScale.x,nodePos.y + childScale.y,nodePos.z - childScale.z);
		childPositions[6] = SVector3(nodePos.x - childScale.x,nodePos.y + childScale.y,nodePos.z + childScale.z);
		childPositions[7] = SVector3(nodePos.x + childScale.x,nodePos.y + childScale.y,nodePos.z + childScale.z);


		//for all children
		for(int i = 0; i < 8; i++)
		{
			//make a new child
				childNodes.at(i) = new COctreeNode(childPositions[i],childScale);
				childNodes.at(i)->clear();

				for(int j = 0; j < objects.size(); j++)
				{
					//check collisions and fill list
					if(OBBVSOBB(objects.at(j)->boxPos(),objects.at(j)->boxScale(),objects.at(j)->boxOrientation(),
						childNodes.at(i)->nodePos,childNodes.at(i)->nodeScale,SVector3(0,0,0)))
					{
						childNodes.at(i)->addObject(objects.at(j));
					}
				}

				//subdivide
				childNodes.at(i)->subdivide();
		}

		//clear the parents list
		objects.clear();
	}
}

void COctreeNode::render()
{
	//if the octree node is in the view frustum then draw it
	if(g_frustumCulling.CubeInFrustum(nodePos.x,nodePos.y,nodePos.z,nodeScale.x))
	{
		SVector3 camPos = g_input.pos;

		if(childNodes.size() > 0)
		{
			for(int i = 0; i < childNodes.size(); i++)
			{
				childNodes.at(i)->render();
			}

		}
		else
		{
			int lod = 0;
			
			for(int i = 0; i < objects.size(); i++)
			{
				//if the object has not already been rendered
				//by another octree node then draw it
				if(!objects.at(i)->renderedThisPass())
				{
					float d = distance(camPos,objects.at(i)->boxPos());

					lod = 2;

					if(d < 700)
					{
						lod = 1;
					}

					if(d < 400)
					{
						lod = 0;
					}

					//render the object
					objects.at(i)->lod(lod);
					objects.at(i)->render();

					//flag so that this object will not be redundantly rendered
					//by another octree node
					objects.at(i)->setRenderFlags(true);
				}
			}
		}
	}
}

void COctreeNode::clearRenderFlags()
{
	//clear flags so we can track which objects have
	//already been rendered
	if(childNodes.size() > 0)
	{
		for(int i = 0; i < childNodes.size(); i++)
		{
			childNodes.at(i)->clearRenderFlags();
		}

	}
	else
	{
		for(int i = 0; i < objects.size(); i++)
		{
			objects.at(i)->clearRenderFlags();
		}
	}

}

void COctreeNode::checkCollisions()
{
}

void COctreeNode::draw()
{
	glColor3f(0,1,0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
		glTranslated(nodePos.x,nodePos.y,nodePos.z);
		glScaled(nodeScale.x,nodeScale.y,nodeScale.z);
		cube();
	glPopMatrix();

	if(childNodes.size() > 0)
	{
		for(int i = 0; i < childNodes.size(); i++)
		{
			childNodes.at(i)->draw();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void COctreeNode::enumerate()
{
}

void COctreeNode::execute()
{
	if(g_editMode == EDIT_OCTREE)
	{
		ShowWindow(g_hOctree, SW_SHOW);

		HWND view = GetDlgItem(g_hOctree,IDC_VIEW_OCTREE);
		if(SendMessage(view,BM_GETSTATE,0,0) & BST_PUSHED)
		{
			if(!m_pushed[0])
			{
				m_draw = !m_draw;
				m_pushed[0] = true;
			}
		}
		else
		{
			m_pushed[0] = false;
		}

		HWND build = GetDlgItem(g_hOctree,IDC_BUILD_OCTREE);
		if(SendMessage(build,BM_GETSTATE,0,0) & BST_PUSHED)
		{
			if(!m_pushed[1])
			{
				m_build = !m_build;
				m_pushed[1] = true;
			}
		}
		else
		{
			m_pushed[1] = false;
		}

		HWND render = GetDlgItem(g_hOctree,IDC_RENDER_OCTREE);
		if(SendMessage(render,BM_GETSTATE,0,0) & BST_PUSHED)
		{
			if(!m_pushed[2])
			{
				m_use = !m_use;
				m_pushed[2] = true;
			}
		}
		else
		{
			m_pushed[2] = false;
		}

		HWND translate = GetDlgItem(g_hOctree,IDC_OCTREE_TRANSLATE);
		if(SendMessage(translate,BM_GETSTATE,0,0) & BST_PUSHED)
		{
			g_octreeEditMode = TRANSLATE;
		}

		HWND scale = GetDlgItem(g_hOctree,IDC_OCTREE_SCALE);
		if(SendMessage(scale,BM_GETSTATE,0,0) & BST_PUSHED)
		{
			g_octreeEditMode = SCALE;
		}

		char text[255];

		GetDlgItemText(g_hOctree, IDC_OCTREE_OBJECTS,text,255); 
		g_octreeMaxObjects = atoi(text);

		GetDlgItemText(g_hOctree, IDC_OCTREE_NODE_SIZE,text,255); 
		g_octreeMinSize = atoi(text);
	}
	else
	{
		ShowWindow(g_hOctree, SW_HIDE);
	}
}

void COctreeNode::backBufferRender()
{
	if(g_editMode == EDIT_OCTREE)
	{
		switch(g_octreeEditMode)
		{
			case TRANSLATE:
						
					useTranslateWidget(&m_selectedAxis,&nodePos);

			break;

			case SCALE:
						
					useScaleWidget(&m_selectedAxis,&nodePos,&nodeScale);
			break;
		}
	}
}

void COctreeNode::renderAux()
{
	if(g_editMode == EDIT_OCTREE)
	{
		switch(g_octreeEditMode)
		{
			case TRANSLATE:

				translateWidget(nodePos,m_selectedAxis,0);

			break;

			case SCALE:

				translateWidget(nodePos,m_selectedAxis,1);

			break;
		}
	}
}

void COctreeNode::output(std::ofstream &fout)
{
	fout << "octree_params" << "\n";
	fout << nodePos.x << " " << nodePos.y << " " << nodePos.z << "\n";
	fout << nodeScale.x << " " << nodeScale.y << " " << nodeScale.z << "\n";
	fout << g_octreeMaxObjects << " " << g_octreeMinSize << "\n";
	fout << m_draw << " " << m_use << "\n";
}
