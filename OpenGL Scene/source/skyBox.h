#include "globals.h"

class CSkyBox{

private:
	
//Sky Box Textures
unsigned int skyLeftTexture;
unsigned int skyRightTexture;
unsigned int skyFrontTexture;
unsigned int skyBackTexture;
unsigned int skyUpTexture;
unsigned int skyDownTexture;

public:

CSkyBox();
~CSkyBox();

void drawSkyBox();
void loadTextures();


};

