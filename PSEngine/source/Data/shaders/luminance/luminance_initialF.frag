//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D image;

const vec3 LUMINANCE_VECTOR  = vec3(0.2125, 0.7154, 0.0721);
const float	bias = 0.0001;

void main(void)
{
    vec3 sample;
	vec4 initial;
	
    float  log_lum_sum = 0.0;

	float texel_size = 1.0 / 256.0;
	
	initial = texture2D(image, tex_coord);
	sample = initial.rgb;
	
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias );

	sample = texture2D(image, tex_coord + vec2( texel_size,  texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2( texel_size, -texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2( texel_size,  	  0)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2(-texel_size,  texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2(-texel_size, -texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2(-texel_size,  	  0)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2(        0,  texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	sample = texture2D(image, tex_coord + vec2(        0, -texel_size)).rgb;
	log_lum_sum += log( dot(sample, LUMINANCE_VECTOR) + bias);
	
	log_lum_sum /= 9.0;
   
	gl_FragColor = vec4(log_lum_sum, log_lum_sum, log_lum_sum, 1.0);
}
