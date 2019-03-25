#include	"collectable.h"


Collectable::Collectable(vector2 position,int type, char *name_p) :
	Quad(PATH_SPRITES,name_p,position.x,position.y,8,8,0xffffffff)
{
	set_priority( RENDER_NEAR_FRONT );
	m_active = true;
	m_type = type;
}

Collectable::~Collectable( void )
{

}

void Collectable::execute( void )
{
	m_rotation += 6.0f;
	if( m_rotation >= 360.0f )
		m_rotation = 0.0f;
	Quad::execute();
}

void Collectable::render( void )
{
	if(m_active)
	{
		rotate( m_rotation );
		m_hide = false;
		Quad::render();
	}
	else
	{
		m_hide = true;
	}
}

int Collectable::collect( void )
{
	m_active = false;
	return m_type;
}

