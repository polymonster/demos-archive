#ifndef _BALL_H
#define _BALL_H

#include "globals.h"

class CBall
{
	private:

		SVector3 m_position;
		float m_radius;
		bool collided;
		float m_velocity;
		SVector3 m_velocityVector;

		float m_rotation;
		SVector3 m_rotationAxis;

		float m_maxVelocity;

		GLUquadricObj *sphereQuadric;

		int m_textureID;

		SMatrix4x4 rotationMatrix;
		SMatrix4x4 lastRotationMatrix;

		bool m_jump;
		bool m_onEdge;

	public:

		CBall();
		void render();
		void control();
		void update(bool update);
		SVector3 getPosition(){return m_position;};
		float getRadius(){return m_radius;};
		void offset(SVector3 offset);

		void onEdge(bool onEdge){m_onEdge = onEdge;};
};

#endif