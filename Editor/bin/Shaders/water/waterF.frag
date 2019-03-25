////Water Shader // Fragment

uniform samplerCube cubeMap;
uniform sampler2D normalMap;
uniform sampler2D normalMap2;
uniform sampler2D local;
uniform sampler2D underwater;
uniform int localReflections;
uniform float transparency;

uniform vec2 reflectionDistortionWeight;
uniform vec2 refractionDistortionWeight;

varying vec2 texCoord; 
varying vec2 texCoord2; 
varying vec2 texCoord3; 
varying vec3 vView;
varying vec3 worldNormal;

void main()
{
	//normal mapping
	vec3 normalTex = texture2D(normalMap, texCoord).rgb;
	
	vec3 dvduTex =  normalTex;
	dvduTex.x = 1.0 - normalTex.x;
	dvduTex.y = 1.0 - normalTex.y;
	dvduTex.z = 1.0 - normalTex.z;
	
   	normalTex = (normalTex - 0.5) * 2.0;
	dvduTex = (dvduTex - 0.5) * 2.0;

	vec3 normalTex2 = texture2D(normalMap2, texCoord2).rgb;
	
	vec3 dvduTex2 =  normalTex;
	dvduTex2.x = 1.0 - normalTex.x;
	dvduTex2.y = 1.0 - normalTex.y;
	dvduTex2.z = 1.0 - normalTex.z;
	
   	normalTex2 = (normalTex2 - 0.5) * 2.0;
	dvduTex2 = (dvduTex2 - 0.5) * 2.0;
	
	vec3 newNorm = vec3(0.0,1.0,0.0);

	vec3 averageNormal = newNorm + (0.5 * (normalTex + normalTex2));
	
	vec3 averageDvdu = (0.5 * (dvduTex + dvduTex2));

	vec3 base = vec3(0.5,0.5,0.5);
	vec3 beneath = vec3(0.5,0.5,0.5);
	
	float alpha = 1.0;
	
	//if local reflections
	if(localReflections == 1)
	{
		vec2 disturbedTextureCoords;

		vec4 texMatrix = gl_TexCoord[0];
		vec4 texMatrix2 = gl_TexCoord[0];
		
		texMatrix.x +=  averageNormal.x * reflectionDistortionWeight.x;
		texMatrix.y +=  averageNormal.y * reflectionDistortionWeight.y;
			
		texMatrix2.x +=  averageDvdu.x * refractionDistortionWeight.x;
		texMatrix2.y +=  averageDvdu.y * refractionDistortionWeight.y;

		texMatrix = texMatrix * mat4
		(
			0.5,0,0,0.5,
			0,0.5,0,0.5,
			0,0,0.5,0.5,
			0,0,0,1
		);

		texMatrix2 = texMatrix2 * mat4
		(
			0.5,0,0,0.5,
			0,0.5,0,0.5,
			0,0,0.5,0.5,
			0,0,0,1
		);
		
		beneath = texture2DProj(underwater, texMatrix2).rgb;
		alpha = texture2DProj(underwater, texMatrix2).a * 4.0;
		base = texture2DProj(local, texMatrix).rgb;
	}
		
	vec3 cubeReflect = normalize(reflect(vView, -averageNormal));
	vec3 col = textureCube(cubeMap, cubeReflect).rgb;
	
	vec4 final;
	
	if(localReflections == 0) 
	{
		final.rgb = col;
		final.a = transparency;
	}
	else 
	{
		col *= 0.15;
		base *= 0.35;
		beneath *= (0.5 * transparency);
		
		final.rgb = col + base + beneath;
		final.a = 1.0;
	}
	
	
	gl_FragColor = vec4(final);

}
