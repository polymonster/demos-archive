//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;
uniform float texel_offset;


void main(void)
{
	vec4 sample;
	float texel_size = texel_offset;

	sample = texture2D(image, tex_coord);
	
	vec2 newTexCoord = clamp(tex_coord + vec2( texel_size,  texel_size),0.0,1.0);
	sample += texture2D(image, newTexCoord);
	
	newTexCoord = clamp(tex_coord + vec2( texel_size, -texel_size),0.0,1.0);
	sample += texture2D(image, newTexCoord);
	
	newTexCoord = clamp(tex_coord + vec2( texel_size,  	  0),0.0,1.0);
	sample += texture2D(image, newTexCoord);

	newTexCoord = clamp(tex_coord + vec2(-texel_size,  texel_size),0.0,1.0);	
	sample += texture2D(image, newTexCoord);

	newTexCoord = clamp(tex_coord + vec2(-texel_size, -texel_size),0.0,1.0);	
	sample += texture2D(image, newTexCoord);

	newTexCoord = clamp(tex_coord + vec2(-texel_size,  	  0),0.0,1.0);	
	sample += texture2D(image, newTexCoord);

	newTexCoord = clamp(tex_coord + vec2(        0,  texel_size),0.0,1.0);	
	sample += texture2D(image, newTexCoord);

	newTexCoord = clamp(tex_coord + vec2(        0, -texel_size),0.0,1.0);	
	sample += texture2D(image, newTexCoord);
	
	sample /= 9.0;
    
	gl_FragColor = vec4(sample.rgb,1.0);
}
