//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D depth_layer;
uniform sampler2D accumulation;

void main(void)
{
	//adattive blend
	vec3 final = texture2D(depth_layer,tex_coord).rgb;
	final += texture2D(accumulation,tex_coord).rgb;
	
	gl_FragColor = vec4(final,1.0);
}