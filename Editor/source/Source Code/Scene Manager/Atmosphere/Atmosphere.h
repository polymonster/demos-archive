#ifndef _ATMOSPHERE_H
#define _ATMOSPHERE_H

#include "globals.h"
#include "SceneObject.h"

extern int g_atmosphereEditMode;

class CAtmosphere : public CSceneObject
{
	public:

		CAtmosphere();
		CAtmosphere(std::ifstream &fin);
		~CAtmosphere();
		void execute();
		void render();

		void skyBox();

		void backBufferRender();
		void renderAux();

		void skyDome();

		void dayNightCycle();

		bool shadowCasterPos();

		void dialogHandler();
		void environmentOptions();

		SMatrix4x4 m_shadowView;
		SMatrix4x4 m_shadowProj;
		SMatrix4x4 m_textureDefault;
		SMatrix4x4 m_currentProj;
		SMatrix4x4 m_currentView;

		bool m_renderBloom;
		bool m_realTimeShadows;
		int m_blurPasses;

		void bindShadowMap();
		void unbindShadowMap();
		void shadowMapBegin();
		void shadowMapEnd();
		bool renderShadowMap();

		void renderToolbox();
		void renderSkyBoxViewer();

		SVector3 m_color;
		float m_luminance;

		SVector3 m_shadowPosition;
		SVector2<float> m_shadowRotation;

		CFBO *m_blur[2];

		void skyBoxOptions();

		void output(std::ofstream &fout);

	private:

		float m_daylight;
		float m_actualLight;
		float m_sunRot;
		float m_time;

		SVector3 m_position;
		SVector3 m_scale;
		SVector3 m_atmosphereColour;
		SVector3 m_selectedAxis;

		bool m_pushed;

		CFBO *m_shadowMap;
		
		float m_shadowOffset;

		int m_shadowMapSize;
		int m_bloomSize;

		bool m_renderInitialShadow;

		int m_cubeMapID;

};

#endif