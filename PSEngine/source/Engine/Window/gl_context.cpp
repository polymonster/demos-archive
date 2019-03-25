#include "gl_context.h"
#include "common.h"

//#define _USING_GL_3

PSRenderer::PSRenderer( HWND window )
{
	int pixel_format = 0;
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,						// Version Number
		PFD_DRAW_TO_WINDOW |	// Format Must Support Window
		PFD_SUPPORT_OPENGL |	// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,		// Must Support Double Buffering
		PFD_TYPE_RGBA,			// Request An RGBA Format
		32,						// 32 bpp
		0, 0, 0, 0, 0, 0,		// Color Bits Ignored
		8,						// 8 bit Alpha Buffer //IMPORTANT ATI will give this by default
		0,						// Shift Bit Ignored
		0,						// No Accumulation Buffer
		0, 0, 0, 0,				// Accumulation Bits Ignored
		24,						// 24 bit Z-Buffer (Depth Buffer) //IMPORTANT (32 does not exist, stop changing this)
		8,						// 8 bit Stencil Buffer //IMPORTANT ATI will give this by default
		0,						// No Auxiliary Buffer
		PFD_MAIN_PLANE,			// Main Drawing Layer
		0,						// Reserved
		0, 0, 0					// Layer Masks Ignored
	};

	m_device_context = GetDC( window );
	pixel_format = ChoosePixelFormat( m_device_context, &pfd );
	SetPixelFormat( m_device_context, pixel_format, &pfd );

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		GL_CONTEXT_PROFILE_MASK, GL_CONTEXT_COMPATIBILITY_PROFILE_BIT,
		0
	};

	HGLRC temp_context = wglCreateContext( m_device_context );
	wglMakeCurrent( m_device_context, temp_context );

	#ifdef _USING_GL_3

		m_render_context = wglCreateContextAttribsARB(m_device_context , 0 , attribs);

		wglMakeCurrent( NULL, NULL );
		
		if(m_render_context->unused != 0)
		{
			m_render_context = temp_context;
		}
		else
		{
			wglDeleteContext(temp_context);
		}

		wglMakeCurrent(m_device_context,m_render_context);

	#else

		m_render_context = temp_context;

	#endif

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	swap_buffers();
}

PSRenderer::~PSRenderer()
{
	if( m_render_context )
	{
		if( !wglMakeCurrent( NULL, NULL ) )
			MessageBoxA( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

		if( !wglDeleteContext( m_render_context ) )
			MessageBoxA( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

		m_render_context = NULL;
	}
}

void PSRenderer::swap_buffers()
{
	SwapBuffers(m_device_context);
}

void PSRenderer::make_active()
{
	wglMakeCurrent( m_device_context, m_render_context );
}

void PSRenderer::size_viewport( Vector2i new_size )
{
	glViewport(0,0,new_size.x,new_size.y);
}