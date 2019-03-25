#include "particles.h"	
#include "myMath.h"
#include "timerSingleton.h"

CParticles::CParticles(){

	texId[0] = 0;
	texId[1] = 0;
	texId[2] = 0;
	texId[3] = 0;
	texId[4] = 0;
	texId[5] = 0;

	m_activeParticles = 0;

	inUse = false;

	for (int i=0;i<MAX_PARTICLES;i++)				// Initials All The Textures
	{
		particle[i].active = false;
	}
};

void CParticles::initRicochet(SVector3 pos,int type){

	slowdown = 4.0f;

	m_type = type;

	int i;

	switch(type)
	{
	case 0: //machine gun

		for (i=0;i<MAX_RICOCHET_PARTICLES;i++)				// Initials All The Textures
		{
			particle[i].active=true;								// Make All The Particles Active
			particle[i].life=1.0f;								// Give All The Particles Full Life
			particle[i].fade= 0.05;	// Random Fade Value
			particle[i].r=colors[1][0];
			particle[i].g=colors[1][1];
			particle[i].b=colors[1][2];
			particle[i].xdir=float((rand()%50)-26.0f)*4.0f;		// Random Speed On X Axis
			particle[i].ydir=float((rand()%50)-25.0f)*4.0f;		// Random Speed On Y Axis
			particle[i].zdir=float((rand()%50)-25.0f)*4.0f;		// Random Speed On Z Axis
			particle[i].xg=  0.0f;									// Set Horizontal Pull To Zero
			particle[i].yg= -7.0f;									// Set Vertical Pull Downward
			particle[i].zg=  0.0f;									// Set Pull On Z Axis To Zero
			particle[i].x= pos.x; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On X Axis
			particle[i].y= pos.y; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On Y Axis
			particle[i].z= pos.z;// Center On Z Axis
		}

	break;

	case 3: // plasma

		for (i=0;i<MAX_RICOCHET_PARTICLES;i++)				// Initials All The Textures
		{
			particle[i].active=true;								// Make All The Particles Active
			particle[i].life=1.0f;								// Give All The Particles Full Life
			particle[i].fade= 0.02;	// Random Fade Value
			particle[i].r=colors[4][0];
			particle[i].g=colors[4][1];
			particle[i].b=colors[4][2];
			particle[i].xdir=float((rand()%50)-26.0f)*1.0f;		// Random Speed On X Axis
			particle[i].ydir=float((rand()%50)-25.0f)*1.0f;		// Random Speed On Y Axis
			particle[i].zdir=float((rand()%50)-25.0f)*1.0f;		// Random Speed On Z Axis
			particle[i].xg=  0.0f;									// Set Horizontal Pull To Zero
			particle[i].yg=  0.0f;									// Set Vertical Pull Downward
			particle[i].zg=  0.0f;									// Set Pull On Z Axis To Zero
			particle[i].x= pos.x; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On X Axis
			particle[i].y= pos.y; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On Y Axis
			particle[i].z= pos.z;// Center On Z Axis
		}

	break;

	case 2: // shotgun

		for (i=0;i<MAX_RICOCHET_PARTICLES;i++)				// Initials All The Textures
		{
			particle[i].active=true;								// Make All The Particles Active
			particle[i].life=1.0f;								// Give All The Particles Full Life
			particle[i].fade= 0.4;	// Random Fade Value
			particle[i].r=colors[1][0];
			particle[i].g=colors[1][1];
			particle[i].b=colors[1][2];
			particle[i].xdir=float((rand()%50)-26.0f)*5.0f;		// Random Speed On X Axis
			particle[i].ydir=float((rand()%50)-25.0f)*5.0f;		// Random Speed On Y Axis
			particle[i].zdir=float((rand()%50)-25.0f)*5.0f;		// Random Speed On Z Axis
			particle[i].xg=  0.0f;									// Set Horizontal Pull To Zero
			particle[i].yg=  7.0f;									// Set Vertical Pull Downward
			particle[i].zg=  0.0f;									// Set Pull On Z Axis To Zero
			particle[i].x= pos.x; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On X Axis
			particle[i].y= pos.y; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On Y Axis
			particle[i].z= pos.z;// Center On Z Axis
		}

	break;

	case 1: // plasma

		for (i=0;i<MAX_RICOCHET_PARTICLES;i++)				// Initials All The Textures
		{
			particle[i].active=true;								// Make All The Particles Active
			particle[i].life=1.0f;								// Give All The Particles Full Life
			particle[i].fade= 0.4;	// Random Fade Value
			particle[i].r=colors[1][0];
			particle[i].g=colors[1][1];
			particle[i].b=colors[1][2];
			particle[i].xdir=float((rand()%50)-26.0f)*5.0f;		// Random Speed On X Axis
			particle[i].ydir=float((rand()%50)-25.0f)*5.0f;		// Random Speed On Y Axis
			particle[i].zdir=float((rand()%50)-25.0f)*5.0f;		// Random Speed On Z Axis
			particle[i].xg=  0.0f;									// Set Horizontal Pull To Zero
			particle[i].yg=  7.0f;									// Set Vertical Pull Downward
			particle[i].zg=  0.0f;									// Set Pull On Z Axis To Zero
			particle[i].x= pos.x; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On X Axis
			particle[i].y= pos.y; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On Y Axis
			particle[i].z= pos.z;// Center On Z Axis
		}

	break;


	}

	inUse = true;

}

