//interpolants
varying vec3 position;
varying vec2 tex_coord;
varying vec4 material_col;
varying vec2 pixel_velocity;
varying vec4 current_ss_position;
varying vec4 previous_ss_position;
varying vec4 grandfather_ss_position;
varying float positionw;

varying mat3 TBN;

//samplers
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

//uniforms
uniform mat4 view_mat;
uniform mat4 view_mat_nrm;

#define WORLD_SPACE

void main (void)
{
	//declaractions
	vec4 albedo;
	vec3 normal_buffer;
	vec3 position_buffer;
	
	//read textures
	vec4 diffuse_texture = texture2D(diffuse_map, tex_coord);
	vec3 normal = normalize( texture2D(normal_map, tex_coord).xyz * 2.0 - 1.0);
	vec4 specular_texture = texture2D(specular_map, tex_coord);
	
	//invert the normals y instead of above this is dependant on the setup of the normal map
	normal.y *= -1.0;
	normal_buffer = (TBN * normal);
	
	#ifndef WORLD_SPACE
		normal_buffer = normalize( vec4(normal_buffer.xyz,1.0) * view_mat_nrm).xyz;
		position_buffer = (vec4(position.xyz,1.0)  * view_mat).xyz;
	#else
		position_buffer = position.xyz;
	#endif
	
	albedo = diffuse_texture;
	
	//calculate motion blur velocity
	float depth = (current_ss_position.z - 1.0) / 10000.0;
		
	vec4 current_ss = current_ss_position / current_ss_position.w;
	vec4 previous_ss = previous_ss_position / previous_ss_position.w;
	vec4 grandfather_ss = grandfather_ss_position / grandfather_ss_position.w;
	
	vec2 current_movement = ((current_ss - previous_ss).xy);
	vec2 previous_movement = ((previous_ss - grandfather_ss).xy);
	
	float cur_mag = current_movement.x * current_movement.x + current_movement.y * current_movement.y;
	float prev_mag = previous_movement.x * previous_movement.x + previous_movement.y * previous_movement.y;
	
	float current_branchless = max(prev_mag - cur_mag, 0.0);
	float prev_branchless = max(cur_mag - prev_mag, 0.0);
	
	vec2 pixel_velocity = ((current_movement / 2.0) * max(current_branchless,1.0)) + ((previous_movement / 2.0) * max(prev_branchless,1.0));
	
	gl_FragData[0] = vec4( normal_buffer, 1.0 );
	gl_FragData[1] = vec4( position_buffer, specular_texture.r );
	gl_FragData[2] = vec4( albedo.rgb, material_col.a );
	gl_FragData[3] = vec4( pixel_velocity.xy, depth , 1.0 );
}
