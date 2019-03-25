/*	This is a header file containing Input Output Procedures used for 
	detecting edge based on estimating	the distance between an edge.

	Start date :		Wed, 27-10-1999
	Last Modified date:	Wed, 27-10-1999
	Filename:			PBMLibrary.cpp
	Copyright:			Sudirman 1999

*/

#include <stdio.h>
#include <string.h>

#include "PBMLibrary.h"

int getinteger(FILE *fd) {
  int c, i;
  char dummy[10000];

  c = getc(fd);
  while (1) 
  {
    if (c=='#')   
      fgets(dummy,9000,fd);
    if (c==EOF)
      return -1;
    if (c>='0' && c<='9')
      break;   
    c = getc(fd);
  }

  i = 0;
  while (1) {
    i = (i*10) + (c - '0');
    c = getc(fd);
    if (c==EOF) return (i);
    if (c<'0' || c>'9') break;
  }

  return (i);
}

bool read_pgma(const char *filename,unsigned char **in,int *x_size,int *y_size, char* msg) {
	FILE  *fd;
	char header;
	int  tmp;

	if ((fd=fopen(filename,"r")) == NULL) {
		strcpy(msg,"Unable to open file");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	header=fgetc(fd);
	header=fgetc(fd);

	*x_size = getinteger(fd);
	if (*x_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	*y_size = getinteger(fd);
	if (*y_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	tmp = getinteger(fd);

	*in = new unsigned char[*x_size * *y_size];

	for (int i=0;i<*x_size**y_size;i++) {
		in[0][i] = getinteger(fd);
		if (in[0][i]<0) {
			strcpy(msg,"Image has the wrong size");
			delete *in;
			*in = NULL;
			*x_size = 0;
			*y_size = 0;
			return false;
		}
	}

	fclose(fd);

	return true;
}

bool read_pgmb(const char *filename,unsigned char **in,int *x_size,int *y_size, char* msg) {
	FILE  *fd;
	char header;
	int  tmp;

	if ((fd=fopen(filename,"rb")) == NULL) {
		strcpy(msg,"Unable to open file");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	header=fgetc(fd);
	header=fgetc(fd);

	*x_size = getinteger(fd);
	if (*x_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	*y_size = getinteger(fd);
	if (*y_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	tmp = getinteger(fd);

	*in = new unsigned char[*x_size * *y_size];
	
	if (fread(*in,1,*x_size * *y_size,fd) == 0) {
		strcpy(msg,"Image has the wrong size");
		delete *in;
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}
    
	fclose(fd);

	return true;
}

bool read_ppma(const char *filename,unsigned char **in,int *x_size,int *y_size, char* msg) {
	FILE  *fd;
	char header;
	int  tmp;

	if ((fd=fopen(filename,"r")) == NULL) {
		strcpy(msg,"Unable to open file");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	header=fgetc(fd);
	header=fgetc(fd);

	*x_size = getinteger(fd);
	if (*x_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	*y_size = getinteger(fd);
	if (*y_size<=0) {
		strcpy(msg,"Image has the wrong size");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	tmp = getinteger(fd);

	*in = new unsigned char[*x_size * *y_size *3];

	for (int i=0;i<*x_size**y_size;i++) {
		in[0][3*i+0] = getinteger(fd);
		if (in[0][3*i+0]<0) {
			strcpy(msg,"Image has the wrong size");
			delete *in;
			*in = NULL;
			*x_size = 0;
			*y_size = 0;
			return false;
		}

		in[0][3*i+1] = getinteger(fd);
		if (in[0][3*i+1]<0) {
			strcpy(msg,"Image has the wrong size");
			delete *in;
			*in = NULL;
			*x_size = 0;
			*y_size = 0;
			return false;
		}

		in[0][3*i+2] = getinteger(fd);
		if (in[0][3*i+2]<0) {
			strcpy(msg,"Image has the wrong size");
			delete *in;
			*in = NULL;
			*x_size = 0;
			*y_size = 0;
			return false;
		}
	}

	fclose(fd);

	return true;
}

bool read_ppmb(const char *filename,unsigned char **in,int *x_size,int *y_size, char* msg) {

	FILE  *fd;
	char header;
	int  tmp;

	if ((fd=fopen(filename,"rb")) == NULL) {
		strcpy(msg,"Unable to open file");
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}

	header=fgetc(fd);
	header=fgetc(fd);

	*x_size = getinteger(fd);
	*y_size = getinteger(fd);
	tmp = getinteger(fd);

	*in = new unsigned char[*x_size * *y_size *3];

	if (fread(*in,1,*x_size * *y_size * 3,fd) == 0) {
		strcpy(msg,"Image has the wrong size");
		delete *in;
		*in = NULL;
		*x_size = 0;
		*y_size = 0;
		return false;
	}
    
	fclose(fd);

	return true;
}

unsigned char* ReadPBM(const char* path,int* width, int* height,bool* color, char* msg) {
	
	if (msg==NULL)
		return NULL;

	FILE* f;
	char header;
	
	if ((f=fopen(path,"r"))==NULL) {
		strcpy(msg,"Unable to open file");
		return NULL;
	}

	header=fgetc(f);
	if(!(header=='P')) {
		fclose(f);
		strcpy(msg,"Invalid PBM file header");
		return NULL;
	}

	header=fgetc(f);
	if((header=='2')||(header=='5'))
		*color = false;
	else if ((header=='3')||(header=='6'))
		*color = true;
	else {
		fclose(f);
		strcpy(msg,"Invalid PBM file header");
		return NULL;
	}
	
	fclose(f);

	int len = strlen(path);

	char ext[4];
	strncpy(ext,path+len-4,4);

	if (strnicmp(ext,".ppm",4) && strnicmp(ext,".pgm",4)) {
		strcpy(msg,"File does not have the correct extension");
		return NULL;
	}

	if (!strnicmp(ext,".ppm",4)) {
		if (!*color) {
			strcpy(msg,"Conflicting file extension and header");
			return NULL;
		}
	}

	if (!strnicmp(ext,".pgm",4)) {
		if (*color) {
			strcpy(msg,"Conflicting file extension and header");
			return NULL;
		}
	}

	unsigned char* image;
	*width = 0;
	*height = 0;

	if (!strnicmp(ext,".ppm",4)) {
		if (header=='3') {
			// Read ASCII-Encoded color PPM file
			if (read_ppma(path,&image,width,height,msg))
				return image;
			else {
				*width = 0;
				*height = 0;
				return NULL;
			}
		} else {
			// Read BINARY-Encoded color PPM file
			if (read_ppmb(path,&image,width,height,msg))
				return image;
			else {
				*width = 0;
				*height = 0;
				return NULL;
			}
		}
	} else {
		if (header=='2') {
			// Read ASCII-Encoded graylevel PGM file
			if (read_pgma(path,&image,width,height,msg))
				return image;
			else {
				*width = 0;
				*height = 0;
				return NULL;
			}
		} else {
			// Read BINARY-Encoded graylevel PGM file
			if (read_pgmb(path,&image,width,height,msg))
				return image;
			else {
				*width = 0;
				*height = 0;
				return NULL;
			}
		}
	}

	return NULL;
}

bool WritePPM(const char* path,unsigned char* image, int width, int height, char* msg) {

	if ((width<=0)||(height<=0)) {
		strcpy(msg,"Invalid image dimension");
		return false;
	}

	if (image==NULL) {
		strcpy(msg,"Empty image array");
		return false;
	}

	FILE *f;

	if ((f=fopen(path,"wb"))==NULL) {
		strcpy(msg,"Unable to create file");
		return false;
	}

	fprintf(f,"P6\n");
	fprintf(f,"%i\n%i\n255\n",width,height);
	if (fwrite(image,width*height*3,1,f) != 1) {
		fclose(f);
		strcpy(msg,"Unable to create file");
		return false;
	}

	fclose(f);
	return true;
		
}
		
bool WritePGM(const char* path,unsigned char* image, int width, int height, char* msg) {

	if ((width<=0)||(height<=0)) {
		strcpy(msg,"Invalid image dimension");
		return false;
	}

	if (image==NULL) {
		strcpy(msg,"Empty image array");
		return false;
	}

	FILE *f;

	if ((f=fopen(path,"wb"))==NULL) {
		strcpy(msg,"Unable to create file");
		return false;
	}

	fprintf(f,"P5\n");
	fprintf(f,"%i\n%i\n255\n",width,height);
	if (fwrite(image,width*height,1,f) != 1) {
		fclose(f);
		strcpy(msg,"Unable to create file");
		return false;
	}

	fclose(f);
	return true;

}

	


