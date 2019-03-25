/*=========================================================*\
|	object.h - generic scene object
|-----------------------------------------------------------|
|				Project :	PolySpoon Engine
|				Coder	:	ABleas
|				Date	:	16/09/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/

/*======================== INCLUDES =======================*/

#include "console.h"
#include "helpers.h"


/*======================== CONSTANTS ======================*/


/*========================= CLASSES =======================*/

class Object
{

public:

	virtual void update() = 0;
	virtual void render() = 0;
	virtual void physics_world_render() = 0;
	
private:


};