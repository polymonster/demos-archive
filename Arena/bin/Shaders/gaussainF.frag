//Grass shader // Fragment

varying vec2 texCoord;

uniform sampler2D decal;

//const float textureOffset[7] = float[7](3.0,2.0,1.0,0.0,-1.0,-2.0,-3.0);
//const float gaussFilter[7] = float[7](0.015625,0.09375,0.234375,0.3125,0.234375,0.09375,0.015625);

uniform vec2 u_scale;

void main()
{
	float textureOffset[13];
	float gaussFilter[13];

	textureOffset[0] = 6.0;
	gaussFilter[0] = 0.00001;

	textureOffset[1] = 5.0;
	gaussFilter[1] = 0.0005;

	textureOffset[2] = 4.0;
	gaussFilter[2] = 0.001;

	textureOffset[3] = 3.0;
	gaussFilter[3] = 0.003;

	textureOffset[4] = 3.0;
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
	gaussFilter[12] = 0.00001;

	vec4 color = vec4(0.0,0.0,0.0,0.0);
	vec2 blurCoord = vec2(0.0,0.0);
	
	/*blurCoord.x = texCoord.x + (textureOffset[0] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[0] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[0];

	blurCoord.x = texCoord.x + (textureOffset[1] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[1] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[1];

	blurCoord.x = texCoord.x + (textureOffset[2] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[2] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[2];

	blurCoord.x = texCoord.x + (textureOffset[3] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[3] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[3];

	blurCoord.x = texCoord.x + (textureOffset[4] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[4] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[4];

	blurCoord.x = texCoord.x + (textureOffset[5] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[5] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[5];

	blurCoord.x = texCoord.x + (textureOffset[6] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[6] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[6];

	blurCoord.x = texCoord.x + (textureOffset[7] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[7] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[7];

	blurCoord.x = texCoord.x + (textureOffset[8] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[8] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[8];

	blurCoord.x = texCoord.x + (textureOffset[9] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[9] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[9];

	blurCoord.x = texCoord.x + (textureOffset[10] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[10] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[10];

	blurCoord.x = texCoord.x + (textureOffset[11] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[11] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[11];

	blurCoord.x = texCoord.x + (textureOffset[12] * u_scale.x);
	blurCoord.y = texCoord.y + (textureOffset[12] * u_scale.y);
	color += texture2D(decal, blurCoord.st) * gaussFilter[12];*/

	for(int i = 0; i < 13; i++)
	{
		blurCoord.x = texCoord.x + (textureOffset[i] * u_scale.x);
		blurCoord.y = texCoord.y + (textureOffset[i] * u_scale.y);
		color += texture2D(decal, blurCoord.st) * gaussFilter[i];
	}
	
	gl_FragColor = color;
	
}


