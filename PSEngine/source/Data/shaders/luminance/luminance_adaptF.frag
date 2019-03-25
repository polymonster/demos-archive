//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D current_lum_tex;
uniform sampler2D current_adapted_lum_tex;

uniform float	ellapsed_time;

void main(void)
{
    float adapted_lum = texture2D(current_adapted_lum_tex, vec2(0.5, 0.5)).r;
    float current_lum = texture2D(current_lum_tex, vec2(0.5, 0.5)).r;
    
	//adapt at 2% per frame
    float new_adpatation = adapted_lum + (current_lum - adapted_lum) * ( 1.0 - pow( 0.98, ellapsed_time * 30.0) );
	
	gl_FragColor = vec4(new_adpatation, new_adpatation, new_adpatation, 1.0);
}
