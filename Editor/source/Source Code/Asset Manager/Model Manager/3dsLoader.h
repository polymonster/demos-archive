#ifndef _3DSLOADER_H
#define _3DSLOADER_H

#include "Globals.h"
#include "3ds.h"
#include "myMath.h"


class C3dsLoader
{
	public:

		//CLoad3DS mLoad3ds;

		t3DModel m3DModel;	

		C3dsLoader(){};
		C3dsLoader(t3DModel model,char* filename);

		int textureArrayManager[32];
		int normalMapArrayManager[32];

		void Unload_3ds();

		int returnClosestVert(SVector3 pos);

		void getVerts(int face,SVector3 &p1, SVector3 &p2, SVector3 &p3, SVector3 &p4);

		void translateToVert(int i);

		int returnNumberFaces();

		void calculateTangentArray();
		void calculateNormals();

		void initVBO();
		void drawVBO();
		void renderPicking(colorByte color);
		void deleteVBO();

		SVector3 *vboVertices;
		SVector3 *vboNormals;
		SVector3 *vboTangents;
		SVector2<float> *vboTextureCoords;

		unsigned int vboID;
		unsigned int colID;
		unsigned int texID;
		unsigned int normalID;

		void drawNormals();
		void drawTangents();

		void drawBB();

		SVector3 m_boundingBoxPos;
		SVector3 m_boundingBoxDimensions;

		void getBox(SVector3 *pos,SVector3 *scale);

		int m_polyCount;
		int vertexCount;
};



#endif
