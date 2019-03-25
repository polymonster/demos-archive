//Bright Pass Shader // Fragment

varying vec2 texCoord;

uniform sampler2D decal;

uniform vec2 u_scale;

const float Luminance = 0.05;
const float fMiddleGray = 0.22;
const float fWhiteCutoff = 0.9;

void main()
{
	//OLD
	/*vec4 color = vec4(0.0,0.0,0.0,0.0);

	color = texture2D(decal, texCoord.st);
	
	float total = color.r + color.g + color.b;
	
	if(total <= 1.5)
	{
		discard;
	}
	
	gl_FragColor = color;*/
	
	vec4 ColorOut = texture2D( decal, texCoord.st );

    ColorOut *= fMiddleGray / ( Luminance + 0.001 );
    ColorOut *= ( 1.0 + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    ColorOut -= 5.0;

    ColorOut = max( ColorOut, 0.0 );

    ColorOut /= ( 10.0 + ColorOut );

    gl_FragColor = vec4(ColorOut.xyz, 1.0 );
	
}


