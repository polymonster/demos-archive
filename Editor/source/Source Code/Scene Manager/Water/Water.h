#ifndef WATER_H
#define WATER_H

#include "Globals.h"
#include "SceneObject.h"

extern int g_waterEditMode;
extern int g_selectedWaterPlane;
extern int g_numberOfWaterPlanes;
extern int g_waterSubEditMode;

class CWaterPlane : public CSceneObject
{
	private:

		SVector3 m_position;
		SVector3 m_scale;
		SVector3 m_selectedAxis;
		double m_planeEquation[4];

		SVector2<float> m_reflectionDistortion;
		SVector2<float> m_refractionDistortion;

		SVector2<float> m_animationSpeed;

		SVector2<float> m_textureOffset[2];

		int m_textureID;

		float m_transparency;

	public:

		CWaterPlane(int id);
		~CWaterPlane();

		void execute();
		void render();

		void setToolbarValues();

		void animate();
		
		void backBufferRender();

		void reflectionBegin();
		void reflectionEnd();
		void refractionBegin();
		void refractionEnd();

		void reposition(SVector3 increment);
		void rescale(SVector3 increment);

		void dialogHandler();

		void renderAux();

		void renderToolboxTexture();

		void output(std::ofstream &fout);
		void readIn(std::ifstream &fin);

		CFBO *reflectionFBO;
		CFBO *refractionFBO;

		int m_localReflections;

		int m_id;
};

#endif