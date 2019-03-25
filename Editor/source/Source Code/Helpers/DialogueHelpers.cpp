#include "Helpers.h"
#include "Input.h"
#include "modelManager.h"
#include "toolbars.h"

CHOOSECOLOR cc;                 // common dialog box structure 
static COLORREF acrCustClr[16]; // array of custom colors 
HBRUSH hbrush;                  // brush handle
static DWORD rgbCurrent;        // initial color selection
char buffer[1024];

SVector3 initColorChooser()
{
	extern HWND g_hWnd;

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
				cc.lStructSize = sizeof(cc);
				cc.hwndOwner = g_hWnd;
				cc.lpCustColors = (LPDWORD) acrCustClr;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
				 
	if (ChooseColor(&cc)==TRUE) 
	{
		hbrush = CreateSolidBrush(cc.rgbResult);
		rgbCurrent = cc.rgbResult;
	}

	int r = GetRValue(rgbCurrent);
	int g = GetGValue(rgbCurrent);
	int b = GetBValue(rgbCurrent);

	SVector3 floatColour(r/255.0f,g/255.0f,b/255.0f);

	return floatColour;
}

void FillOpenParams(OPENFILENAME &open_params, HWND hwnd, char *filter, char *file_name)
{
	/** Initialize the "open_params" fields we need to for opening a .bmp file **/

	open_params.lStructSize = sizeof(OPENFILENAME); // The size of the struct
	open_params.hwndOwner = hwnd; // The "window owner" of this dialog box
	open_params.lpstrFilter = filter;
	
	open_params.lpstrFile = file_name; // This is a pointer to a filename used
								      // to initialize the dialog box -- Since our
								     // "file_name" equals NULL, No "default filename"
							        // will be used
				
	open_params.nMaxFile = BUFF_MAX; // The size of the "file_name" buffer
				
	open_params.lpstrInitialDir = NULL; // Now this already equals NULL, but I'm setting
									   // it again just so I can comment it -- This means
									  // (for Windows NT 5.0/Windows 98 and later)
									 // that if the "current directory" contains any files of 
									// the specified filter type(s) this directory will be the 
								   // initial directory.  Otherwise it will be the "personal
								  // files directory" for whoever's running the program 

	open_params.lpstrFileTitle = NULL; // Again it's already NULL but what this is, is the 
									  // title of the dialog box -- Since we're setting it
									 // to NULL it will use the default title ("Open" in 
									// our case)

	// As you can imagine, there is a plethora of flags that can be set for the dialog box
	// I'll break down what these four mean
	// OFN_FILEMUSTEXIST -- Only allows the typing of existing files in the File Name 
	//						entry field
	// OFN_PATHMUSTEXIST -- Allows the user to ONLY type valid paths and filenames
	// OFN_NOCHANGEDIR -- Changes the current directory back to it's original value
	//					  if the user changes the directory while searching for files
	// OFN_HIDEREADONLY -- This hides the "read only" check box (the ability to open files
	//					   as "read only" is possible)					
	open_params.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
						| OFN_HIDEREADONLY;

	// Okay we've filled what we wanted to
}

int openFile()
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box
	
	strcat(filter,"3ds Mesh"); 

		
	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';
	filter[index++] = '3';
	filter[index++] = 'd';
	filter[index++] = 's';
				
	FillOpenParams(open_params,g_hWnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetOpenFileName(&open_params))
	{
		return -1;//CModelManager::getInstance()->open(open_params.lpstrFile);
	}

	return -1;
}

char* openFile(const char *desc, const char *ext)
{
	OPENFILENAME open_params = {0}; 

	char filter[BUFF_MAX] = {0};
	char file_name[BUFF_MAX] = {0};
	
	strcat(filter,desc); 

	int index = strlen(filter) + 1;

	int extLen = strlen(ext);

	filter[index++] = '*';
	filter[index++] = '.';

	for(int i = 0; i < extLen; i++)
	{
		filter[index++] = ext[i];
	}
				
	FillOpenParams(open_params,g_hWnd,filter,file_name);

	if(GetOpenFileName(&open_params))
	{
		strcpy(buffer,open_params.lpstrFile);
		return buffer;
	}

	return NULL;
}

char* saveFile(const char *desc, const char *ext)
{
	OPENFILENAME open_params = {0}; // This structure is used by the either the
								   // GetOpenFileName() function (for opening files)
								  // or the GetSaveFileName() function (for saving files)
								 // We're going to use it for opening a file -- Info about
								// the file we try to open WILL be stored in this struct

	char filter[BUFF_MAX] = {0}; // This will be filled in with a "filter", explained later :)
	char file_name[BUFF_MAX] = {0}; // This will be used as the "default filename" for the
							        // dialog box
	
	strcat(filter,desc); 

	int index = strlen(filter) + 1; // index now equals the first open
												// char one past the first NULL in filter

	int extLen = strlen(ext);

	// The "second half" of filter is filled with "*.bmp"
	filter[index++] = '*';
	filter[index++] = '.';

	for(int i = 0; i < extLen; i++)
	{
		filter[index++] = ext[i];
	}
				
	FillOpenParams(open_params,g_hWnd,filter,file_name);

	// If GetOpenFileName DOES NOT FAIL (it could be returning an error code
	// so it would pass the "if test")
	if(GetSaveFileName(&open_params))
	{
		strcpy(buffer,open_params.lpstrFile);
		strcat(buffer,".");
		strcat(buffer,ext);
		return buffer;
	}

	return NULL;
}

void polyCounter()
{

}