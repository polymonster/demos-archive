//consts
const float bias = 0.0001;

//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D hdr_image;
uniform sampler2D bloom_image;
uniform sampler2D adapted_luminance_image;

uniform float bloom_scale;
uniform float middle_grey;

void main(void)
{
	//sample textures
	vec4 final = texture2DLod( hdr_image, tex_coord, 0.0 );
	vec4 bloom = texture2DLod( bloom_image, tex_coord, 0.0 );
	float adapted_luminance = texture2DLod( adapted_luminance_image, vec2(0.5,0.5), 0.0).r;
	
	//tone map
	final *= (middle_grey / (adapted_luminance + bias));
	final /= (1.0 + final);
	
	//apply bloom
	final += bloom * bloom_scale;

	gl_FragColor = vec4(final.r,final.g,final.b,1.0);
}
