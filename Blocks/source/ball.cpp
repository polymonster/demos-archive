#include "ball.h"
#include "input.h"
#include "textureSingleton.h"
#include "myMath.h"

bool firstTime = true;

CBall::CBall()
{
	m_position = SVector3(0,0,0);
	m_radius = 8;
	collided = false;
	m_jump = true;
	m_velocity = 0;
	m_velocityVector = SVector3(0,0,0);
	m_maxVelocity = 1.5f;
	m_rotation = 0;
	m_rotationAxis = SVector3(0,0,0);

	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX,rotationMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,lastRotationMatrix);
	glPopMatrix();

	m_textureID = CTextureMan::getInstance()->load("data/texture/ball.tga");
}

void CBall::render()
{

	glPushMatrix();
	glLoadIdentity();
	glRotated(m_rotation,m_rotationAxis.x,0,m_rotationAxis.z);
	glGetFloatv(GL_MODELVIEW_MATRIX,rotationMatrix);
	glPopMatrix();

	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	
	glColor3f(1,0,0);

	if(!collided) glColor3f(1,0,1);

	CTextureMan::getInstance()->bindTexture(m_textureID,1);

	glPushMatrix();

	glTranslated(m_position.x,m_position.y,m_position.z);

	glMultMatrixf(rotationMatrix);

	glScaled(m_radius,m_radius,m_radius);

	sphere();

	glPopMatrix();
	
	glEnable(GL_LIGHTING);
}

void CBall::control()
{
	inputReader input = user::getInstance()->input;

	float rotrad = degreesToRadians(input.rot.y);

	SVector3 direction(0,0,0);

	bool move = false;

	if((input.key['W']) || (input.gamepad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
	{
		//FORWARDS
		direction.x += sin(rotrad);
		direction.z -= cos(rotrad);
		move = true;
	}

	if((input.key['S']) || (input.gamepad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
	{
		//BACKWARDS
		direction.x -= sin(rotrad);
		direction.z += cos(rotrad);
		move = true;
	}

	if((input.key['A']) || (input.gamepad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
	{
		//STRAFE LEFT
		direction.x -= cos(rotrad);
		direction.z -= sin(rotrad);
		move = true;
	}

	if((input.key['D']) || (input.gamepad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		//STRAFE RIGHT
		direction.x += cos(rotrad);
		direction.z += sin(rotrad);
		move = true;
	}

	if((input.gamepad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) || (input.key[32]))
	{
		if(m_jump)
		{
			direction.y = 150.0f;
			//move = true;
			m_jump = false;
		}
	}

	if((input.gamepad.state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
     input.gamepad.state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
    (input.gamepad.state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
     input.gamepad.state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
	{	
		input.gamepad.state.Gamepad.sThumbLX = 0;
		input.gamepad.state.Gamepad.sThumbLY = 0;
	}
	else
	{
		SVector3 thumbDir;

		thumbDir.x = input.gamepad.state.Gamepad.sThumbLX / 32767.0f;
		thumbDir.z = -input.gamepad.state.Gamepad.sThumbLY / 32767.0f;

		SVector3 rotated = rotateY(thumbDir,SVector3(0,0,0),input.rot.y);

		direction.x = rotated.x;
		direction.z = rotated.z;

		move = true;
	}

	if(move) 
	{
		m_rotationAxis.x = m_velocityVector.z;
		m_rotationAxis.y = 0;
		m_rotationAxis.z = -m_velocityVector.x;
	}

	if(m_velocityVector.x > m_maxVelocity) m_velocityVector.x = m_maxVelocity;
	if(m_velocityVector.z > m_maxVelocity) m_velocityVector.z = m_maxVelocity;
	if(m_velocityVector.x < -m_maxVelocity) m_velocityVector.x = -m_maxVelocity;
	if(m_velocityVector.z < -m_maxVelocity) m_velocityVector.z = -m_maxVelocity;

	m_velocityVector += direction * 0.015f;

	m_rotation += absolute(m_velocityVector.x * 3) + absolute(m_velocityVector.z) * 5;

	m_position += m_velocityVector;

	//APPLY GRAVITY
	m_velocityVector.y -= 0.07f;
	if(m_velocityVector.y < -5.0) m_velocityVector.y = -5.0;

	//SLOW DOWN VELOCITY X
	if(m_velocityVector.x != 0) 
	{
		if(m_velocityVector.x < 0)
		{
			m_velocityVector.x += 0.005f;

			if(m_velocityVector.x > 0) m_velocityVector.x = 0;
		}
		else
		{
			m_velocityVector.x -= 0.005f;

			if(m_velocityVector.x < 0) m_velocityVector.x = 0;
		}
	}

	//SLOW DOWN VELOCITY Z
	if(m_velocityVector.z != 0) 
	{
		if(m_velocityVector.z < 0)
		{
			m_velocityVector.z += 0.005f;

			if(m_velocityVector.z > 0) m_velocityVector.z = 0;
		}
		else
		{
			m_velocityVector.z -= 0.005f;

			if(m_velocityVector.z < 0) m_velocityVector.z = 0;
		}
	}
	
	//CHECK FOR COLLISION WITH THE FLOOR
	//bottom of the sphere
	float extent = (m_position.y - m_radius);
	if(extent < 0) 
	{
		//if we have a collision offset the y position
		m_position.y -= extent;

		m_velocityVector.y = 0;

		//allow the user to jump again
		m_jump = true;
	}
}

void CBall::update(bool update)
{
	collided = update;
	if(collided) m_velocityVector = m_velocityVector * -0.4;
}

void CBall::offset(SVector3 offset)
{
	m_position += offset;

	if(offset.y > 0) 
	{
		if(m_onEdge)
		{
			m_velocityVector.y = -1.0f; 
		}
		else
		{
			if(offset.x == 0 && offset.z == 0)
			{
				m_velocityVector.y = 0.0f;
			}
		}

		m_jump = true;
	}

	if((offset.x != 0) && (absolute(m_velocityVector.x) > 0.5)) m_velocityVector.x *= 0.5f;
	if((offset.z != 0) && (absolute(m_velocityVector.z) > 0.5)) m_velocityVector.z *= 0.5f;
}
