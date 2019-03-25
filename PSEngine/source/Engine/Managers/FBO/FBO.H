#ifndef _FBO_H
#define _FBO_H

#define COLOUR 0
#define VARIANCE_SHADOW 1
#define DEPTH_SHADOW 2
#define GBUFFER 3
#define COLOUR_FLOAT 4
#define COLOUR_HALF_FLOAT 5

#include "window.h" 

class FBO
{
	private:

		unsigned int  m_fbo;
		unsigned int  m_depth_buffer;
		unsigned int  m_texture_count;
		unsigned int  *m_storage_texture;

		unsigned int create_texture(int channel_type, int channel_format, int data_type);
		void create_render_buffer();

		int m_sizex;
		int m_sizey;

	public:

		FBO(int type,int xsize, int ysize, bool render_buffer);
		~FBO();

		void bind(bool clear_depth = true, bool clear_colour = true);
		void bind(float alpha, bool clear_depth = true, bool clear_colour = true);
		void bind_clear_depth_to_max();

		void bind_mrt(int count);
		void unbind();

		void blit_depth_to_backbuffer();

		void bind_texture(int unit);
		void bind_texture(int unit, int attachment);

		unsigned int Fbo() const { return m_fbo; }
};

#endif