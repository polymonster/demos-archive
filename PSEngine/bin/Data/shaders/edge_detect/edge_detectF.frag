//consts
float normal_threshold = 0.0001;
float depth_threshold = 0.0001;
float normal_sensitivity = 1.0;
float depth_sensitivity = 1.0;
float edge_intensity = 2.0;

//interpolants
varying vec2 tex_coord;

//samplers
uniform sampler2D normal_depth_texture;
uniform vec2 px_size;
uniform float dilation;

void main(void)
{
	//edge detection wip
	/*vec2 texture_offset[5];
	float kernel[5];
	float weight = 2.0;
	
	texture_offset[0] = vec2(px_size.x,0.0);
	kernel[0] = 1.0;
	
	texture_offset[1] = vec2(-px_size.x,0.0);
	kernel[1] = 1.0;

	texture_offset[2] = vec2(0.0,0.0);
	kernel[2] = -4.0;

	texture_offset[3] = vec2(0.0,px_size.y);
	kernel[3] = 1.0;

	texture_offset[4] = vec2(0.0,px_size.y);
	kernel[4] = 1.0;

	float edge_weight = 0.0;
	vec2 sample_coord = vec2(0.0,0.0);
	
	for(int i = 0; i < 5; i++)
	{
		sample_coord = tex_coord + (texture_offset[i]);
		edge_weight += clamp(texture2D(depth_texture, sample_coord).b,0.0,1.0) * kernel[i] * weight;
	}

	gl_FragColor = vec4(edge_weight,edge_weight,edge_weight,1.0);*/

	vec4 n1 = texture2D(normal_depth_texture, tex_coord + vec2(-1.0, -1.0) * px_size);
	vec4 n2 = texture2D(normal_depth_texture, tex_coord + vec2( 1.0,  1.0) * px_size);
	vec4 n3 = texture2D(normal_depth_texture, tex_coord + vec2(-1.0,  1.0) * px_size);
	vec4 n4 = texture2D(normal_depth_texture, tex_coord + vec2( 1.0, -1.0) * px_size);

	vec4 diagonal_delta = abs(n1 - n2) + abs(n3 - n4);

	float normal_delta = dot(diagonal_delta.xyz, vec3(1.0,1.0,1.0));
	float depth_delta = diagonal_delta.w;
	
	// Filter out very small changes, in order to produce nice clean results.
	normal_delta = clamp((normal_delta - normal_threshold) * normal_sensitivity,0.0,1.0);
	depth_delta = clamp((depth_delta - depth_threshold) * depth_sensitivity,0.0,1.0);

	// Does this pixel lie on an edge?
	float amount = clamp(depth_delta,0.0,1.0) * edge_intensity;
	
	gl_FragColor = vec4(amount,amount,amount,1.0);
}