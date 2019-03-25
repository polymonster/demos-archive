/*=========================================================*\
|	texture_manager.cpp - texture resources
|-----------------------------------------------------------|
|				Project :	PolySpoon Texture Manager
|				Coder	:	ABleas
|				Date	:	24/05/09
|-----------------------------------------------------------|
|	Copyright (c) PolySpoon 2009. All rights reserved.		|
\*=========================================================*/


/*======================== INCLUDES =======================*/

#include "texture_manager.h"
#include <string>
#include <fstream>

/*======================== CONSTANTS ======================*/


/*======================== VARIABLES ======================*/

TextureManager g_texture_mgr;

int TextureDefaults::normal_map;
int TextureDefaults::specular_map;
int TextureDefaults::parallax_map;
int TextureDefaults::no_texture;
int TextureDefaults::test_cube;
int TextureDefaults::materials_lookup;


/*================= PROTOTYPE DEFINITIONS =================*/

/*=========================================================*\
|	constructor/destructor
\*=========================================================*/
TextureManager::TextureManager()
{
	m_num_loaded_textures = 0;

	for( int i = 0; i < MAX_LOADED_TEXTURES; i++ )
	{
		m_texture_slot[i].gl_handle = -1;
		m_texture_slot[i].filename[0] = NULL;
	}

}

TextureManager::~TextureManager()
{
	// unload resources
	for( int i = 0; i < MAX_LOADED_TEXTURES; i++ )
	{
		if( m_texture_slot[i].gl_handle != -1 )
		{
			glDeleteTextures( 1, &m_texture_slot[i].gl_handle );
			m_texture_slot[i].gl_handle = -1;
			m_texture_slot[i].filename[0] = 0;
		}
	}
}

/*=========================================================*\
|	init - set initial variables and preloaded textures
\*=========================================================*/
void TextureManager::init( void )
{
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_max_texture_units );

	for(int i = 0; i < 8; i++)
	{
		m_texture_unit_cache[i] = -1;
		m_texture_unit_enabled[i] = false;
	}
	
	preload_resources();
}


void TextureManager::preload_resources( void )
{

}

/*=========================================================*\
|	load_texture - load a tga texture from filename
\*=========================================================*/
int TextureManager::load_texture( char *filename_p )
{
	std::string filename_str(&filename_p[0]);
	filename_str = TEXTURE_PATH + filename_str;

	// TODO: correct filename with paths..
	TGAHeader *tex_p = load_texture_data( const_cast<char *>(filename_str.c_str()) );

	if( tex_p )
	{
		// number of channels
		int channels = tex_p->bpp / 8;

		int id = 0;
		while( m_texture_slot[id].gl_handle != -1 && id < MAX_LOADED_TEXTURES ) id++;

		if( id == MAX_LOADED_TEXTURES )
			return -1;

		strcpy_s( m_texture_slot[id].filename, filename_str.c_str() );

		// generate the texture
		glGenTextures( 1, &m_texture_slot[id].gl_handle );
		glBindTexture( GL_TEXTURE_2D, m_texture_slot[id].gl_handle );

		switch( channels )
		{
			case 3:
				glTexImage2D( GL_TEXTURE_2D, 0, channels, tex_p->width, tex_p->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_p->data_p );
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
			case 4:
				glTexImage2D( GL_TEXTURE_2D, 0, channels, tex_p->width, tex_p->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_p->data_p );
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
		}

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// clean up texture data and container
		delete [] tex_p->data_p;
		delete tex_p;

		g_console_p->dbgmsg( "texture %s into slot %d\n", filename_str.c_str(), id );
		return id;

	}

	return -1;
}



