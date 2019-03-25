#ifndef _3DSLOADER_H
#define _3DSLOADER_H

#include "globals.h"
#include "3ds.h"
#include "myMath.h"

//////////////////////////////////////
//The 3dsLoader Class
//////////////////////////////////////
class C3dsLoader
{
	public:

		CLoad3DS mLoad3ds;	
		
		int textureArrayManager[10];
		t3DSModel m3DModel;	

		void Init_3ds(char* filename);
		void Render_3ds();
		void RenderNoTexture();
		void Texture_3ds(UINT textureArray[], LPSTR strFileName, int ID);
		void Unload_3ds();
		int returnClosestVert(SVector3 pos);
		void getVerts(int face,SVector3 &p1, SVector3 &p2, SVector3 &p3, SVector3 &p4);
		void translateToVert(int i);
		void renderMappedQuad();
		int returnNumberFaces();
		SVector3 getModelsNormal(int face);

		int m_polyCount;
};


#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
