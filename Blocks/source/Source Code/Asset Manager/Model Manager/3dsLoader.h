#ifndef _3DSLOADER_H
#define _3DSLOADER_H

#include "Globals.h"
#include "3ds.h"
#include "myMath.h"

//////////////////////////////////////
//The 3dsLoader Class
//////////////////////////////////////
class C3dsLoader
{
	public:

		CLoad3DS mLoad3ds;	
		
		int textureArrayManager[32];
		int normalMapArrayManager[32];

		t3DModel m3DModel;	

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

		void calculateTangentArray();
		void calculateNormals();

		int m_polyCount;

		void initVBO();
		void drawVBO();
		void updateVerts();
		void deleteVBO();

		SVector3 *vboVertices;
		SVector3 *vboNormals;
		SVector3 *vboTangents;
		SVector2<float> *vboTextureCoords;

		unsigned int vboID;
		unsigned int colID;
		unsigned int texID;
		unsigned int normalID;

		int vertexCount;

		void drawNormals();
		void drawTangents();
};



#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
