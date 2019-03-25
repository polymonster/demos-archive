//Water Shader // Vertex 

uniform vec3 cameraPos;
uniform vec4 lightPos;

varying vec2 texCoord;
varying vec2 texCoord2;
varying vec2 texCoord3;

varying vec3 vView;
varying vec3 worldNormal;

void main()
{
	gl_Position = ftransform();
	
	//openGL states
	vec3 vert = vec3(gl_ModelViewMatrix * gl_Vertex);

	vec3 tbnNormal = gl_Normal;

	//TBN Matrix calculations
	vec3 tangent = vec3(-1.0,0.0,0.0);
	vec3 binormal = vec3(0.0,0.0,1.0);
	mat3 TBN = mat3(tangent, binormal, gl_Normal);

	//eye vector based on tbn
	vView = normalize(cameraPos - gl_Position.xyz);
	vView *= TBN;

	texCoord = gl_MultiTexCoord1.xy;
	texCoord2 = gl_MultiTexCoord2.xy;
	texCoord3 = gl_MultiTexCoord0.xy;
	
	gl_TexCoord[0] = gl_Position;
	
}

