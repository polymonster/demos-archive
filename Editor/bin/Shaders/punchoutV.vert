//grass shader // Vertex

varying vec2 texCoord;

uniform float vertexOffset;

void main()
{
	vec4 newVertexPos = gl_Vertex;

	texCoord = gl_MultiTexCoord0.xy;

	if(texCoord.y == 1.0) newVertexPos.x += vertexOffset;

	//openGL states
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	if(texCoord.y == 1.0) gl_Position.x += vertexOffset;

	
}
