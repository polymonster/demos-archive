//Parallax Mapping // Vertex

uniform vec3 cameraPos;
uniform vec4 lightPos;

varying vec3 eyeVec;
varying vec2 texCoord;
varying vec3 lightVec;

uniform mat4 orientationMatrix;


void main()
{
	texCoord = gl_MultiTexCoord1.xy;
	
	vec4 great = gl_Vertex * orientationMatrix;
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * great;
	
	vec3 n = normalize(gl_NormalMatrix * gl_Normal.xyz);
	vec3 t = normalize(gl_NormalMatrix * gl_Color.xyz);
	vec3 b = cross(n, t);
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 tmpVec = lightPos.xyz - vVertex;
	lightVec = tmpVec;

	tmpVec = -vVertex;
	eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);
	
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	
	

}