void CParticles::initMuzzle(SVector3 pos){

		particle[0].active=true;
		particle[0].life=1.0f;
		particle[0].x= pos.x;
		particle[0].y= pos.y; 
		particle[0].z= pos.z;
		particle[0].fade = 0.07f;
		particle[0].r=colors[1][0];
		particle[0].g=colors[1][1];
		particle[0].b=colors[1][2];

}

void CParticles::muzzleFlash(){
	
	if(particle[0].active)
	{
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
		//CTextureMan::getInstance()->bindTexture(texId[3]);

		glPushMatrix();
			glTranslated(particle[0].x,particle[0].y,particle[0].z);
			billboardBegin();
			glColor4f(particle[0].r,particle[0].g,particle[0].b,particle[0].life); 
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			    glTexCoord2d(1,1); glVertex2f(15,15); // Top Right
				glTexCoord2d(0,1); glVertex2f(-15,15); // Top Left
				glTexCoord2d(1,0); glVertex2f(15,-15); // Bottom Right
				glTexCoord2d(0,0); glVertex2f(-15,-15); // Bottom Left
			glEnd();
			billboardEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		
	}

}

void CParticles::ricochetParticles(){

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-20,20);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	int size = 5;

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);		

	// Enable Texture Mapping
	if((m_type == 0) || (m_type == 1)) CTextureMan::getInstance()->bindParticle(5);
	if(m_type == 2) CTextureMan::getInstance()->bindParticle(2);
	if(m_type == 3) 
	{
		CTextureMan::getInstance()->bindParticle(5);
		size = 100;
	}

	for (int i=0;i<MAX_RICOCHET_PARTICLES;i++)					// Loop Through All The Particles
	{
		if(particle[i].active)							// If The Particle Is Active
		{
			float x=particle[i].x;						// Grab Our Particle X Position
			float y=particle[i].y;						// Grab Our Particle Y Position
			float z=particle[i].z;						// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glPushMatrix();
			glTranslated(x,y,z);
			billboardBegin();
			glColor4f(particle[i].r,particle[i].g,particle[i].b,particle[i].life); 
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			    glMultiTexCoord2f(GL_TEXTURE1_ARB,1,1); glVertex2f(size,size); // Top Right
				glMultiTexCoord2f(GL_TEXTURE1_ARB,0,1); glVertex2f(-size,size); // Top Left
				glMultiTexCoord2f(GL_TEXTURE1_ARB,1,0); glVertex2f(size,-size); // Bottom Right
				glMultiTexCoord2f(GL_TEXTURE1_ARB,0,0); glVertex2f(-size,-size); // Bottom Left
			glEnd();
			billboardEnd();
			glPopMatrix();
		}
    }
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glDisable(GL_POLYGON_OFFSET_FILL);
	
}

