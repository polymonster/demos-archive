/************************************************************
*	APRON TUTORIAL PRESENTED BY MORROWLAND					*
*************************************************************
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
************************************************************/

#include "CJPEGFile.h"
#include "data/include/define.h"
#include <stdio.h>
#include <setjmp.h>
#include "data/include/jpeglib.h"


//----------------------------------------------------------------------------
struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}
//----------------------------------------------------------------------------


void CJPEGFile::Load(char *filename, int bitsperpixel, void *where)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;    // source file
	JSAMPARRAY buffer;    // Output row buffer
	int row_stride;   // physical row width in output buffer
	
	if ((infile = fopen(filename, "rb")) == NULL)
	{
		return;
	}
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return;
	}
	
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	
	(void) jpeg_read_header(&cinfo, TRUE);
	
	(void) jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	int y = cinfo.output_height-1;	
	
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		
		unsigned char *super = *buffer;
		
		for(WORD x=0;x<cinfo.output_width;x++)
		{
			unsigned char r = *super;
			super++;
			unsigned char g = *super;
			super++;
			unsigned char b = *super;
			super++;
			
			((BYTE *)where)[(x*3)+((y*cinfo.output_width)*3)+0] = r;
			((BYTE *)where)[(x*3)+((y*cinfo.output_width)*3)+1] = g;
			((BYTE *)where)[(x*3)+((y*cinfo.output_width)*3)+2] = b;
		}
		
		y--;
	}
	
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
}
 


void CJPEGFile::GetSize(char *filename, unsigned long &width, unsigned long &height)
{
	
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;
	
	if ((infile = fopen(filename, "rb")) == NULL)
	{
		
		return;
	}
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return;
	}
	
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	
	(void) jpeg_read_header(&cinfo, TRUE);
	
	(void) jpeg_start_decompress(&cinfo);
	
	width  = cinfo.output_width;
	height = cinfo.output_height;
	
	jpeg_destroy_decompress(&cinfo);	
	fclose(infile);
}