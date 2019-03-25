#include "octree.h"
#include "helpers.h"

COctreeNode::COctreeNode(SVector3 pos,SVector3 scale)
{
	nodePos = pos;
	nodeScale = scale;
}

void COctreeNode::clear()
{
	objects.clear();
}

void COctreeNode::addObject(CToyBlock *object)
{
	objects.push_back(object);
}

void COctreeNode::subdivide()
{
	children = false;

	int juice = objects.size();

	if(juice > 6)
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
				if(OBBVSOBB(objects.at(j)->newPosition(),objects.at(j)->scale(),objects.at(j)->rotation(),
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
		if(childNodes.size() > 0)
		{
			for(int i = 0; i < childNodes.size(); i++)
			{
				childNodes.at(i)->render();
			}

		}
		else
		{
				for(int i = 0; i < objects.size(); i++)
				{
					//if the object has not already been rendered
					//by another octree node then draw it
					if(!objects.at(i)->renderedThisPass())
					{
						//increment this value for debug purposes
						g_objectCounter++;

						//render the object
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
	g_objectCounter = 0;

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
	if(childNodes.size() > 0)
	{
		for(int i = 0; i < childNodes.size(); i++)
		{
			childNodes.at(i)->checkCollisions();
		}

	}
	else
	{
		for(int i = 0; i < objects.size(); i++)
		{
			for(int j = 0; j < objects.size(); j++)
			{
				if(i != j)
				{
						SVector3 pos2(0,0,0);
						SVector3 scale2(50,50,50);
						SVector3 rot2(0,0,0);

						if(OBBVSOBB(objects.at(i)->newPosition(),objects.at(i)->scale(),objects.at(i)->rotation(),
						objects.at(j)->newPosition(),objects.at(j)->scale(),objects.at(j)->rotation()))
						{
							objects.at(i)->updateThisRecursion(false);

							SVector3 blockPos = objects.at(i)->newPosition();
							SVector3 colliderPos = objects.at(j)->newPosition();

							if(colliderPos.y < blockPos.y)
							{
								float offset = ((objects.at(i)->newPosition().y - objects.at(i)->scale().y)
												- (objects.at(j)->newPosition().y + objects.at(j)->scale().y));
												
								objects.at(i)->collisionBelow(true,offset - 0.001f);
							}
						}
				}

			}

			
		}

	}
}

void COctreeNode::draw()
{

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
}

void COctreeNode::enumerate()
{
	if(childNodes.size() > 0)
	{
		for(int i = 0; i < childNodes.size(); i++)
		{
			childNodes.at(i)->enumerate();
		}
	}
	else
	{
		std::ofstream fout;

		fout.open("octree nodes.txt");

		char msg[2048];
		char buf[32];

		itoa(objects.size(),buf,10);

		strcpy(msg,"");
		strcat(msg,buf);
		strcat(msg,"\n");

		/*for(int i = 0; i < objects.size(); i++)
		{
			fout << objects.at(i)->newPosition().x << " , ";
			fout << objects.at(i)->newPosition().y << " , ";
			fout << objects.at(i)->newPosition().z << " , ";
			fout << "\n";
		}*/

		MessageBox(g_hWnd, msg, "Output", MB_OK);
	}
}
