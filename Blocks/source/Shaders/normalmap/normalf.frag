//Parallax Mapping // Fragment

uniform sampler2D decal;
uniform sampler2D parallaxMap;
uniform sampler2D normalMap;

varying vec3 eyeVec;
varying vec2 texCoord;
varying vec3 lightVec;

uniform vec4 materialColor;

void main()
{
	float invRadius = 100;

	float distSqr = dot(lightVec, lightVec);
	float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = lightVec * inversesqrt(distSqr);

	vec3 vVec = normalize(eyeVec);
	
	vec4 base = texture2D(decal, texCoord);

	if(base.a < 0.1) discard;
	
	vec3 bump = normalize( texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);

	//vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	float diffuse = max( dot(lVec, bump), 0.3 );
	
	//vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * 
					diffuse;	

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 
	                 0.5 );
	
	//vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular * 
					 specular;	
	
	/*vec4 col = ( vAmbient*base + 
					 vDiffuse*base + 
					 vSpecular) * att;*/

	vec4 col = ((diffuse*base + specular*base + materialColor) * 0.3);

	gl_FragColor = vec4(col.xyz,1);
}
