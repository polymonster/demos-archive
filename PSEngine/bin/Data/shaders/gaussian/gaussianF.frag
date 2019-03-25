//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;

//uniforms
uniform vec2 u_scale;

void main(void)
{
	float texture_offset[9];
	float gauss_kernel[9];
	
	texture_offset[0] = 4.0;
	gauss_kernel[0] = 0.004625;
	
	texture_offset[1] = 3.0;
	gauss_kernel[1] = 0.015625;

	texture_offset[2] = 2.0;
	gauss_kernel[2] = 0.09375;

	texture_offset[3] = 1.0;
	gauss_kernel[3] = 0.234375;

	texture_offset[4] = 0.0;
	gauss_kernel[4] = 0.3125;

	texture_offset[5] = -1.0;
	gauss_kernel[5] = 0.234375;

	texture_offset[6] = -2.0;
	gauss_kernel[6] = 0.09375;

	texture_offset[7] = -3.0;
	gauss_kernel[7] = 0.015625;

	texture_offset[8] = -4.0;
	gauss_kernel[8] = 0.004625;

	vec4 colour = vec4(0.0,0.0,0.0,0.0);
	vec2 blur_coord = vec2(0.0,0.0);
	
	for(int i = 0; i < 9; i++)
	{
		blur_coord.x = tex_coord.x + (texture_offset[i] * u_scale.x);
		blur_coord.y = tex_coord.y + (texture_offset[i] * u_scale.y);
		colour += texture2D(image, blur_coord) * gauss_kernel[i];
	}
	
	gl_FragColor = vec4(colour.r,colour.g,colour.b,1.0);
}
