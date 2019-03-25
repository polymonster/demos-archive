//Grass shader // Fragment

varying vec2 texCoord;

uniform sampler2D decal;

#define epsilon 0.0001

void main()
{
	vec4 decalTex = texture2D(decal, texCoord.st);

	if(decalTex.a < 1.0) discard;

	gl_FragColor =  decalTex;
}
