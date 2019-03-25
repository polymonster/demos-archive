#ifndef _ATMOSPHERE_H
#define _ATMOSPHERE_H

#include "globals.h"

class CSkyBox
{
	public:

		void execute(){};
		void render();

	private:

	SVector3 m_position;
	SVector3 m_scale;

};

#endif