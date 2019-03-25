//Parallax Mapping // Vertex

varying vec3 color;
varying vec2 texCoord;
varying vec2 normalTexCoord;

varying vec3 normal;
varying vec3 position;

varying vec3 camera;
varying vec3 lightPosition;

uniform vec4 lightPos;
uniform vec4 cameraPos;

attribute vec4 uTextureWeights1;
attribute vec4 uTextureWeights2;

varying vec4 vTextureWeights1;
varying vec4 vTextureWeights2;

varying vec3 eyeVec;
varying vec3 lightVec;

//varying float depthColor;

void main()
{
	//openGL states
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
    texCoord = gl_MultiTexCoord1.xy;
	//texCoord.y *= 0.5;
	normalTexCoord = texCoord + 0.5;
	
	normal = gl_Normal.xyz;
	position = gl_Vertex.xyz;
	camera = cameraPos.xyz;
	lightPosition = lightPos.xyz;
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_Vertex;
	
	vTextureWeights1 = gl_Color;
	vTextureWeights2 = uTextureWeights2;
	
	//depthColor = -gl_Vertex.y * 0.008;
}
