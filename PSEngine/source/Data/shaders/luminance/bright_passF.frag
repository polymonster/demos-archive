//interpolants
varying vec2 tex_coord;

//sampl
uniform sampler2D image;
uniform sampler2D average_luminance;

const float BRIGHT_PASS_THRESHOLD = 5.0;
const float BRIGHT_PASS_OFFSET = 10.0f;

uniform float adapted_lum;
uniform float middle_grey;

void main(void)
{
	vec4 sample = texture2D( image, tex_coord );
	
	float adlum = texture2D(average_luminance, vec2(0.5,0.5)).r ;
	
	// Determine what the pixel's value will be after tone-mapping occurs
	sample.rgb *= middle_grey / (adlum + 0.0001);
	
	// Subtract out dark pixels
	sample.rgb -= BRIGHT_PASS_THRESHOLD;
	
	// Clamp to 0
	sample = max(sample, 0.0);
	
	// Map the resulting value into the 0 to 1 range. 
	sample.rgb /= (BRIGHT_PASS_OFFSET + sample.rgb);
    
	gl_FragColor = vec4(sample.r,sample.g,sample.b, 1.0);
}