/*=========================================================*\
|	load_texture_data - load data from the file
\*=========================================================*/
TGAHeader *TextureManager::load_texture_data( char *filename_p )
{
	// header buffer
	unsigned char header[32];
	memset( header, 0, sizeof( header ) );

	FILE *file_p = fopen( filename_p, "rb" );

	if( file_p )
	{
		// create a tga header container
		TGAHeader *new_tex_p = new TGAHeader();

		// read header to temp storage
		fread( header, 18, 1, file_p );

		// get tga header data
		unsigned short *w_p = (unsigned short *)&header[12];
		unsigned short *h_p = (unsigned short *)&header[14];
		new_tex_p->width = *w_p;
		new_tex_p->height = *h_p;

		new_tex_p->bpp = header[16];
		new_tex_p->attributes.attribute = header[17] & 0x000f;
		new_tex_p->attributes.horizontal = ( header[17] >> 4 ) & 0x0001;
		new_tex_p->attributes.vertical = ( header[17] >> 5 ) & 0x0001;

		new_tex_p->image_start = header[0];
		new_tex_p->image_type = header[2];

		// read texture data
		int channels = new_tex_p->bpp / 8;
		int indent = header[0];

		fseek( file_p, indent+18, 0 );

		if( new_tex_p->image_type == 10 )
		{
			// RLE Compression
			// Create some variables to hold the rleID, current colors read, & stride.
			BYTE rleID = 0;
			int colorsRead = 0;
			int stride = channels * new_tex_p->width;

			// Next we want to allocate the memory for the pixels and create an array,
			// depending on the channel count, to read in for each pixel.
			new_tex_p->data_p = new unsigned char[stride * new_tex_p->height];
			BYTE *pColors = new BYTE [channels];

			int i = 0;
			int size = new_tex_p->width*new_tex_p->height;
			// Load in all the pixel data
			while( i < size )
			{
				// Read in the current color count + 1
				fread( &rleID, sizeof(BYTE), 1, file_p );
				
				// Check if we don't have an encoded string of colors
				if( rleID < 128 )
				{
					// Increase the count by 1
					rleID++;

					// Go through and read all the unique colors found
					while( rleID )
					{
						// Read in the current color
						fread( pColors, sizeof(BYTE) * channels, 1, file_p );

						// Store the current pixel in our image array
						new_tex_p->data_p[colorsRead + 0] = pColors[2];
						new_tex_p->data_p[colorsRead + 1] = pColors[1];
						new_tex_p->data_p[colorsRead + 2] = pColors[0];

						// If we have a 4 channel 32-bit image, assign one more for the alpha
						if( new_tex_p->bpp == 32)
							new_tex_p->data_p[colorsRead + 3] = pColors[3];

						// Increase the current pixels read, decrease the amount
						// of pixels left, and increase the starting index for the next pixel.
						i++;
						rleID--;
						colorsRead += channels;
					}
				}
				// Else, let's read in a string of the same character
				else
				{
					// Minus the 128 ID + 1 (127) to get the color count that needs to be read
					rleID -= 127;

					// Read in the current color, which is the same for a while
					fread( pColors, sizeof(BYTE) * channels, 1, file_p );

					// Go and read as many pixels as are the same
					while(rleID)
					{
						// Assign the current pixel to the current index in our pixel array
						new_tex_p->data_p[colorsRead + 0] = pColors[2];
						new_tex_p->data_p[colorsRead + 1] = pColors[1];
						new_tex_p->data_p[colorsRead + 2] = pColors[0];

						// If we have a 4 channel 32-bit image, assign one more for the alpha
						if(new_tex_p->bpp == 32)
							new_tex_p->data_p[colorsRead + 3] = pColors[3];

						// Increase the current pixels read, decrease the amount
						// of pixels left, and increase the starting index for the next pixel.
						i++;
						rleID--;
						colorsRead += channels;
					}
					
				}	
			}
		}
		else if( header[1] && new_tex_p->bpp == 8 )
		{
			// 8 bit palette based
			unsigned short *palette_start_p = (unsigned short*)&header[3];
			unsigned short *number_of_cols_p = (unsigned short*)&header[5];
			unsigned char palette_channels = header[7] / 8;
			int palette_size = number_of_cols_p[0] * palette_channels;

			new_tex_p->palette_p = new unsigned char [palette_size];

			fseek( file_p, 18, 0 );
			fread( new_tex_p->palette_p, palette_size, 1, file_p );

			// convert our data into 24 bit
			int index_size = ( ( new_tex_p->bpp / 8 ) * new_tex_p->width ) * new_tex_p->height;
			new_tex_p->indices_p = new unsigned char[index_size];

			fread( new_tex_p->indices_p, index_size, 1, file_p );

			int image_size = ( palette_channels * new_tex_p->width ) * new_tex_p->height;
			new_tex_p->data_p = new unsigned char[image_size];

			unsigned char *data_p = new_tex_p->data_p;
			for( int i = 0; i < index_size; i++ )
			{
				*(data_p)++ = new_tex_p->palette_p[ new_tex_p->indices_p[ i ]*3 ];
				*(data_p)++ = new_tex_p->palette_p[ (new_tex_p->indices_p[ i ]*3)+1 ];
				*(data_p)++ = new_tex_p->palette_p[ (new_tex_p->indices_p[ i ]*3)+2 ];
			}

			// discard palette and index texture data
			delete [] new_tex_p->palette_p;
			delete [] new_tex_p->indices_p;

			// make sure the texture is processed as 24 bit
			new_tex_p->bpp = 24;

			// correct channel order
			bgr_to_rgb( new_tex_p->data_p, new_tex_p->width, new_tex_p->height, 3 );

		}
		else
		{
			// 24 or 32 bit BGR image
			int image_size = ( channels * new_tex_p->width ) * new_tex_p->height;
			new_tex_p->data_p = new unsigned char[image_size];

			fread( new_tex_p->data_p, image_size, 1, file_p );

			// correct channel order
			bgr_to_rgb( new_tex_p->data_p, new_tex_p->width, new_tex_p->height, channels );

		}

		// correct orientation
		if( new_tex_p->attributes.vertical )
				flip_vertical( new_tex_p->data_p, new_tex_p->width, new_tex_p->height, channels );
		if( new_tex_p->attributes.horizontal )
			    flip_horizontal( new_tex_p->data_p, new_tex_p->width, new_tex_p->height, channels );

		fclose( file_p );

		return new_tex_p;
	}

	g_console_p->dbgmsg( "Could not load texture file from filename '%s'!\n", filename_p );
	return NULL;
}