void CParticles::updateRicochet(){

	inUse = false;

	float animSpeed = CTimer::getInstance()->animBaseSpeed();

	for (int i=0;i<MAX_RICOCHET_PARTICLES;i++)					// Loop Through All The Particles
	{
		if(particle[i].active)							// If The Particle Is Active
		{
			inUse = true;
			// Done Building Triangle Strip
			particle[i].x+=particle[i].xdir*animSpeed/(slowdown);// Move On The X Axis By X Speed
			particle[i].y+=particle[i].ydir*animSpeed/(slowdown);// Move On The Y Axis By Y Speed
			particle[i].z+=particle[i].zdir*animSpeed/(slowdown);// Move On The Z Axis By Z Speed
			particle[i].xdir+=particle[i].xg*animSpeed/(slowdown);			// Take Pull On X Axis Into Account
			particle[i].ydir+=particle[i].yg*animSpeed/(slowdown);			// Take Pull On Y Axis Into Account
			particle[i].zdir+=particle[i].zg*animSpeed/(slowdown);			// Take Pull On Z Axis Into Account

			particle[i].life-=animSpeed * 2;			// Reduce Particles Life By 'Fade'

			if(particle[i].life < 0)
			{
				particle[i].active = false;
			}

		}
    }
}

void CParticles::initBulletHole(SVector3 pos, SVector3 poly[]){

	SVector3 offset = getNormal(poly);

	pos = pos - offset;

	particle[m_activeParticles].active=true;
	particle[m_activeParticles].x = pos.x;
	particle[m_activeParticles].y = pos.y;
	particle[m_activeParticles].z = pos.z;

	particle[m_activeParticles].life=1;
	particle[m_activeParticles].fade=0.001;
	
	SVector3 vectors[4];

	for(int i = 0; i < 4; i++)
	{
		particle[m_activeParticles].particlePoly[i] = normalize(pos - poly[i]) * 5;
		particle[m_activeParticles].poly[i] = poly[i];
	}

	m_activeParticles++;

	if(m_activeParticles >= 50)
	{
		m_activeParticles = 0;
	}

}

void CParticles::bulletHole(){

		for(int i = 0; i < MAX_PARTICLES; i++)
		{
			if(particle[i].active)
			{
				glDisable(GL_LIGHTING);
				//do stencil test for the poly it is colliding with
				//set the stenicl mask area (polygon)
				glEnable(GL_STENCIL_TEST);						
				glStencilFunc(GL_ALWAYS, 1, 1);				
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);		
																
				glDisable(GL_DEPTH_TEST);						

				glColor4f(1.0f, 1.0f, 1.0f,0.0f);				
				glBegin(GL_QUADS);							
					glVertex3f(particle[i].poly[0].x,particle[i].poly[0].y,particle[i].poly[0].z); 
					glVertex3f(particle[i].poly[1].x,particle[i].poly[1].y,particle[i].poly[1].z);
					glVertex3f(particle[i].poly[2].x,particle[i].poly[2].y,particle[i].poly[2].z);
					glVertex3f(particle[i].poly[3].x,particle[i].poly[3].y,particle[i].poly[3].z);
				glEnd();

				//set the stencil mask area then draw the particle in the stencil
				glEnable(GL_DEPTH_TEST);						
				glColorMask(1,1,1,1);								
				glStencilFunc(GL_EQUAL, 1, 1);											
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		

				glBlendFunc(GL_SRC_ALPHA,GL_ONE);

				glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
				CTextureMan::getInstance()->bindParticle(6);

				glPushMatrix();

				glTranslated(particle[i].x,particle[i].y,particle[i].z);

				glColor4f(1,1,1,particle[i].life); 

				glPolygonOffset(10.0f,10.0f);

					glBegin(GL_QUADS);						
						glTexCoord2d(0,0); glVertex3f(particle[i].particlePoly[0].x,particle[i].particlePoly[0].y,particle[i].particlePoly[0].z); 
						glTexCoord2d(0,1); glVertex3f(particle[i].particlePoly[1].x,particle[i].particlePoly[1].y,particle[i].particlePoly[1].z);
						glTexCoord2d(1,1); glVertex3f(particle[i].particlePoly[2].x,particle[i].particlePoly[2].y,particle[i].particlePoly[2].z);
						glTexCoord2d(1,0); glVertex3f(particle[i].particlePoly[3].x,particle[i].particlePoly[3].y,particle[i].particlePoly[3].z);
					glEnd();

				glPopMatrix();


				glDisable(GL_TEXTURE_2D);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_LIGHTING);
				glDisable(GL_STENCIL_TEST);	
			}
		}
}

