//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;
uniform float texel_offset;

void main(void)
{
	vec4 initial = texture2D(image, vec2(0.0,0.0));
	
	float adlum = initial.r;
	
	adlum += texture2D(image, vec2(0.25,0.0)).r;
	adlum += texture2D(image, vec2(0.5,0.0)).r;
	adlum += texture2D(image, vec2(0.75,0.0)).r;
	
	adlum += texture2D(image, vec2(0.0,0.25)).r;
	adlum += texture2D(image, vec2(0.25,0.25)).r;
	adlum += texture2D(image, vec2(0.5,0.25)).r;
	adlum += texture2D(image, vec2(0.75,0.25)).r;
	
	adlum += texture2D(image, vec2(0.0,0.5)).r;
	adlum += texture2D(image, vec2(0.25,0.5)).r;
	adlum += texture2D(image, vec2(0.5,0.5)).r;
	adlum += texture2D(image, vec2(0.75,0.5)).r;
	
	adlum += texture2D(image, vec2(0.0,0.75)).r;
	adlum += texture2D(image, vec2(0.25,0.75)).r;
	adlum += texture2D(image, vec2(0.5,0.75)).r;
	adlum += texture2D(image, vec2(0.75,0.75)).r;

	adlum /= 16.0;
	
	adlum = exp( adlum );
    
	gl_FragColor = vec4(adlum,adlum,adlum,1.0);
}