void TextureManager::bgr_to_rgb( unsigned char *texture_data_p, int width, int height, int bytes_per_texel )
{
	// unsupported texture format
	if( bytes_per_texel < 3 || bytes_per_texel > 4 )
		return;

	unsigned char *index_p = texture_data_p;
	int texture_size = bytes_per_texel * ( width * height );
	for( int i = 0; i < texture_size; i+=bytes_per_texel )
	{
		unsigned char blue = index_p[i];
		*(index_p+i) = *(index_p+i+2);
		*(index_p+i+2) = blue;
	}
}

void TextureManager::flip_vertical( unsigned char *texture_data_p, int width, int height, int bytes_per_texel )
{
	// flip texture vertically
	int rowlen = (bytes_per_texel*width);
	unsigned char *data_0_p = texture_data_p;
	unsigned char *data_1_p = texture_data_p+(rowlen*height) - rowlen;

	unsigned char *row_p = new unsigned char[rowlen];

	int halfh = height/2;
	for( int i = 0; i < halfh; i++ )
	{
		memcpy( row_p, data_1_p-(rowlen*i), rowlen );
		memcpy( data_1_p-(rowlen*i), data_0_p+(rowlen*i), rowlen );
		memcpy( data_0_p+(rowlen*i), row_p, rowlen );
	}

	delete [] row_p;
}

void TextureManager::flip_horizontal( unsigned char *texture_data_p, int width, int height, int bytes_per_texel )
{
	// flip texture horizontally
	int rowlen = (bytes_per_texel*width);
	unsigned char *data_0_p = texture_data_p;
	unsigned char *data_1_p = texture_data_p+rowlen-1;

	unsigned char *row_p = new unsigned char[rowlen];

	int halfw = width/2;
	for( int i = 0; i < height; i++ )
	{
		for( int i = 0; i < halfw; i++ )
		{
			memcpy( row_p, data_1_p-(bytes_per_texel*i), bytes_per_texel );
			memcpy( data_1_p-(bytes_per_texel*i), data_0_p+(bytes_per_texel*i), bytes_per_texel );
			memcpy( data_0_p+(bytes_per_texel*i), row_p, bytes_per_texel );
		}
	}

	delete [] row_p;
}


int TextureManager::get_texture_id( char *filename_p )
{
	for( int i = 0; i < MAX_LOADED_TEXTURES; i++ )
	{
		if( m_texture_slot[i].gl_handle != -1 )
		{
			if( !strcmp( filename_p, m_texture_slot[i].filename ) )
				return i;
		}
	}

	g_console_p->dbgmsg( "Could not find texture from filename '%s'!\n", filename_p );
	return -1;
}

void TextureManager::invalidate_unit( int unit, bool inavlidate_enabled /*= false */ )
{
	m_texture_unit_cache[unit] = -1;

	if(inavlidate_enabled)
		m_texture_unit_enabled[unit] = false;
}

