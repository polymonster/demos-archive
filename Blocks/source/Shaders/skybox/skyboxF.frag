////Water Shader // Fragment

uniform samplerCube cubeMap;
varying vec3 normal;

void main()
{

	vec3 cubeNormal = normalize(normal);

	vec3 col = textureCube(cubeMap, cubeNormal).rgb;

	gl_FragColor = vec4(col, 1);

}
