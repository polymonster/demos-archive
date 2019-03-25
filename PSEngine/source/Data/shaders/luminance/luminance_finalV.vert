//interpolants
varying vec2 tex_coord;

void main(void)
{
	//set the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	tex_coord = gl_MultiTexCoord0.xy;
}
