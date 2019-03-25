#ifndef _TOY_BLOCK_H
#define _TOY_BLOCK_H

#include "globals.h"

#define MAX_VELOCITY 300
#define GRAVITY 30

class CToyBlock
{
public:

	CToyBlock(unsigned int id);
	~CToyBlock();

	void render();
	void renderSelectedBox();
	void incrementPositions(SVector3 pos,SVector3 rot, SVector3 scale);

	void execute();
	void gravity();

	void renderAux();
	void renderBackBuffer();

	void select(bool select){m_selected = select;};

	void updatePosition(bool update);
	void updatePosition(SVector3 offset);

	void offsetY(SVector3 offset);

	SVector3 position(){return m_position;};
	SVector3 newPosition(){return m_newPosition;};
	SVector3 scale(){return m_scale;};
	SVector3 rotation(){return m_newRotation;};

	void updateThisRecursion(bool update);
	void collisionBelow(bool update,float offset);
	void ignoreYAxis(bool ignore);

	void clearRenderFlags();
	bool renderedThisPass(){return m_renderedThisPass;};
	void setRenderFlags(bool rendered);

private:

	SVector3 m_position;
	SVector3 m_rotation;
	SVector3 m_newRotation;
	SVector3 m_scale;
	SVector3 m_selectedAxis;
	SVector3 m_newPosition;

	int m_colorId;

	float m_velocity;

	bool m_selected;

	int m_id;

	bool m_updateThisRecursion;
	bool m_collisionBelow;
	float m_collisionBelowOffset;
	bool m_ignoreYAxis;

	bool m_renderedThisPass;
};

#endif