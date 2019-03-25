//Grass shader // Fragment

varying vec2 texCoord;

uniform sampler2D decal;

//const float textureOffset[7] = float[7](3.0,2.0,1.0,0.0,-1.0,-2.0,-3.0);
//const float gaussFilter[7] = float[7](0.015625,0.09375,0.234375,0.3125,0.234375,0.09375,0.015625);

uniform vec2 u_scale;

void main()
{
	float textureOffset[9];
	float gaussFilter[9];

	/*textureOffset[0] = 6.0;
	gaussFilter[0] = 0.00001;

	textureOffset[1] = 5.0;
	gaussFilter[1] = 0.0005;

	textureOffset[2] = 4.0;
	gaussFilter[2] = 0.001;

	textureOffset[3] = 3.0;
	gaussFilter[3] = 0.003;

	textureOffset[4] = 2.0;
	gaussFilter[4] = 0.075;

	textureOffset[5] = 1.0;
	gaussFilter[5] = 0.2;

	textureOffset[6] = 0.0;
	gaussFilter[6] = 0.5;

	textureOffset[7] = -1.0;
	gaussFilter[7] = 0.2;

	textureOffset[8] = -2.0;
	gaussFilter[8] = 0.075;

	textureOffset[9] = -3.0;
	gaussFilter[9] = 0.003;

	textureOffset[10] = -4.0;
	gaussFilter[10] = 0.001;

	textureOffset[11] = -5.0;
	gaussFilter[11] = 0.0005;

	textureOffset[12] = -6.0;
	gaussFilter[12] = 0.00001;*/
	
	textureOffset[0] = 4.0;
	gaussFilter[0] = 0.004625;
	
	textureOffset[1] = 3.0;
	gaussFilter[1] = 0.015625;

	textureOffset[2] = 2.0;
	gaussFilter[2] = 0.09375;

	textureOffset[3] = 1.0;
	gaussFilter[3] = 0.234375;

	textureOffset[4] = 0.0;
	gaussFilter[4] = 0.3125;

	textureOffset[5] = -1.0;
	gaussFilter[5] = 0.234375;

	textureOffset[6] = -2.0;
	gaussFilter[6] = 0.09375;

	textureOffset[7] = -3.0;
	gaussFilter[7] = 0.015625;

	textureOffset[8] = -4.0;
	gaussFilter[8] = 0.004625;

	vec4 color = vec4(0.0,0.0,0.0,0.0);
	vec2 blurCoord = vec2(0.0,0.0);
	
	for(int i = 0; i < 9; i++)
	{
		blurCoord.x = texCoord.x + (textureOffset[i] * u_scale.x);
		blurCoord.y = texCoord.y + (textureOffset[i] * u_scale.y);
		color += texture2D(decal, blurCoord.st) * gaussFilter[i];
	}
	
	gl_FragColor = vec4(color.rgb,1);
	
}


