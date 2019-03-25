//interpolants
varying vec4 vertex_position;
const float BIAS = 0.001;

//samplers
uniform sampler2D depth_layer;

void main(void)
{
	//get the depth by perspective divide, and covert to 0 - 1 range
	float depth = (vertex_position.z / vertex_position.w);
	depth = (depth * 0.5) + 0.5;
	
	vec2 tex_coord = (vertex_position.xy / vertex_position.w);
	tex_coord = (tex_coord * 0.5) + 0.5;
	
	vec4 previous_layer = texture2D(depth_layer,tex_coord);
	
	if(depth <= previous_layer.a) discard;
	
	gl_FragColor = vec4(0.0, 0.05, 0.0 , depth);

}