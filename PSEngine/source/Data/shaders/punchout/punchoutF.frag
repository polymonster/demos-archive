//inputs
varying vec2 tex_coord;

//samplers
uniform sampler2D texture;

void main(void)
{
	vec4 pixel_colour = texture2D(texture, tex_coord);
	
	if(pixel_colour.a == 0.0) discard;
	gl_FragColor = vec4(pixel_colour.rgb,1.0);

}