void TextureManager::bind_texture( int id, int texture_unit )
{
	if(id < 0 || id > MAX_LOADED_TEXTURES) return;
	if(m_texture_slot[id].gl_handle == -1) return;

	glActiveTexture( GL_TEXTURE0 + texture_unit );
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, m_texture_slot[id].gl_handle );
}


void TextureManager::bind_texture( char *filename_p, int texture_unit )
{
	int id = get_texture_id( filename_p );

	bind_texture(id, texture_unit);
}

void TextureManager::disable_all_units()
{
	for(int i = 0; i < 8; i++)
	{
		m_texture_unit_enabled[i] = false;

		glActiveTexture( GL_TEXTURE0 + i );
		glDisable(GL_TEXTURE_2D);
	}
}


void TextureManager::bind_cubemap( int id , int texture_unit )
{
	if(id < 0 || id > MAX_LOADED_TEXTURES) return;
	if(m_texture_slot[id].gl_handle == -1) return;

	glActiveTextureARB(GL_TEXTURE0 + texture_unit);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_texture_slot[id].gl_handle);
}

void TextureManager::unbind_cubemap(int unit)
{
	//glActiveTextureARB(GL_TEXTURE0_ARB + unit);
	//glDisable(GL_TEXTURE_CUBE_MAP_ARB);
}

int TextureManager::load_cubemap( std::string filename )
{
	filename = TEXTURE_PATH + filename;

	std::ifstream fin;

	fin.open(filename.c_str());

	std::string name;
	std::string front;
	std::string back;
	std::string left;
	std::string right;
	std::string top;
	std::string bottom;

	std::string word;

	while(!fin.eof())
	{
		fin >> word;

		if(word == "name")
		{
			fin >> name;
			name = TEXTURE_PATH + name;
		}

		if(word == "front")
		{
			fin >> front;
			front = TEXTURE_PATH + front;
		}

		if(word == "back")
		{
			fin >> back;
			back = TEXTURE_PATH + back;
		}

		if(word == "left")
		{
			fin >> left;
			left = TEXTURE_PATH + left;
		}

		if(word == "right")
		{
			fin >> right;
			right = TEXTURE_PATH + right;
		}

		if(word == "top")
		{
			fin >> top;
			top = TEXTURE_PATH + top;
		}

		if(word == "bottom")
		{
			fin >> bottom;
			bottom = TEXTURE_PATH + bottom;
		}
	}

	return load_cubemap
	(
	const_cast<char *>(front.c_str()),
	const_cast<char *>(back.c_str()),
	const_cast<char *>(left.c_str()),
	const_cast<char *>(right.c_str()),
	const_cast<char *>(top.c_str()),
	const_cast<char *>(bottom.c_str()),
	const_cast<char *>(name.c_str())
	);
}

int TextureManager::load_cubemap(char *front,char *back,char *left,char *right,char *top,char *bottom,char *name)
{
	int id = 30;

	glGenTextures( 1, &m_texture_slot[id].gl_handle );

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,m_texture_slot[id].gl_handle);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	int type = GL_RGB;
	int channels;

	//BACK
	TGAHeader *b = load_texture_data(back);

	type = GL_RGB;
	channels = b->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, type, b->width, b->height, 0, type, GL_UNSIGNED_BYTE, b->data_p);

	//FRONT
	TGAHeader *f = load_texture_data(front);

	type = GL_RGB;
	channels = f->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, type, f->width, f->height, 0, type, GL_UNSIGNED_BYTE, f->data_p); 

	//LEFT
	TGAHeader *l = load_texture_data(left);

	type = GL_RGB;
	channels = l->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, type, l->width, l->height, 0, type, GL_UNSIGNED_BYTE, l->data_p); 

	//RIGHT
	TGAHeader *r = load_texture_data(right);

	type = GL_RGB;
	channels = r->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, type, r->width, r->height, 0, type, GL_UNSIGNED_BYTE, r->data_p); 

	//TOP
	TGAHeader *t = load_texture_data(top);

	type = GL_RGB;
	channels = t->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, type, t->width, t->height, 0, type, GL_UNSIGNED_BYTE, t->data_p); 

	//BOTTOM
	TGAHeader *d = load_texture_data(bottom);

	type = GL_RGB;
	channels = d->bpp / 8;
	if(channels == 4) type = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, type, d->width, d->height, 0, type, GL_UNSIGNED_BYTE, d->data_p); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete d, t, f, b, l, r;

	return id;
}