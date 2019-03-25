//Parallax Mapping // Fragment

uniform sampler2D decal;
uniform sampler2D parallaxMap;
uniform sampler2D normalMap;

varying vec3 direction;
varying vec3 eyeVec; 
varying vec2 texCoord; 
varying vec3 myCam;

void main()
{
	//parallax mapping
	float scale = 0.02;
	float bias = 0.01;

	/*float height = texture2D(parallaxMap,texCoord.st).r;
	float v = height * scale - bias;
	vec3 eye = normalize(eyeVec);
	vec2 newCoords = texCoord + (eye.xy * v);*/

	vec2 newCoords = texCoord;
	
	//normal mapping
	vec3 normalTex = texture2D(normalMap, newCoords).rgb;
   	//expand out the normal texture
   	normalTex = (normalTex - 0.5f) * 2;

	vec3 lightD = -normalize(direction);
	// Calculate the half angle.
  	vec3 cam = normalize(myCam);
   	vec3 halfAngle = lightD + cam;
   	halfAngle = normalize(halfAngle);

   	// Calculate the diffuse lighting term and the specular lighting term.
   	float diffuse = clamp(dot(normalTex, lightD),0.0,1.0);
   	float specular = clamp(dot(normalTex, halfAngle),0.0,1.0);

   	float specFactor = specular * specular;
   	specular = specFactor * specFactor;
   	specFactor = specular * specular;

	//hardcode material property
	vec3 dMat = vec3(0.7f, 0.7f, 0.7f);
   	vec3 sMat = vec3(0.1f, 0.1f, 0.1f);
   	float ambient = 0.4f;

	vec3 color = texture2D(decal,newCoords).rgb;

	vec3 final = color * (dMat * (ambient + diffuse) + sMat * specFactor);

	gl_FragColor = vec4(final ,1.0);
	
}
