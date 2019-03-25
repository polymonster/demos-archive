////Water Shader // Fragment

uniform sampler2D sky;
uniform sampler2D stars;

varying vec2 texCoord1;
varying vec2 texCoord2;

void main()
{


	vec3 day = texture2D(sky,texCoord1.st).rgb;
	vec3 night = texture2D(stars,texCoord2.st).rgb;
	vec3 atmosphere = gl_Color.rgb;
	
	float blend = gl_Color.a;
	
	day *= 1.0 - blend;
	night *= blend;
	
	vec3 final = (day + night + atmosphere);

	gl_FragColor = vec4(final, 1.0);

}
