//SkyDOME Shader // Vertex 

varying vec2 texCoord1;
varying vec2 texCoord2;

void main()
{
	//openGL states
	gl_Position = ftransform();
	
	texCoord1 = gl_MultiTexCoord1.st;	
	texCoord2 = gl_MultiTexCoord2.st;	
}