void CParticles::update(){

	inUse = false;
	for (int i=0;i<MAX_PARTICLES;i++)					// Loop Through All The Particles
	{
		if(particle[i].active)							// If The Particle Is Active
		{
			inUse = true;
			particle[i].life -= CTimer::getInstance()->animBaseSpeed();

			if(particle[i].life < 0)
			{
				particle[i].active = false;
			}

		}
    }
}

void CParticles::initBlood(SVector3 pos){

	slowdown = 4.0f;
	inUse = true;

	for (int i=0;i<MAX_BLOOD_PARTICLES;i++)				// Initials All The Textures
	{
		particle[i].active=true;								// Make All The Particles Active
		particle[i].life=1.0f;								// Give All The Particles Full Life
		particle[i].fade= 0.02;	// Random Fade Value
		//particle[i].r=colors[1][0];
		//particle[i].g=colors[1][1];
		//particle[i].b=colors[1][2];
		particle[i].xdir=float((rand()%50)-26.0f)*10.0f;		// Random Speed On X Axis
		particle[i].ydir=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Y Axis
		particle[i].zdir=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Z Axis
		particle[i].xg=  0.0f;									// Set Horizontal Pull To Zero
		particle[i].yg= -7.0f;									// Set Vertical Pull Downward
		particle[i].zg=  0.0f;									// Set Pull On Z Axis To Zero
		particle[i].x= pos.x; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On X Axis
		particle[i].y= pos.y; //+= float ( ( (rand()%50) / 100.0f)  - 0.2f );						// Center On Y Axis
		particle[i].z= pos.z;																		// Center On Z Axis
	}

}


void CParticles::bloodParticles(){

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);

	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	CTextureMan::getInstance()->bindParticle(2);

	for (int i=0;i<MAX_BLOOD_PARTICLES;i++)					// Loop Through All The Particles
	{
		if(particle[i].active)							// If The Particle Is Active
		{
			float x=particle[i].x;						// Grab Our Particle X Position
			float y=particle[i].y;						// Grab Our Particle Y Position
			float z=particle[i].z;						// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glPushMatrix();
			glTranslated(x,y,z);
			billboardBegin();
			glColor4f(0.5,0.1,0.1,0); 
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			    glTexCoord2d(1,1); glVertex2f(2,2); // Top Right
				glTexCoord2d(0,1); glVertex2f(-2,2); // Top Left
				glTexCoord2d(1,0); glVertex2f(2,-2); // Bottom Right
				glTexCoord2d(0,0); glVertex2f(-2,-2); // Bottom Left
			glEnd();
			billboardEnd();
			glPopMatrix();

		}
    }
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
	
}

void CParticles::updateBlood(){

	inUse = false;

	float animSpeed = CTimer::getInstance()->animBaseSpeed();

	for (int i=0;i<MAX_PARTICLES;i++)					// Loop Through All The Particles
	{
		if(particle[i].active)							// If The Particle Is Active
		{
			inUse = true;
			// Done Building Triangle Strip
			particle[i].x+=particle[i].xdir*animSpeed/(slowdown);// Move On The X Axis By X Speed
			particle[i].y+=particle[i].ydir*animSpeed/(slowdown);// Move On The Y Axis By Y Speed
			particle[i].z+=particle[i].zdir*animSpeed/(slowdown);// Move On The Z Axis By Z Speed

			particle[i].xdir+=particle[i].xg*animSpeed;			// Take Pull On X Axis Into Account
			particle[i].ydir+=particle[i].yg*animSpeed;			// Take Pull On Y Axis Into Account
			particle[i].zdir+=particle[i].zg*animSpeed;			// Take Pull On Z Axis Into Account

			particle[i].life-=particle[i].fade;			// Reduce Particles Life By 'Fade'

			if(particle[i].life <= 0) particle[i].active = false;

		}
    }
}

