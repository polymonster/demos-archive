//SkyBox Shader // Vertex 

//interpolate the normal to the fragment shader
varying vec3 normal;

void main()
{
	//openGL states
	gl_Position = ftransform();
	
	//set the face normal
	normal = gl_Normal;	
}

