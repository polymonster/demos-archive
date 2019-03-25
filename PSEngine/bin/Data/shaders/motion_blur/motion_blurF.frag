//consts
//const  float anti_alias_avg = 0.08333333;
const  float anti_alias_avg = 0.125;

//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;
uniform sampler2D pixel_velocity_image;
uniform sampler2D edge_detect_image;

uniform int number_of_samples;
uniform float pixel_blur_amount;

void main(void)
{
	//anti-alias wip
	/*vec2 px_size = vec2(1.0 / 1024.0, 1.0 / 768.0);
	vec4 final = vec4(0.0,0.0,0.0,0.0);
	
	vec2 lookup = tex_coord;
	float weight = 0.0;
	
	final = texture2DLod( image, lookup, 0.0 );
	
	vec4 accum = vec4(0.0,0.0,0.0,0.0);
	
	//4x
	lookup = clamp(tex_coord + vec2(-px_size.x,0.0),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;

	lookup = clamp(tex_coord + vec2(px_size.x,0.0),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;
	
	lookup = clamp(tex_coord + vec2(0.0,-px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;
	
	lookup = clamp(tex_coord + vec2(0.0,px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;
	
	//8x
	lookup = clamp(tex_coord + vec2(-px_size.x,-px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;

	lookup = clamp(tex_coord + vec2( px_size.x,-px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;
	
	lookup = clamp(tex_coord + vec2( px_size.x, px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;
	
	lookup = clamp(tex_coord + vec2(-px_size.x, px_size.y),0.0,1.0);
	weight = clamp(texture2DLod(edge_detect_image, lookup, 0.0).r,0.0,1.0);
	accum += texture2DLod( image, lookup, 0.0 ) * weight * anti_alias_avg;

	final += accum;*/

	//motion blur
	vec4 final = texture2DLod( image,tex_coord, 0.0 );
	vec2 pixel_velocity = texture2D( pixel_velocity_image, tex_coord ).xy * pixel_blur_amount;
	pixel_velocity = clamp(pixel_velocity,-0.15,0.15);

	float a = 1.0;
	for(int i = 1; i < number_of_samples; i++)
	{
		vec2 lookup_pos = tex_coord + (pixel_velocity * (float(i) / float(number_of_samples)));
		
		//yucky branching
		if(lookup_pos.x >= 0.0 && lookup_pos.x <= 1.0 && lookup_pos.y >= 0.0 && lookup_pos.y <= 1.0)
		{
			final += texture2DLod( image, lookup_pos, 0.0 );
			a++;
		}
	}
	
	final /= a;
	
	gl_FragColor = vec4(final.r,final.g,final.b,1.0);

}