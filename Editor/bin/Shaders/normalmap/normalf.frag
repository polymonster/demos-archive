//Parallax Mapping // Fragment

uniform sampler2D decal;
uniform sampler2D normalMap;

uniform sampler2DShadow shadowMap;

varying vec3 eyeVec;
varying vec2 texCoord;
varying vec3 lightVec;

uniform int normalMapEnabled;
uniform int textureMapEnabled;

void main()
{
	float invRadius = 100.0;

	float distSqr = dot(lightVec, lightVec);
	float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = lightVec * inversesqrt(distSqr);

	vec3 vVec = normalize(eyeVec);
	
	vec4 base = vec4(0.0,0.0,1.0,1.0);
	float specular = 1.0;
	float diffuse = 1.0;
	vec4 vDiffuse = vec4(1.0,1.0,1.0,1.0); 
	
	if(textureMapEnabled == 1)
	{
		base = texture2D(decal, texCoord);
		
		if(base.a < 0.4) 
		{
			discard;
		}
	}
	
	if(normalMapEnabled == 1)
	{
		vec3 bump = normalize( texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);

		diffuse = max( dot(lVec, bump), 0.5);
		
		vec4 vDiffuse = gl_LightSource[0].diffuse * diffuse;	

		specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 0.3);
	}
	
	vec4 col = ((vDiffuse*base + specular*base) * 0.5);
	
	vec3 shadowUV = gl_TexCoord[0].xyz / gl_TexCoord[0].q;
	float mapScale = 1.0 / 4096.0;

	vec4 shadowColor = shadow2D(shadowMap, shadowUV);

	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale, -mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  	  0, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale, -mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  	  0, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0, -mapScale, 0));

	shadowColor = shadowColor / 9.0;

	shadowColor += 0.3;
	shadowColor = clamp(shadowColor, 0.0, 1.0);
	
	if((shadowUV.x >= 0.01 && shadowUV.y >= 0.01 && shadowUV.x <= 0.99 && shadowUV.y <= 0.99))
	{
		col.rgb *= shadowColor.r;
	}
	
	gl_FragColor = vec4(col.xyz,1.0);
}
