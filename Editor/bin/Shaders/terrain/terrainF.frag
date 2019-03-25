//Parallax Mapping // Fragment

//texture splat weights
varying vec3 color;
varying vec4 vTextureWeights1;
varying vec4 vTextureWeights2;

//standard varying
varying vec3 normal;
varying vec3 position;
varying vec2 texCoord;
varying vec3 camera;
varying vec3 lightPosition;

//texture samplers
uniform sampler2DShadow texture_pallet1;

uniform sampler2D texture_pallet2;
uniform sampler2D texture_pallet3;
uniform sampler2D texture_pallet4;
uniform sampler2D texture_pallet5;
uniform sampler2D texture_pallet6;
uniform sampler2D texture_pallet7;
uniform sampler2D texture_pallet8;

varying float depthColor;

void main()
{
	//vec4 specular = vec4(0.0);
	
	vec4 diffuse;
	vec3 norm = normalize(normal);
	vec3 lightVector = lightPosition - position;
	float dist = length(lightVector);
	
	float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation + 
						gl_LightSource[0].linearAttenuation * dist +
						gl_LightSource[0].quadraticAttenuation * dist * dist);

	lightVector = normalize(lightVector);
	float nxDir = max(0.0,dot(norm,lightVector)); 
	diffuse = gl_LightSource[0].diffuse * nxDir * attenuation;

	/*if(nxDir != 0.0);
	{
		vec3 cameraVector = normalize(camera - position.xyz);
		vec3 halfVector = normalize(lightVector + cameraVector);
		float nxHalf = max(0.0,dot(norm,halfVector));
		float specPow = pow(nxHalf,gl_FrontMaterial.shininess);
		specular = gl_LightSource[0].specular * specPow * attenuation;
	}*/
	
	//get our terrains texture pallet
	vec3 tex[8];
	//tex[0] = texture2D(texture_pallet1, texCoord.st).rgb;
	tex[1] = texture2D(texture_pallet2, texCoord.st).rgb;
	tex[2] = texture2D(texture_pallet3, texCoord.st).rgb;
	tex[3] = texture2D(texture_pallet4, texCoord.st).rgb;
	tex[4] = texture2D(texture_pallet5, texCoord.st).rgb;
	tex[5] = texture2D(texture_pallet6, texCoord.st).rgb;
	tex[6] = texture2D(texture_pallet7, texCoord.st).rgb;
	tex[7] = texture2D(texture_pallet8, texCoord.st).rgb;

 
	//blend together using weights (splats)
	//splat weights 1 = textures (1 - 4)
	//vec3 texCol1 = tex[0] * vTextureWeights1.r;
	//mix(texCol1,tex[1],vTextureWeights1.g);
	vec3 texCol2 = tex[1] * vTextureWeights1.r;
	vec3 texCol3 = mix(texCol2,tex[2],vTextureWeights1.g);
	vec3 texCol4 = mix(texCol3,tex[3],vTextureWeights1.b);
	vec3 texCol5 = mix(texCol4,tex[4],vTextureWeights1.a);
	vec3 texCol6 = mix(texCol5,tex[5],vTextureWeights2.r);
	vec3 texCol7 = mix(texCol6,tex[6],vTextureWeights2.g);
	vec3 overall  = mix(texCol7,tex[7],vTextureWeights2.b);
	
	vec3 shadowUV = gl_TexCoord[0].xyz / gl_TexCoord[0].q;
	float mapScale = 1.0 / 2048.0;

	vec4 shadowColor = shadow2D(texture_pallet1, shadowUV);

	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3( mapScale,  mapScale, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3( mapScale, -mapScale, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3( mapScale,  	  0, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3(-mapScale,  mapScale, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3(-mapScale, -mapScale, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3(-mapScale,  	  0, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3(        0,  mapScale, 0));
	shadowColor += shadow2D(texture_pallet1, shadowUV.xyz + vec3(        0, -mapScale, 0));

	shadowColor = shadowColor / 9.0;

	shadowColor += 0.3;
	shadowColor = clamp(shadowColor, 0.0, 1.0);

	//comine the final splatted pixel colour with lighting and output
	vec4 final =  (gl_LightSource[0].ambient + diffuse) * vec4(overall.rgb,1.0);
	//vec4 final =  vec4(overall.rgb,1.0);
	
	//vec4 shadow = texture2DProj(texture_pallet1, gl_TexCoord[0]);
	//float shad = shadow2DProj(texture_pallet1, gl_TexCoord[0]).r;

	if((shadowUV.x >= 0.01 && shadowUV.y >= 0.01 && shadowUV.x <= 0.99 && shadowUV.y <= 0.99))
	{
		final.rgb *= shadowColor.r;
	}
	
	//final.a = depthColor;
	
	gl_FragColor = final;
}
