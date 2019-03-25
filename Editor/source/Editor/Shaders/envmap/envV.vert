//Water Shader // Vertex 

uniform vec3 cameraPos;
uniform vec4 lightPos;

varying vec3 vView;
varying vec3 worldNormal;

void main()
{
	//openGL states
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vView = normalize(cameraPos - gl_Position.xyz);
	
	vec3 transformedNormal = (gl_NormalMatrix * gl_Normal.xyz) * -1.0;

	//eye vector based on tbn
	worldNormal = normalize(reflect(vView, transformedNormal));
}

