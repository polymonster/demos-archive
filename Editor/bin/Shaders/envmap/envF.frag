//Water Shader // Fragment

uniform samplerCube cubeMap;

varying vec3 vView;
varying vec3 worldNormal;

void main()
{
	vec3 col = textureCube(cubeMap, worldNormal).rgb;
	col *= 0.5;

	vec3 grey = vec3(0.5,0.5,0.5);
	
	vec3 mixed = col * grey;

	gl_FragColor = vec4(mixed,1.0);
}
