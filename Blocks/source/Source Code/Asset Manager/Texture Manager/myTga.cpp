#include "myTga.h"

tImageTGA *LoadMyTGA(const char *filename)
{
	tImageTGA *pImageData = NULL;		// This stores our important image data
	WORD width = 0, height = 0;			// The dimensions of the image
	WORD xorigin = 0, yorigin = 0;
	BYTE length = 0;					// The length in bytes to the pixels
	BYTE imageType = 0;					// The image type (RLE, RGB, Alpha...)
	BYTE bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
	int i = 0;							// A counter

	// Open a file pointer to the targa file and check if it was found and opened 
	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		// Display an error message saying the file was not found, then return NULL
		MessageBox(g_hWnd, filename, "Error", MB_OK);
		return NULL;
	}
		
	// Allocate the structure that will hold our eventual image data (must free it!)
	pImageData = (tImageTGA*)malloc(sizeof(tImageTGA));

	// Read in the length in bytes from the header to the pixel data
	fread(&length, sizeof(BYTE), 1, pFile);
	
	// Jump over one byte
	fseek(pFile,1,SEEK_CUR); 

	// Read in the imageType (RLE, RGB, etc...)
	fread(&imageType, sizeof(BYTE), 1, pFile);
	
	// Skip past general information we don't care about
	fseek(pFile, 5, SEEK_CUR); 

	fread(&xorigin, sizeof(WORD), 1, pFile);
	fread(&yorigin, sizeof(WORD), 1, pFile);

	// Read the width, height and bits per pixel (16, 24 or 32)
	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(BYTE), 1, pFile);
	
	// Now we move the file pointer to the pixel data
	fseek(pFile, length + 1, SEEK_CUR); 

	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			//if its upside down read in backwards
			if(yorigin == 0) 
			{
				// Load in all the pixel data line by line
				for(int y = height - 1; y >= 0; y--)
				{
					// Store a pointer to the current line of pixels
					unsigned char *pLine = &(pImageData->data[stride * y]);

					// Read in the current line of pixels
					fread(pLine, stride, 1, pFile);
				
					// Go through all of the pixels and swap the B and R values since TGA
					// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
					for(i = 0; i < stride; i += channels)
					{
						int temp     = pLine[i];
						pLine[i]     = pLine[i + 2];
						pLine[i + 2] = temp;
					}
				}
			}
			else //read in normally
			{
				// Load in all the pixel data line by line
				for(int y = 0; y < height; y++)
				{
					// Store a pointer to the current line of pixels
					unsigned char *pLine = &(pImageData->data[stride * y]);

					// Read in the current line of pixels
					fread(pLine, stride, 1, pFile);
				
					// Go through all of the pixels and swap the B and R values since TGA
					// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
					for(i = 0; i < stride; i += channels)
					{
						int temp     = pLine[i];
						pLine[i]     = pLine[i + 2];
						pLine[i + 2] = temp;
					}
				}

			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for(int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}	
		// Else return a NULL for a bad or unsupported pixel format
		else
			return NULL;
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		// Create some variables to hold the rleID, current colors read, channels, & stride.
		BYTE rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->data = new unsigned char[stride * height];
		BYTE *pColors = new BYTE [channels];

		// Load in all the pixel data
		while(i < width*height)
		{
			// Read in the current color count + 1
			fread(&rleID, sizeof(BYTE), 1, pFile);
			
			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while(rleID)
				{
					// Read in the current color
					fread(pColors, sizeof(BYTE) * channels, 1, pFile);

					// Store the current pixel in our image array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

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
				fread(pColors, sizeof(BYTE) * channels, 1, pFile);

				// Go and read as many pixels as are the same
				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
				
			}	
		}

		// Free up pColors
		delete[] pColors;
	}

	// Close the file pointer that opened the file
	fclose(pFile);

	// Fill in our tImageTGA structure to pass back
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;
	return pImageData;
}
