//Parallax Mapping // Vertex

uniform vec3 cameraPos;
uniform vec4 lightPos;

varying vec3 direction;
varying vec3 eyeVec;
varying vec2 texCoord;
varying vec3 myCam;

void main()
{
	//openGL states
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	texCoord = gl_MultiTexCoord0.xy;

	//values we need 
	vec3 normal = gl_Normal;
	vec4 lighta = lightPos;
	vec3 tangent = gl_Color.xyz;

	//TBN calculation
	tangent *= gl_NormalMatrix;
	normal *= gl_NormalMatrix;

	normalize(normal);
	normalize(tangent );
	
	vec3 binormal = cross(tangent, normal);
      
	mat3 TBN = mat3(tangent, binormal, normal);

	//eye vector based on tbn
	eyeVec = cameraPos - gl_Vertex.xyz;
	eyeVec *= TBN;

	//direction based on TBN
	direction = lighta.xyz - gl_Vertex.xyz;
	direction *= TBN;

	//send the cameraPos through
	myCam.xyz = eyeVec.xyz;

}
