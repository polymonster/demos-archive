//interpolants
varying vec4 vertex_position;

void main(void)
{
	//set the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vertex_position = gl_Position;
}
