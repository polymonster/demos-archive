//interpolants
varying vec4 shadow_coord;

void main(void)
{
	//set the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	shadow_coord = gl_TextureMatrix[7] * gl_Vertex;
}
