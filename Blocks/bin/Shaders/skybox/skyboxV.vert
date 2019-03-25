//SkyBox Shader // Vertex 

varying vec3 normal;

void main()
{
	//openGL states
	gl_Position = ftransform();
	normal = gl_Normal;	
}

