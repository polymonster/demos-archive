//Grass shader // Fragment

varying vec2 texCoord;

uniform sampler2D decal;

/*const vec2 gaussFilter[7] = vec2[7]
(
	vec2(-3.0,0.015625),
	vec2(-2.0,0.09375),
	vec2(-1.0,0.234375),
	vec2(0.0,0.3125),
	vec2(1.0,0.234375),
	vec2(2.0,0.09375),
	vec2(3.0,0.015625)
);*/

const float textureOffset[7] = float[7](3.0,2.0,1.0,0.0,-1.0,-2.0.-3.0);
const float gaussFilter[7] = float[7](0.015625,0.09375,0.234375,0.3125,0.234375,0.09375,0.015625);
//const float gaussFilter[7] = float[7](0.1,0.2,0.4,0.8,0.4,0.2,0.1);

uniform vec2 u_scale;

void main()
{
	//vec4 color = texture2D(decal,texCoord.st);
	//vec4 color;
	//color.a = texture2D(decal,texCoord.st).a;

	//vec2 blurCoord;

	//vec2 scale = vec2(0.00390625,0);

	//for( int i = 0; i < 7; i++ )
	//{
	//	blurCoord.x = texCoord.x + (textureOffset[i] * u_scale.x);
	//	blurCoord.y = texCoord.y + (textureOffset[i] * u_scale.y);

	//	color += (texture2D(decal, blurCoord.xy) * gaussFilter[i]);
	//}	

	//gl_FragColor =  color;

	gl_FragColor =  vec4(0,0,0,0);
}


