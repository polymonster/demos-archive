#include "FBO.h"
#include "texture_manager.h"

void FBO::create_render_buffer()
{
	glGenRenderbuffersEXT(1, &m_depth_buffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depth_buffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_sizex, m_sizey);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depth_buffer);
}

unsigned int FBO::create_texture(int channel_type, int channel_format, int data_type)
{
	unsigned int handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, channel_type, m_sizex, m_sizey, 0, channel_format, data_type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return handle;
}

FBO::FBO(int type,int xsize, int ysize, bool render_buffer)
{
	//set the size of the fbo
	m_sizex = xsize;
	m_sizey = ysize;
	m_texture_count = 0;

	//create an fbo
	glGenFramebuffersEXT(1, &m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	switch(type)
	{
		case COLOUR :

			m_storage_texture = new unsigned int[1];
			m_storage_texture[m_texture_count++] = create_texture(GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE);

			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_storage_texture[0], 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		break;

		case COLOUR_FLOAT :
		case COLOUR_HALF_FLOAT:

			m_storage_texture = new unsigned int[1];
			if(type == COLOUR_FLOAT) m_storage_texture[m_texture_count++] = create_texture(GL_RGBA32F,GL_RGBA,GL_FLOAT);
			else m_storage_texture[m_texture_count++] = create_texture(GL_RGBA16F,GL_RGBA,GL_FLOAT);

			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_storage_texture[0], 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			break;

		case VARIANCE_SHADOW :
		{
			m_storage_texture = new unsigned int[1];
			m_storage_texture[m_texture_count++] = create_texture(GL_RGB16F,GL_RGB,GL_FLOAT);

			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *m_storage_texture, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float full_depth[4] = {1,1,1,1};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, full_depth);
		}
		break;

		case DEPTH_SHADOW :
		{
			//create a depth shadow texture (D24)
			m_storage_texture = new unsigned int[1];
			m_storage_texture[m_texture_count++] = create_texture(GL_DEPTH_COMPONENT32,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );

			glDrawBuffer(GL_FALSE);
			glReadBuffer(GL_FALSE);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, *m_storage_texture, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float full_depth[4] = {1,1,1,1};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, full_depth);

			break;
		}
		case GBUFFER :

			//create a new array of textures
			m_storage_texture = new unsigned int[4];

			for(int i = 0; i < 4; i++)
			{
				m_storage_texture[m_texture_count++] = create_texture(GL_RGBA16F,GL_RGBA,GL_FLOAT);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, m_storage_texture[i], 0);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}

		break;
	}

	if(render_buffer)
	{
		create_render_buffer();
	}

	//if there are any errors creating, terminate the program
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) 
	{
		g_console_p->dbgmsg("could not create FBO: %d, %d / %d, %d", xsize, ysize, type, render_buffer);
		PostQuitMessage(0);
	}

	//unbind the frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

FBO::~FBO()
{
	glDeleteFramebuffersEXT(1, &m_fbo);
	glDeleteRenderbuffersEXT(1, &m_depth_buffer);

	//delete textures
	glDeleteTexturesEXT( m_texture_count , m_storage_texture );

}

void FBO::bind_texture(int unit)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,m_storage_texture[0]);

	g_texture_mgr.invalidate_unit(unit,false);
}
void FBO::bind_texture(int unit, int attachment)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,m_storage_texture[attachment]);

	g_texture_mgr.invalidate_unit(unit,false);
}

void FBO::bind(bool clear_depth, bool clear_colour)
{
	//bind the FBO
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_fbo);

	//clear the screen like usual
	if(clear_depth) glClear(GL_DEPTH_BUFFER_BIT);
	if(clear_colour) glClear(GL_COLOR_BUFFER_BIT);

	//save the old viewport size
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//resize the viewport to the size of the FBO
	glViewport(0,0,m_sizex,m_sizey);
}

void FBO::bind(float alpha, bool clear_depth, bool clear_colour)
{
	//bind the FBO
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_fbo);

	//clear the screen like usual
	glClearColor(0.0,0.0,0.0,alpha);
	if(clear_depth) glClear(GL_DEPTH_BUFFER_BIT);
	if(clear_colour) glClear(GL_COLOR_BUFFER_BIT);

	//save the old viewport size
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//resize the viewport to the size of the FBO
	glViewport(0,0,m_sizex,m_sizey);
}

void FBO::bind_clear_depth_to_max()
{
	//bind the FBO
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_fbo);

	//clear the screen like usual
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//save the old viewport size
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//resize the viewport to the size of the FBO
	glViewport(0,0,m_sizex,m_sizey);
}

void FBO::blit_depth_to_backbuffer()
{
	//begin
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
	glClear (GL_DEPTH_BUFFER_BIT);

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_fbo);

	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);

	glBlitFramebufferEXT(0, 0, 1024, 768,0, 0, 1024, 768,
	GL_DEPTH_BUFFER_BIT,
	GL_NEAREST);

	//end
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FBO::bind_mrt(int count)
{
	GLenum *targets = new GLenum[ count ];

	for(int i = 0; i < count; i++)
	{
		targets[i] = GL_COLOR_ATTACHMENT0_EXT + i;
	}

	//bind the FBO
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT , m_fbo);

	//save the old viewport size
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//resize the viewport to the size of the FBO
	glViewport(0,0,m_sizex,m_sizey);

	glDrawBuffers(count, targets);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	delete targets;
}

void FBO::unbind()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}