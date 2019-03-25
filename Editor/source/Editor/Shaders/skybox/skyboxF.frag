////Skybox Shader // Fragment

//the texture unit the cube map is bound to
uniform samplerCube cubeMap;

//interpolated normal
varying vec3 normal;

void main()
{
	//normalise the normal vector
	vec3 cubeNormal = normalize(normal);

	//use the texture cube function
	vec3 col = textureCube(cubeMap, cubeNormal).rgb;

	//out put the cube map texture pixel colour
	gl_FragColor = vec4(col, 1.0);

}
