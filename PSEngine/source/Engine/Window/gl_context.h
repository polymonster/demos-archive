#ifndef gl_context_h__
#define gl_context_h__

#include <Windows.h>
#include "common.h"

class PSRenderer
{
public:

	PSRenderer(HWND window);
	~PSRenderer();

	void swap_buffers();
	void make_active();

	void size_viewport(Vector2i new_size);

private:

	HGLRC		m_render_context;
	HDC			m_device_context;

};

#endif // gl_context_h__