#ifndef _SCENE_H
#define _SCENE_H

#include "globals.h"
#include "toyBlock.h"
#include "skyBox.h"
#include "fbo.h"
#include "ball.h"
#include "octree.h"

class CScene
{
	public:

		CScene();
		~CScene(){};

		void render();

		void backBufferRender();
		void mainRender();
		void auxRender();

		void floorPlane(float lightness);
		void beginReflectionStencil();
		void endReflectionStencil();

		void shadowMapRender();
		void buildShadowTextureMatrix();
		void renderLitScene();

		void update();

		void addBlock();

		void checkCollisions();

		void buildOctree();


	private:

		std::vector<CToyBlock*> blocks;
		CSkyBox *skyBox;
		CFBO *shadowTexture;
		CBall *ball;

		SMatrix4x4 lightProjectionMatrix;
		SMatrix4x4 lightViewMatrix;

		COctreeNode *octree;
};

#endif