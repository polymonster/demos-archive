//gaussain shader // Vertex

varying vec2 texCoord;

void main()
{
	texCoord = gl_MultiTexCoord0.xy;
	//openGL states
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}
